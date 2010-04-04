/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009, 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of GLToy.
//
// GLToy is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// GLToy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with GLToy.
// If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's headers
#include <Render/GLToy_Render.h>
#include <Render/Platform_GLToy_Render.h>

// GLToy
#include <Core/Console/GLToy_Console.h>
#include <Core/Data Structures/GLToy_BinaryTree.h>
#include <Core/GLToy_Timer.h>
#include <Maths/GLToy_Maths.h>
#include <Maths/GLToy_Vector.h>
#include <Render/Font/GLToy_Font.h>
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_Renderable.h>
#include <Render/GLToy_RenderFunctor.h>
#include <Render/GLToy_Texture.h>
#include <Render/Shader/GLToy_Shader_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

float GLToy_Render::s_fFOV = 90.0f;
float GLToy_Render::s_fAspectRatio = 1.0f;
bool GLToy_Render::s_bDrawFPS = GLToy_IsDebugBuild();
GLToy_BinaryTree< const GLToy_Renderable*, float > GLToy_Render::s_xTransparents;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Render::Initialise()
{
    if( !Platform_Initialise() )
    {
        return false;
    }

    GLToy_InitialiserCall( GLToy_Camera );
    GLToy_InitialiserCall( GLToy_Shader_System );
    GLToy_InitialiserCall( GLToy_Texture_System );
    GLToy_InitialiserCall( GLToy_Font_System );

    if( !Project_Initialise() )
    {
        return false;
    }

    GLToy_Console::RegisterVariable( "showfps", &s_bDrawFPS );

    return true;
}

void GLToy_Render::Shutdown()
{
    Project_Shutdown();

    GLToy_Font_System::Shutdown();
    GLToy_Texture_System::Shutdown();
    GLToy_Shader_System::Shutdown();

    Platform_Shutdown();
}

void GLToy_Render::BeginRender()
{
    EnableDepthTesting();
    EnableDepthWrites();
    SetDepthFunction( DEPTH_LEQUAL );

    Platform_BeginRender();

    GLToy_Camera::ApplyTransforms();
}

void GLToy_Render::BeginRender2D()
{
    DisableDepthTesting();
    SetOrthogonalProjectionMatrix();
    SetIdentityViewMatrix();
}

void GLToy_Render::Render()
{
    Project_Render();

    // here we render the transparent object tree ...
    EnableBlending();
    EnableDepthTesting();
    DisableDepthWrites();

    // the magic of binary trees draws everything depth sorted
    s_xTransparents.Traverse( GLToy_IndirectRenderTransparentFunctor< const GLToy_Renderable >() );

    // clean up for next frame
    s_xTransparents.Clear();
}

void GLToy_Render::Render2D()
{
    // Project_Render2D();

    if( s_bDrawFPS )
    {
        UseProgram( 0 );
        // draw fps counter, we should have the console's font by now
        GLToy_Font* pxFont = GLToy_Font_System::FindFont( GLToy_Hash_Constant( "console" ) );
		if( pxFont )
		{
			GLToy_String szFPSString;
			szFPSString.SetToFormatString( "%.2f fps", GLToy_Maths::Max( GLToy_Timer::GetSmoothedFrameRate(), 0.01f ) );
			pxFont->RenderString( szFPSString, GLToy_Render::GetMaxX() - szFPSString.GetLength() * pxFont->GetWidth(), 1.0f - pxFont->GetHeight() );
		}
    }
}


void GLToy_Render::EndRender()
{
    Flush();

    Platform_EndRender();
}

void GLToy_Render::RegisterTransparent( const GLToy_Renderable* const pxTransparent, const float fSquaredDistanceFromCamera )
{
    s_xTransparents.AddNode( pxTransparent, -fSquaredDistanceFromCamera );
}

bool GLToy_Render::Platform_Initialise()
{
    return Platform_GLToy_Render::Initialise();
}

void GLToy_Render::Platform_Shutdown()
{
    Platform_GLToy_Render::Shutdown();
}

void GLToy_Render::Platform_BeginRender()
{
    Platform_GLToy_Render::BeginRender();
}

void GLToy_Render::Platform_EndRender()
{
    Platform_GLToy_Render::EndRender();
}

/////////////////////////////////////////////////////////////////////////////////////////////
// G L   I N T E R F A C E
/////////////////////////////////////////////////////////////////////////////////////////////

u_int GLToy_Render::GetError()
{
    return Platform_GLToy_Render::GetError();
}

void GLToy_Render::ClearDepth( const float fDepth )
{
    Platform_GLToy_Render::ClearDepth( fDepth );
}

void GLToy_Render::SetDepthFunction( const u_int uDepthFunction )
{
    Platform_GLToy_Render::SetDepthFunction( uDepthFunction );
}

void GLToy_Render::SetViewport( const int iX, const int iY, const u_int uWidth, const u_int uHeight )
{
    s_fAspectRatio = static_cast< float >( uWidth ) / static_cast< float >( uHeight );
    Platform_GLToy_Render::SetViewport( iX, iY, uWidth, uHeight );
}

void GLToy_Render::SetIdentityProjectionMatrix()
{
    Platform_GLToy_Render::SetIdentityProjectionMatrix();
}

void GLToy_Render::SetPerspectiveProjectionMatrix()
{
    Platform_GLToy_Render::SetIdentityProjectionMatrix();
    Platform_GLToy_Render::SetPerspectiveProjectionMatrix( s_fFOV, s_fAspectRatio );
}

void GLToy_Render::SetOrthogonalProjectionMatrix()
{
    Platform_GLToy_Render::SetIdentityProjectionMatrix();
    Platform_GLToy_Render::SetOrthogonalProjectionMatrix( s_fAspectRatio );
}

void GLToy_Render::SetIdentityViewMatrix()
{
    Platform_GLToy_Render::SetIdentityViewMatrix();
}

void GLToy_Render::SetLookAtViewMatrix( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xLookAt, const GLToy_Vector_3& xUp )
{
    Platform_GLToy_Render::SetIdentityViewMatrix();
    Platform_GLToy_Render::SetLookAtViewMatrix( xPosition, xLookAt, xUp );
}

void GLToy_Render::Translate( const GLToy_Vector_3& xTranslation )
{
    Platform_GLToy_Render::Translate( xTranslation );
}

void GLToy_Render::Rotate( const GLToy_Vector_3& xAxis, const float fAngle )
{
    Platform_GLToy_Render::Rotate( xAxis, fAngle );
}

void GLToy_Render::Transform( const GLToy_Matrix_3& xMatrix )
{
    Platform_GLToy_Render::Transform( xMatrix );
}

void GLToy_Render::PushViewMatrix()
{
    Platform_GLToy_Render::PushViewMatrix();
}

void GLToy_Render::PopViewMatrix()
{
    Platform_GLToy_Render::PopViewMatrix();
}

void GLToy_Render::SubmitTexturedQuad2D( const float fXMin, const float fYMin, const float fXMax, const float fYMax, const float fUMin, const float fVMin, const float fUMax, const float fVMax )
{
    GLToy_Render::SubmitUV( GLToy_Vector_3( fUMin, fVMax, 0.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( fXMin, fYMin, 0.0f ) );

    GLToy_Render::SubmitUV( GLToy_Vector_3( fUMax, fVMax, 0.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( fXMax, fYMin, 0.0f ) );

    GLToy_Render::SubmitUV( GLToy_Vector_3( fUMax, fVMin, 0.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( fXMax, fYMax, 0.0f ) );

    GLToy_Render::SubmitUV( GLToy_Vector_3( fUMin, fVMin, 0.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( fXMin, fYMax, 0.0f ) );
}

void GLToy_Render::SubmitTexturedQuad2D( const GLToy_Vector_2& xPosition, const GLToy_Vector_2& xSize, const float fUMin, const float fVMin, const float fUMax, const float fVMax )
{
    GLToy_Render::SubmitUV( GLToy_Vector_3( fUMin, fVMax, 0.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( xPosition[ 0 ], xPosition[ 1 ], 0.0f ) );

    GLToy_Render::SubmitUV( GLToy_Vector_3( fUMax, fVMax, 0.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( xPosition[ 0 ] + xSize[ 0 ], xPosition[ 1 ], 0.0f ) );

    GLToy_Render::SubmitUV( GLToy_Vector_3( fUMax, fVMin, 0.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( xPosition[ 0 ] + xSize[ 0 ], xPosition[ 1 ] + xSize[ 1 ], 0.0f ) );

    GLToy_Render::SubmitUV( GLToy_Vector_3( fUMin, fVMin, 0.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( xPosition[ 0 ], xPosition[ 1 ] + xSize[ 1 ], 0.0f ) );
}

void GLToy_Render::StartSubmittingLines()
{
    Platform_GLToy_Render::StartSubmittingLines();
}

void GLToy_Render::StartSubmittingLineStrip()
{
    Platform_GLToy_Render::StartSubmittingLineStrip();
}

void GLToy_Render::StartSubmittingLineLoop()
{
    Platform_GLToy_Render::StartSubmittingLineLoop();
}

void GLToy_Render::StartSubmittingTriangles()
{
    Platform_GLToy_Render::StartSubmittingTriangles();
}

void GLToy_Render::StartSubmittingQuads()
{
    Platform_GLToy_Render::StartSubmittingQuads();
}

void GLToy_Render::StartSubmittingTriangleStrip()
{
    Platform_GLToy_Render::StartSubmittingTriangleStrip();
}

void GLToy_Render::StartSubmittingTriangleFan()
{
    Platform_GLToy_Render::StartSubmittingTriangleFan();
}

void GLToy_Render::StartSubmittingPolygon()
{
    Platform_GLToy_Render::StartSubmittingPolygon();
}

void GLToy_Render::EndSubmit()
{
    Platform_GLToy_Render::EndSubmit();
}

void GLToy_Render::SubmitVertex( const GLToy_Vector_3& xVertex )
{
    Platform_GLToy_Render::SubmitVertex( xVertex );
}

void GLToy_Render::SubmitVertex( const float fX, const float fY, const float fZ )
{
    Platform_GLToy_Render::SubmitVertex( fX, fY, fZ );
}

void GLToy_Render::SubmitNormal( const GLToy_Vector_3& xNormal )
{
    Platform_GLToy_Render::SubmitNormal( xNormal );
}

void GLToy_Render::SubmitColour( const GLToy_Vector_3& xColour )
{
    Platform_GLToy_Render::SubmitColour( xColour );
}

void GLToy_Render::SubmitColour( const GLToy_Vector_4& xColour )
{
    Platform_GLToy_Render::SubmitColour( xColour );
}

void GLToy_Render::SubmitUV( const GLToy_Vector_2& xUV, const u_int uTextureUnit )
{
    Platform_GLToy_Render::SubmitUV( xUV, uTextureUnit );
}

void GLToy_Render::SubmitUV( const GLToy_Vector_3& xUV, const u_int uTextureUnit )
{
    Platform_GLToy_Render::SubmitUV( xUV, uTextureUnit );
}

void GLToy_Render::Flush()
{
    Platform_GLToy_Render::Flush();
}

void GLToy_Render::DisableBlending()
{
    Platform_GLToy_Render::DisableBlending();
}

void GLToy_Render::EnableBlending()
{
    Platform_GLToy_Render::EnableBlending();
}

void GLToy_Render::DisableDepthTesting()
{
    Platform_GLToy_Render::DisableDepthTesting();
}

void GLToy_Render::EnableDepthTesting()
{
    Platform_GLToy_Render::EnableDepthTesting();
}

void GLToy_Render::DisableDepthWrites()
{
    Platform_GLToy_Render::DisableDepthWrites();
}

void GLToy_Render::EnableDepthWrites()
{
    Platform_GLToy_Render::EnableDepthWrites();
}

void GLToy_Render::SetBlendFunction( const u_int uSourceBlend, const u_int uDestinationBlend )
{
    Platform_GLToy_Render::SetBlendFunction( uSourceBlend, uDestinationBlend );
}

void GLToy_Render::DisableBackFaceCulling()
{
    Platform_GLToy_Render::DisableBackFaceCulling();
}

void GLToy_Render::EnableBackFaceCulling()
{
    Platform_GLToy_Render::EnableBackFaceCulling();
}

void GLToy_Render::SetCCWFaceWinding()
{
    Platform_GLToy_Render::SetCCWFaceWinding();
}

void GLToy_Render::SetCWFaceWinding()
{
    Platform_GLToy_Render::SetCWFaceWinding();
}

bool GLToy_Render::IsShader( const u_int uID )
{
    return Platform_GLToy_Render::IsShader( uID );
}

u_int GLToy_Render::CreateFragmentShader()
{
    return Platform_GLToy_Render::CreateFragmentShader();
}

u_int GLToy_Render::CreateVertexShader()
{
    return Platform_GLToy_Render::CreateVertexShader();
}

u_int GLToy_Render::CreateProgram()
{
    return Platform_GLToy_Render::CreateProgram();
}

void GLToy_Render::DeleteShader( u_int uShaderID )
{
    Platform_GLToy_Render::DeleteShader( uShaderID );
}

void GLToy_Render::DeleteProgram( u_int uProgramID )
{
    Platform_GLToy_Render::DeleteProgram( uProgramID );
}

void GLToy_Render::ValidateProgram( u_int uProgramID )
{
    Platform_GLToy_Render::ValidateProgram( uProgramID );
}

void GLToy_Render::CompileShader( u_int uShaderID )
{
    Platform_GLToy_Render::CompileShader( uShaderID );
}

void GLToy_Render::LinkProgram( u_int uProgramID )
{
    Platform_GLToy_Render::LinkProgram( uProgramID );
}

void GLToy_Render::UseProgram( u_int uProgramID )
{
    Platform_GLToy_Render::UseProgram( uProgramID );
}

void GLToy_Render::AttachShader( u_int uProgramID, u_int uShaderID )
{
    Platform_GLToy_Render::AttachShader( uProgramID, uShaderID );
}

void GLToy_Render::DetachShader( u_int uProgramID, u_int uShaderID )
{
    Platform_GLToy_Render::DetachShader( uProgramID, uShaderID );
}

void GLToy_Render::ShaderSource( u_int uShaderID, int iStringCount, char** ppszStrings, const int* piLengths )
{
    Platform_GLToy_Render::ShaderSource( uShaderID, iStringCount, ppszStrings, piLengths );
}

void GLToy_Render::GetProgramInfoLog( u_int uProgramID, int iMaxLength, int* iLength, char* szInfoLog )
{
    Platform_GLToy_Render::GetProgramInfoLog( uProgramID, iMaxLength,  iLength, szInfoLog );
}

void GLToy_Render::GetShaderInfoLog( u_int uShaderID, int iMaxLength, int* iLength, char* szInfoLog )
{
    Platform_GLToy_Render::GetShaderInfoLog( uShaderID, iMaxLength, iLength, szInfoLog );
}

u_int GLToy_Render::GetUniformID( u_int uProgramID, const char* szName )
{
    return Platform_GLToy_Render::GetUniformID( uProgramID, szName );
}

u_int GLToy_Render::GetAttributeID( u_int uProgramID, const char* szName )
{
    return Platform_GLToy_Render::GetAttributeID( uProgramID, szName );
}

void GLToy_Render::BindAttributeID( u_int uProgramID, u_int uIndex, const char* szName )
{
    Platform_GLToy_Render::BindAttributeID( uProgramID, uIndex, szName );
}

void GLToy_Render::SetUniform( u_int uUniformID, int iValue )
{
    Platform_GLToy_Render::SetUniform( uUniformID, iValue );
}

void GLToy_Render::SetUniform( u_int uUniformID, int iValue1, int iValue2 )
{
    Platform_GLToy_Render::SetUniform( uUniformID, iValue1, iValue2 );
}

void GLToy_Render::SetUniform( u_int uUniformID, int iValue1, int iValue2, int iValue3 )
{
    Platform_GLToy_Render::SetUniform( uUniformID, iValue1, iValue2, iValue3 );
}

void GLToy_Render::SetUniform( u_int uUniformID, int iValue1, int iValue2, int iValue3, int iValue4 )
{
    Platform_GLToy_Render::SetUniform( uUniformID, iValue1, iValue2, iValue3, iValue4 );
}

void GLToy_Render::SetUniform( u_int uUniformID, float fValue )
{
    Platform_GLToy_Render::SetUniform( uUniformID, fValue );
}

void GLToy_Render::SetUniform( u_int uUniformID, float fValue1, float fValue2 )
{
    Platform_GLToy_Render::SetUniform( uUniformID, fValue1, fValue2 );
}

void GLToy_Render::SetUniform( u_int uUniformID, float fValue1, float fValue2, float fValue3 )
{
    Platform_GLToy_Render::SetUniform( uUniformID, fValue1, fValue2, fValue3 );
}

void GLToy_Render::SetUniform( u_int uUniformID, float fValue1, float fValue2, float fValue3, float fValue4 )
{
    Platform_GLToy_Render::SetUniform( uUniformID, fValue1, fValue2, fValue3, fValue4 );
}

void GLToy_Render::SetAttribute( u_int uAttributeID, int iValue )
{
    Platform_GLToy_Render::SetAttribute( uAttributeID, iValue );
}

void GLToy_Render::SetAttribute( u_int uAttributeID, int iValue1, int iValue2 )
{
    Platform_GLToy_Render::SetAttribute( uAttributeID, iValue1, iValue2 );
}

void GLToy_Render::SetAttribute( u_int uAttributeID, int iValue1, int iValue2, int iValue3 )
{
    Platform_GLToy_Render::SetAttribute( uAttributeID, iValue1, iValue2, iValue3 );
}

void GLToy_Render::SetAttribute( u_int uAttributeID, int iValue1, int iValue2, int iValue3, int iValue4 )
{
    Platform_GLToy_Render::SetAttribute( uAttributeID, iValue1, iValue2, iValue3, iValue4 );
}

void GLToy_Render::SetAttribute( u_int uAttributeID, float fValue )
{
    Platform_GLToy_Render::SetAttribute( uAttributeID, fValue );
}

void GLToy_Render::SetAttribute( u_int uAttributeID, float fValue1, float fValue2 )
{
    Platform_GLToy_Render::SetAttribute( uAttributeID, fValue1, fValue2 );
}

void GLToy_Render::SetAttribute( u_int uAttributeID, float fValue1, float fValue2, float fValue3 )
{
    Platform_GLToy_Render::SetAttribute( uAttributeID, fValue1, fValue2, fValue3 );
}

void GLToy_Render::SetAttribute( u_int uAttributeID, float fValue1, float fValue2, float fValue3, float fValue4 )
{
    Platform_GLToy_Render::SetAttribute( uAttributeID, fValue1, fValue2, fValue3, fValue4 );
}
