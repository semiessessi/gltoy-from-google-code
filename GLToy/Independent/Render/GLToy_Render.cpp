/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009-2011 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
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
bool GLToy_Render::s_bDrawTimers = GLToy_IsDebugBuild();
bool GLToy_Render::s_bVsync = true;
bool GLToy_Render::s_bClearFrame = true;
u_int GLToy_Render::s_uDepthBuffer = 0xFFFFFFFF;
u_int GLToy_Render::s_uFrameBuffer = 0xFFFFFFFF;
u_int GLToy_Render::s_uFrameTexture = 0xFFFFFFFF;
u_int GLToy_Render::s_uSwapBuffer = 0xFFFFFFFF;
u_int GLToy_Render::s_uSwapTexture = 0xFFFFFFFF;
u_int& GLToy_Render::s_uCurrentBuffer = GLToy_Render::s_uSwapBuffer;
u_int& GLToy_Render::s_uCurrentTexture = GLToy_Render::s_uSwapTexture;
GLToy_BinaryTree< const GLToy_Renderable*, float > GLToy_Render::s_xTransparents;

static bool g_bDebugClear = false;

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
    GLToy_InitialiserCall( GLToy_Texture_System );
    GLToy_InitialiserCall( GLToy_Shader_System );
    // SE - 13/10/2010 - a material system would sit here, so we can materialise fonts why not... :)
    // but actually, just commenting because my original rationale for the texture system initialising after shaders
    // was to allow shader based materials...
#ifndef GLTOY_DEMO
    GLToy_InitialiserCall( GLToy_Font_System );
#endif

    if( !Project_Initialise() )
    {
        return false;
    }

	GLToy_Console::RegisterVariable( "debugclear", &g_bDebugClear );
    GLToy_Console::RegisterVariable( "showfps", &s_bDrawFPS );
    GLToy_Console::RegisterVariable( "showtimers", &s_bDrawTimers );
    GLToy_Console::RegisterCommand( "vsync", SetVsyncEnabled );

    SetVsyncEnabled( s_bVsync );

    if( Platform_GLToy_Render::SupportFramebuffer() )
    {
        // set up framebuffer
        GenFramebuffers( 1, &s_uFrameBuffer );
        BindFramebuffer( FRAMEBUFFER, s_uFrameBuffer );
        GenRenderbuffers( 1, &s_uDepthBuffer );
        BindRenderbuffer( RENDERBUFFER, s_uDepthBuffer );
        RenderbufferStorage( RENDERBUFFER, DEPTH_COMPONENT24, GLToy::GetWindowViewportWidth(), GLToy::GetWindowViewportHeight() );
        FramebufferRenderbuffer( FRAMEBUFFER, DEPTH_ATTACHMENT, RENDERBUFFER, s_uDepthBuffer );
        GLToy_Texture_System::CreateFrameBufferTexture( s_uFrameTexture, GLToy::GetWindowViewportWidth(), GLToy::GetWindowViewportHeight() );
        FramebufferTexture2D( FRAMEBUFFER, COLOR_ATTACHMENT0, TEXTURE_2D, s_uFrameTexture, 0 );

        if( CheckFramebufferStatus( FRAMEBUFFER ) != FRAMEBUFFER_COMPLETE )
        {
            if( s_uFrameBuffer != 0xFFFFFFFF )
            {
                DeleteFramebuffers( 1, &s_uFrameBuffer );
                s_uFrameBuffer = 0xFFFFFFFF;
            }

            if( s_uDepthBuffer != 0xFFFFFFFF )
            {
                DeleteRenderbuffers( 1, &s_uDepthBuffer );
                s_uDepthBuffer = 0xFFFFFFFF;
            }

            if( s_uFrameTexture != 0xFFFFFFFF )
            {
                GLToy_Texture_System::DestroyFrameBufferTexture( s_uFrameTexture );
                s_uFrameTexture = 0xFFFFFFFF;
            }
        }
        else
        {
            // set up framebuffer
            GenFramebuffers( 1, &s_uSwapBuffer );
            BindFramebuffer( FRAMEBUFFER, s_uSwapBuffer );
            RenderbufferStorage( RENDERBUFFER, DEPTH_COMPONENT24, GLToy::GetWindowViewportWidth(), GLToy::GetWindowViewportHeight() );
            FramebufferRenderbuffer( FRAMEBUFFER, DEPTH_ATTACHMENT, RENDERBUFFER, s_uDepthBuffer );
            GLToy_Texture_System::CreateFrameBufferTexture( s_uSwapTexture, GLToy::GetWindowViewportWidth(), GLToy::GetWindowViewportHeight() );
            FramebufferTexture2D( FRAMEBUFFER, COLOR_ATTACHMENT0, TEXTURE_2D, s_uSwapTexture, 0 );

            if( CheckFramebufferStatus( FRAMEBUFFER ) != FRAMEBUFFER_COMPLETE )
            {
                if( s_uSwapBuffer != 0xFFFFFFFF )
                {
                    DeleteFramebuffers( 1, &s_uSwapBuffer );
                    s_uSwapBuffer = 0xFFFFFFFF;
                }

                if( s_uSwapTexture != 0xFFFFFFFF )
                {
                    GLToy_Texture_System::DestroyFrameBufferTexture( s_uSwapTexture );
                    s_uSwapTexture = 0xFFFFFFFF;
                }
            }
        }

        BindFramebuffer( FRAMEBUFFER, 0 );
    }

    if( HasFrameBuffer() )
    {
        GLToy_DebugOutput( "Framebuffer created successfully." );
    }
    else
    {
        GLToy_DebugOutput( "Framebuffer not created!" );
    }

    return true;
}

void GLToy_Render::Shutdown()
{
    if( s_uFrameBuffer != 0xFFFFFFFF )
    {
        DeleteFramebuffers( 1, &s_uFrameBuffer );
        s_uFrameBuffer = 0xFFFFFFFF;
    }

    if( s_uDepthBuffer != 0xFFFFFFFF )
    {
        DeleteRenderbuffers( 1, &s_uDepthBuffer );
        s_uDepthBuffer = 0xFFFFFFFF;
    }

    if( s_uFrameTexture != 0xFFFFFFFF )
    {
        GLToy_Texture_System::DestroyFrameBufferTexture( s_uFrameTexture );
        s_uFrameTexture = 0xFFFFFFFF;
    }

    if( s_uSwapBuffer != 0xFFFFFFFF )
    {
        DeleteFramebuffers( 1, &s_uSwapBuffer );
        s_uSwapBuffer = 0xFFFFFFFF;
    }

    if( s_uSwapTexture != 0xFFFFFFFF )
    {
        GLToy_Texture_System::DestroyFrameBufferTexture( s_uSwapTexture );
        s_uSwapTexture = 0xFFFFFFFF;
    }
    
    Project_Shutdown();
#ifndef GLTOY_DEMO
    GLToy_Font_System::Shutdown();
#endif
    GLToy_Texture_System::Shutdown();
    GLToy_Shader_System::Shutdown();

    Platform_Shutdown();
}

void GLToy_Render::BeginRender()
{
    Platform_GLToy_Render::Clear();

    if( HasFrameBuffer() )
    {
        BindFramebuffer( FRAMEBUFFER, s_uCurrentBuffer );
        SetViewport( 0, 0, GLToy::GetWindowViewportWidth(), GLToy::GetWindowViewportHeight() );
        // u_int uBuffer = COLOR_ATTACHMENT0;
        // GLToy_Render::DrawBuffers( 1, &uBuffer );
    }

    EnableDepthTesting();
    EnableDepthWrites();
    SetDepthFunction( DEPTH_LEQUAL );

    Platform_BeginRender();

	if( g_bDebugClear )
	{
		Platform_GLToy_Render::ClearColour( GLToy_Vector_4( 1.0f, 0.0f, 1.0f, 1.0f ) );
	}

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
    GLToy_IndirectRenderTransparentFunctor< const GLToy_Renderable > xFunctor;
    s_xTransparents.Traverse( xFunctor );

    // clean up for next frame
    s_xTransparents.Clear();

    if( HasFrameBuffer() )
    {
        BindFramebuffer( FRAMEBUFFER, 0 );

        SetOrthogonalProjectionMatrix();
        PushViewMatrix();
        SetIdentityViewMatrix();
        DisableDepthTesting();
        DisableBlending();

        GLToy_Texture_System::BindFrameBufferTexture( s_uCurrentTexture );
        StartSubmittingQuads();
        SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );
        SubmitTexturedQuad2D( GLToy_Vector_2( -0.5f * GLToy_Render::Get2DWidth(), -1.0f ), GLToy_Vector_2( GLToy_Render::Get2DWidth(), 2.0f ), 0.0f, 1.0f, 1.0f, 0.0f );
        EndSubmit();

        SetPerspectiveProjectionMatrix();
        PopViewMatrix();
    }
}

void GLToy_Render::Render2D()
{
    // Project_Render2D();

#ifndef GLTOY_DEM0
	if( s_bDrawFPS || s_bDrawTimers )
    {
        UseProgram( 0 );
        // draw fps counter, we should have the console's font by now
        GLToy_Font* pxFont = GLToy_Font_System::FindFont( GLToy_Hash_Constant( "console" ) );
        if( pxFont )
        {
            GLToy_String szString;
            if( s_bDrawFPS )
            {
                szString.SetToFormatString( "%.2f fps", GLToy_Maths::Max( GLToy_Timer::GetSmoothedFrameRate(), 0.01f ) );
                pxFont->RenderString( szString, GLToy_Render::GetMaxX() - szString.GetLength() * pxFont->GetWidth(), 1.0f - pxFont->GetHeight() );
            }

            if( s_bDrawTimers )
            {
                szString.SetToFormatString( "Update: %.3fms", GLToy::GetUpdateProfileTimer() * 1000.0f );
                pxFont->RenderString( szString, GLToy_Render::GetMaxX() - szString.GetLength() * pxFont->GetWidth(), 0.85f - pxFont->GetHeight() );
                szString.SetToFormatString( "Render: %.3fms", GLToy::GetRenderProfileTimer() * 1000.0f );
                pxFont->RenderString( szString, GLToy_Render::GetMaxX() - szString.GetLength() * pxFont->GetWidth(), 0.8f - pxFont->GetHeight() );
                if( !IsIntelGraphicsCard() )
                {
                    szString.SetToFormatString( "GPU: %.3fms", GLToy::GetGPUProfileTimer() * 1000.0f );
                    pxFont->RenderString( szString, GLToy_Render::GetMaxX() - szString.GetLength() * pxFont->GetWidth(), 0.75f - pxFont->GetHeight() );
                }
                szString.SetToFormatString( "Sync: %.3fms", GLToy::GetSyncProfileTimer() * 1000.0f );
                pxFont->RenderString( szString, GLToy_Render::GetMaxX() - szString.GetLength() * pxFont->GetWidth(), 0.7f - pxFont->GetHeight() );
            }
        }
    }
#endif

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

void GLToy_Render::BindFrameBuffer( const u_int uTextureUnit )
{
    if( HasFrameBuffer() )
    {
        GLToy_Render::BindFrameBufferNoCopy( uTextureUnit );
        StartSubmittingQuads();
        SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );
        SubmitTexturedQuad2D( GLToy_Vector_2( -0.5f * GLToy_Render::Get2DWidth(), -1.0f ), GLToy_Vector_2( GLToy_Render::Get2DWidth(), 2.0f ), 0.0f, 1.0f, 1.0f, 0.0f );
        EndSubmit();
    }
}

void GLToy_Render::BindFrameBufferNoCopy( const u_int uTextureUnit )
{
    if( HasFrameBuffer() )
    {
        BindFramebuffer( FRAMEBUFFER, 0 );
        
        GLToy_Texture_System::BindFrameBufferTexture( s_uCurrentTexture );

        if( &s_uCurrentBuffer == &s_uFrameBuffer )
        {
            s_uCurrentBuffer = s_uSwapBuffer;
            s_uCurrentTexture = s_uSwapTexture;
        }
        else
        {
            s_uCurrentBuffer = s_uFrameBuffer;
            s_uCurrentTexture = s_uFrameTexture;
        }

        BindFramebuffer( FRAMEBUFFER, s_uCurrentBuffer );
    }
}

void GLToy_Render::BindLastFrameBufferTexture( const u_int uTextureUnit )
{
    if( HasFrameBuffer() )
    {        
        GLToy_Texture_System::BindFrameBufferTexture( s_uCurrentTexture );

        if( &s_uCurrentBuffer == &s_uFrameBuffer )
        {
            GLToy_Texture_System::BindFrameBufferTexture( s_uSwapTexture );
        }
        else
        {
            GLToy_Texture_System::BindFrameBufferTexture( s_uFrameTexture );
        }
    }
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

void GLToy_Render::Clear()
{
    Platform_GLToy_Render::Clear();
}

void GLToy_Render::ClearColour( const GLToy_Vector_4& xColour )
{
    Platform_GLToy_Render::ClearColour( xColour );
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

void GLToy_Render::PushViewAttributes()
{
    Platform_GLToy_Render::PushViewAttributes();
}

void GLToy_Render::PopViewAttributes()
{
    Platform_GLToy_Render::PopViewAttributes();
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

void GLToy_Render::SubmitVertex( const GLToy_Vector_2& xVertex )
{
    Platform_GLToy_Render::SubmitVertex( xVertex );
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
    SetBlendFunction( BLEND_ONE, BLEND_ZERO );
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

void GLToy_Render::SetVsyncEnabled( const bool bEnabled )
{
    s_bVsync = bEnabled;
    // pretend this is a console variable
    GLToy_Console::Print( GLToy_String( "vsync is set to " ) + ( bEnabled ? "true" : "false" ) );
    Platform_GLToy_Render::SetVsyncEnabled( bEnabled );
}

void GLToy_Render::DrawBuffers( const int iCount, const u_int* const puBuffers )
{
    Platform_GLToy_Render::DrawBuffers( iCount, puBuffers );
}

bool GLToy_Render::IsRenderbuffer( const u_int uRenderBuffer )
{
    return Platform_GLToy_Render::IsRenderbuffer( uRenderBuffer );
}

void GLToy_Render::BindRenderbuffer( const u_int uTarget, const u_int uRenderBuffer )
{
    Platform_GLToy_Render::BindRenderbuffer( uTarget, uRenderBuffer );
}

void GLToy_Render::DeleteRenderbuffers( const int iCount, u_int* const puRenderBuffers )
{
    Platform_GLToy_Render::DeleteFramebuffers( iCount, puRenderBuffers );
}

void GLToy_Render::GenRenderbuffers( const int iCount, u_int* const puRenderBuffers )
{
    Platform_GLToy_Render::GenRenderbuffers( iCount, puRenderBuffers );
}

void GLToy_Render::RenderbufferStorage( const u_int uTarget, const u_int uInternalFormat, const int iWidth, const int iHeight )
{
    Platform_GLToy_Render::RenderbufferStorage( uTarget, uInternalFormat, iWidth, iHeight );
}

void GLToy_Render::GetRenderbufferParameter( const u_int uTarget, const u_int uPName, int* const piParams )
{
    Platform_GLToy_Render::GetRenderbufferParameter( uTarget, uPName, piParams );
}

bool GLToy_Render::IsFramebuffer( const u_int uFrameBuffer )
{
    return Platform_GLToy_Render::IsFramebuffer( uFrameBuffer );
}

void GLToy_Render::BindFramebuffer( const u_int uTarget, const u_int uFrameBuffer )
{
    Platform_GLToy_Render::BindFramebuffer( uTarget, uFrameBuffer );
}

void GLToy_Render::DeleteFramebuffers( const int iCount, u_int* const puFrameBuffers )
{
    Platform_GLToy_Render::DeleteFramebuffers( iCount, puFrameBuffers );
}

void GLToy_Render::GenFramebuffers( const int iCount, u_int* const puFrameBuffers )
{
    Platform_GLToy_Render::GenFramebuffers( iCount, puFrameBuffers );
}

u_int GLToy_Render::CheckFramebufferStatus( const u_int uTarget )
{
    return Platform_GLToy_Render::CheckFramebufferStatus( uTarget );
}

void GLToy_Render::FramebufferTexture1D( const u_int uTarget, const u_int uAttachment, const u_int uTexTarget, const u_int uTexture, const int iLevel )
{
    Platform_GLToy_Render::FramebufferTexture1D( uTarget, uAttachment, uTexTarget, uTexture, iLevel );
}

void GLToy_Render::FramebufferTexture2D( const u_int uTarget, const u_int uAttachment, const u_int uTexTarget, const u_int uTexture, const int iLevel )
{
    Platform_GLToy_Render::FramebufferTexture2D( uTarget, uAttachment, uTexTarget, uTexture, iLevel );
}

void GLToy_Render::FramebufferTexture3D( const u_int uTarget, const u_int uAttachment, const u_int uTexTarget, const u_int uTexture, const int iLevel, const int iZOffset )
{
    Platform_GLToy_Render::FramebufferTexture3D( uTarget, uAttachment, uTexTarget, uTexture, iLevel, iZOffset );
}

void GLToy_Render::FramebufferRenderbuffer( const u_int uTarget, const u_int uAttachment, const u_int uRenderBufferTarget, const u_int uRenderBuffer )
{
    Platform_GLToy_Render::FramebufferRenderbuffer( uTarget, uAttachment, uRenderBufferTarget, uRenderBuffer );
}

void GetFramebufferAttachmentParameter( const u_int uTarget, const u_int uAttachment, const u_int uPName, int* const piParams )
{
    Platform_GLToy_Render::GetFramebufferAttachmentParameter( uTarget, uAttachment, uPName, piParams );
}

void GLToy_Render::GenerateMipmap( const u_int uTarget )
{
    Platform_GLToy_Render::GenerateMipmap( uTarget );
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

void GLToy_Render::BeginQuery( u_int uTarget, u_int uID )
{
    Platform_GLToy_Render::BeginQuery( uTarget, uID );
}

void GLToy_Render::EndQuery( u_int uTarget )
{
    Platform_GLToy_Render::EndQuery( uTarget );
}

void GLToy_Render::GenerateQueries( u_int uCount, u_int* puIDs )
{
    Platform_GLToy_Render::GenerateQueries( uCount, puIDs );
}

void GLToy_Render::DeleteQueries( u_int uCount, const u_int* puIDs )
{
    Platform_GLToy_Render::DeleteQueries( uCount, puIDs );
}

void GLToy_Render::GetQueryObject( u_int uID, u_int uParameterName, int* piParameters )
{
    Platform_GLToy_Render::GetQueryObject( uID, uParameterName, piParameters );
}

void GLToy_Render::GetQueryObject( u_int uID, u_int uParameterName, unsigned long long* pullParameters )
{
    Platform_GLToy_Render::GetQueryObject( uID, uParameterName, pullParameters );
}

bool GLToy_Render::IsIntelGraphicsCard()
{
    return Platform_GLToy_Render::IsIntelGraphicsCard();
}
