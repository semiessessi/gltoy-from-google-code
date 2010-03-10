/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's headers
#include <Render/GLToy_Render.h>
#include <Render/Platform_GLToy_Render.h>

// GLToy
#include <Maths/GLToy_Vector.h>
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_Renderable.h>
#include <Render/GLToy_RenderFunctor.h>
#include <Render/GLToy_Texture.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

float GLToy_Render::s_fFOV = 90.0f;

const GLToy_RenderFunctor< GLToy_Renderable > GLToy_Render::RenderableFunctor = GLToy_RenderFunctor< GLToy_Renderable >();

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Render::Initialise()
{
    if( !Platform_Initialise() )
    {
        return false;
    }

    GLTOY_INITIALISER_CALL( GLToy_Camera );
    GLTOY_INITIALISER_CALL( GLToy_Texture_System );

    if( !Project_Initialise() )
    {
        return false;
    }

    return true;
}

void GLToy_Render::Shutdown()
{
    Project_Shutdown();

    GLToy_Texture_System::Shutdown();

    Platform_Shutdown();
}

void GLToy_Render::BeginRender()
{
    Platform_BeginRender();

    GLToy_Camera::ApplyTransforms();
}

void GLToy_Render::Render()
{
    Project_Render();
}

void GLToy_Render::EndRender()
{
    GLToy_Render::Flush();

    Platform_EndRender();
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

void GLToy_Render::SetViewport( const int iX, const int iY, const u_int uWidth, const u_int uHeight )
{
    Platform_GLToy_Render::SetViewport( iX, iY, uWidth, uHeight );
}

void GLToy_Render::SetIdentityProjectionMatrix()
{
    Platform_GLToy_Render::SetIdentityProjectionMatrix();
}

void GLToy_Render::SetPerspectiveProjectionMatrix( const u_int uViewportWidth, const u_int uViewportHeight )
{
    Platform_GLToy_Render::SetPerspectiveProjectionMatrix( s_fFOV, uViewportWidth, uViewportHeight );
}

void GLToy_Render::SetOrthogonalProjectionMatrix()
{
    Platform_GLToy_Render::SetOrthogonalProjectionMatrix();
}

void GLToy_Render::SetIdentityViewMatrix()
{
    Platform_GLToy_Render::SetIdentityViewMatrix();
}

void GLToy_Render::SetLookAtViewMatrix( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xLookAt, const GLToy_Vector_3& xUp )
{
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

void GLToy_Render::SubmitNormal( const GLToy_Vector_3& xNormal )
{
    Platform_GLToy_Render::SubmitNormal( xNormal );
}

void GLToy_Render::SubmitColour( const GLToy_Vector_3& xColour )
{
    Platform_GLToy_Render::SubmitColour( xColour );
}

void GLToy_Render::SubmitTextureCoordinate( const GLToy_Vector_3& xTexCoord, const u_int uTextureUnit )
{
    Platform_GLToy_Render::SubmitTextureCoordinate( xTexCoord, uTextureUnit );
}

void GLToy_Render::Flush()
{
    Platform_GLToy_Render::Flush();
}

u_int GLToy_Render::CreateShader( u_int uType )
{
    return Platform_GLToy_Render::CreateShader( uType );
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
