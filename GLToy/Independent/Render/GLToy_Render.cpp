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

// GLToy
#include <Core/GLToy_Timer.h>
#include <Material/GLToy_Material_System.h>
#include <Maths/GLToy_Maths.h>
#include <Maths/GLToy_Vector.h>
#include <Render/Font/GLToy_Font.h>
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_FSFX_System.h>
#include <Render/GLToy_Light_System.h>
#include <Render/GLToy_Renderable.h>
#include <Render/GLToy_RenderFunctor.h>
#include <Render/Shader/GLToy_Shader_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

float GLToy_Render::s_fFOV = 90.0f;
float GLToy_Render::s_fAspectRatio = 1.0f;
bool GLToy_Render::s_bDrawFPS = GLToy_IsDebugBuild();
bool GLToy_Render::s_bDrawTimers = GLToy_IsDebugBuild();
bool GLToy_Render::s_bDrawCounters = GLToy_IsDebugBuild();
bool GLToy_Render::s_bDrawBuffers = false;
bool GLToy_Render::s_bDrawNormals = false;
bool GLToy_Render::s_bVsync = true;
bool GLToy_Render::s_bClearFrame = true;
//u_int GLToy_Render::s_uDepthBuffer = GLToy_MaxUint;
u_int GLToy_Render::s_uDepthTexture = GLToy_MaxUint;
u_int GLToy_Render::s_uFrameBuffer = GLToy_MaxUint;
u_int GLToy_Render::s_uFrameTexture = GLToy_MaxUint;
u_int GLToy_Render::s_uSwapBuffer = GLToy_MaxUint;
u_int GLToy_Render::s_uSwapTexture = GLToy_MaxUint;
u_int* GLToy_Render::s_puCurrentBuffer = &GLToy_Render::s_uSwapBuffer;
u_int* GLToy_Render::s_puCurrentTexture = &GLToy_Render::s_uSwapTexture;
u_int GLToy_Render::s_uDeferredBuffer = GLToy_MaxUint;
u_int GLToy_Render::s_uDiffuseTexture = GLToy_MaxUint;
u_int GLToy_Render::s_uNormalTexture = GLToy_MaxUint;
u_int GLToy_Render::s_uSpecularTexture = GLToy_MaxUint;
u_int GLToy_Render::s_uCurrentDebugBuffer = 0;
u_int GLToy_Render::s_uFrameBufferNoDepth = GLToy_MaxUint;
u_int GLToy_Render::s_uSwapBufferNoDepth = GLToy_MaxUint;
u_int* GLToy_Render::s_puCurrentBufferNoDepth = &GLToy_Render::s_uSwapBufferNoDepth;

u_int GLToy_Render_Metrics::s_uTriangleCount = 0;
u_int GLToy_Render_Metrics::s_uLightCount = 0;
u_int GLToy_Render_Metrics::s_uMaterialSwitchCount = 0;
u_int GLToy_Render_Metrics::s_uTrianglePeak = 0;
float GLToy_Render_Metrics::s_fTriangleTime = 0.0f;

GLToy_Vector_2 GLToy_Render::s_xClipPlanes = GLToy_Maths::ZeroVector2;

GLToy_BinaryTree< const GLToy_Renderable*, float > GLToy_Render::s_xTransparents;
GLToy_List< const GLToy_Renderable* > GLToy_Render::s_xDeferredRenderables;

#ifdef GLTOY_DEBUG
GLToy_Render::DebugStateType GLToy_Render::DebugState;
#endif

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
    GLToy_InitialiserCall( GLToy_Light_System );
    GLToy_InitialiserCall( GLToy_Material_System );
#ifndef GLTOY_DEMO
    GLToy_InitialiserCall( GLToy_Font_System );
#endif
    GLToy_InitialiserCall( GLToy_FSFX_System );

    if( !Project_Initialise() )
    {
        return false;
    }

	GLToy_Console::RegisterVariable( "debugclear", &g_bDebugClear );
    GLToy_Console::RegisterVariable( "showfps", &s_bDrawFPS );
    GLToy_Console::RegisterVariable( "showtimers", &s_bDrawTimers );
    GLToy_Console::RegisterVariable( "showcounters", &s_bDrawCounters );
    GLToy_Console::RegisterVariable( "showbuffers", &s_bDrawBuffers );
    GLToy_Console::RegisterVariable( "buffer", &s_uCurrentDebugBuffer );
    GLToy_Console::RegisterVariable( "shownormals", &s_bDrawNormals );
    GLToy_Console::RegisterVariable( "render.fov", &s_fFOV );
    GLToy_Console::RegisterCommand( "vsync", SetVsyncEnabled );

    SetVsyncEnabled( s_bVsync );

    if( Platform_GLToy_Render::SupportFramebuffer() )
    {
        // set up framebuffer
        GenFramebuffers( 1, &s_uFrameBuffer );
        BindFramebuffer( FRAMEBUFFER, s_uFrameBuffer );
        //GenRenderbuffers( 1, &s_uDepthBuffer );
        //BindRenderbuffer( RENDERBUFFER, s_uDepthBuffer );
        //RenderbufferStorage( RENDERBUFFER, DEPTH_COMPONENT24, GLToy::GetWindowViewportWidth(), GLToy::GetWindowViewportHeight() );
        //FramebufferRenderbuffer( FRAMEBUFFER, DEPTH_ATTACHMENT, RENDERBUFFER, s_uDepthBuffer );
        GLToy_Texture_System::CreateFrameBufferTexture( s_uFrameTexture, GLToy::GetWindowViewportWidth(), GLToy::GetWindowViewportHeight() );
        GLToy_Texture_System::CreateDepthBufferTexture( s_uDepthTexture, GLToy::GetWindowViewportWidth(), GLToy::GetWindowViewportHeight() );
        FramebufferTexture2D( FRAMEBUFFER, COLOR_ATTACHMENT0, TEXTURE_2D, s_uFrameTexture, 0 );
        FramebufferTexture2D( FRAMEBUFFER, DEPTH_ATTACHMENT, TEXTURE_2D, s_uDepthTexture, 0 );

        if( CheckFramebufferStatus( FRAMEBUFFER ) != FRAMEBUFFER_COMPLETE )
        {
            if( s_uFrameBuffer != GLToy_MaxUint )
            {
                DeleteFramebuffers( 1, &s_uFrameBuffer );
                s_uFrameBuffer = GLToy_MaxUint;
            }

            //if( s_uDepthBuffer != GLToy_MaxUint )
            //{
            //    DeleteRenderbuffers( 1, &s_uDepthBuffer );
            //    s_uDepthBuffer = GLToy_MaxUint;
            //}

            if( s_uDepthTexture != GLToy_MaxUint )
            {
                GLToy_Texture_System::DestroyFrameBufferTexture( s_uDepthTexture );
                s_uDepthTexture = GLToy_MaxUint;
            }

            if( s_uFrameTexture != GLToy_MaxUint )
            {
                GLToy_Texture_System::DestroyFrameBufferTexture( s_uFrameTexture );
                s_uFrameTexture = GLToy_MaxUint;
            }
        }
        else
        {
            // set up swap buffer
            GenFramebuffers( 1, &s_uSwapBuffer );
            BindFramebuffer( FRAMEBUFFER, s_uSwapBuffer );
            GLToy_Texture_System::CreateFrameBufferTexture( s_uSwapTexture, GLToy::GetWindowViewportWidth(), GLToy::GetWindowViewportHeight() );
            FramebufferTexture2D( FRAMEBUFFER, COLOR_ATTACHMENT0, TEXTURE_2D, s_uSwapTexture, 0 );
            FramebufferTexture2D( FRAMEBUFFER, DEPTH_ATTACHMENT, TEXTURE_2D, s_uDepthTexture, 0 );

            if( CheckFramebufferStatus( FRAMEBUFFER ) != FRAMEBUFFER_COMPLETE )
            {
                if( s_uSwapBuffer != GLToy_MaxUint )
                {
                    DeleteFramebuffers( 1, &s_uSwapBuffer );
                    s_uSwapBuffer = GLToy_MaxUint;
                }

                if( s_uSwapTexture != GLToy_MaxUint )
                {
                    GLToy_Texture_System::DestroyFrameBufferTexture( s_uSwapTexture );
                    s_uSwapTexture = GLToy_MaxUint;
                }
            }
        }

        // set up framebuffer with no depth
        GenFramebuffers( 1, &s_uFrameBufferNoDepth );
        BindFramebuffer( FRAMEBUFFER, s_uFrameBufferNoDepth );
        FramebufferTexture2D( FRAMEBUFFER, COLOR_ATTACHMENT0, TEXTURE_2D, s_uFrameTexture, 0 );

        if( CheckFramebufferStatus( FRAMEBUFFER ) != FRAMEBUFFER_COMPLETE )
        {
            if( s_uFrameBufferNoDepth != GLToy_MaxUint )
            {
                DeleteFramebuffers( 1, &s_uFrameBufferNoDepth );
                s_uFrameBufferNoDepth = GLToy_MaxUint;
            }
        }
        else
        {
            // set up swap buffer with no depth
            GenFramebuffers( 1, &s_uSwapBufferNoDepth );
            BindFramebuffer( FRAMEBUFFER, s_uSwapBufferNoDepth );
            //RenderbufferStorage( RENDERBUFFER, DEPTH_COMPONENT24, GLToy::GetWindowViewportWidth(), GLToy::GetWindowViewportHeight() );
            //FramebufferRenderbuffer( FRAMEBUFFER, DEPTH_ATTACHMENT, RENDERBUFFER, s_uDepthBuffer );
            FramebufferTexture2D( FRAMEBUFFER, COLOR_ATTACHMENT0, TEXTURE_2D, s_uSwapTexture, 0 );

            if( CheckFramebufferStatus( FRAMEBUFFER ) != FRAMEBUFFER_COMPLETE )
            {
                if( s_uSwapBufferNoDepth != GLToy_MaxUint )
                {
                    DeleteFramebuffers( 1, &s_uSwapBufferNoDepth );
                    s_uSwapBufferNoDepth = GLToy_MaxUint;
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

    // initialise deferred buffers
    if( Platform_GLToy_Render::SupportFramebuffer() )
    {
        // set up framebuffer
        GenFramebuffers( 1, &s_uDeferredBuffer );
        BindFramebuffer( FRAMEBUFFER, s_uDeferredBuffer );
        //BindRenderbuffer( RENDERBUFFER, s_uDepthBuffer );
        //FramebufferRenderbuffer( FRAMEBUFFER, DEPTH_ATTACHMENT, RENDERBUFFER, s_uDepthBuffer );
        GLToy_Texture_System::CreateFrameBufferTexture( s_uDiffuseTexture, GLToy::GetWindowViewportWidth(), GLToy::GetWindowViewportHeight() );
        GLToy_Texture_System::CreateFrameBufferTexture( s_uNormalTexture, GLToy::GetWindowViewportWidth(), GLToy::GetWindowViewportHeight() );
        GLToy_Texture_System::CreateFrameBufferTexture( s_uSpecularTexture, GLToy::GetWindowViewportWidth(), GLToy::GetWindowViewportHeight() );
        FramebufferTexture2D( FRAMEBUFFER, COLOR_ATTACHMENT0, TEXTURE_2D, s_uDiffuseTexture, 0 );
        FramebufferTexture2D( FRAMEBUFFER, COLOR_ATTACHMENT1, TEXTURE_2D, s_uNormalTexture, 0 );
        FramebufferTexture2D( FRAMEBUFFER, COLOR_ATTACHMENT2, TEXTURE_2D, s_uSpecularTexture, 0 );
        FramebufferTexture2D( FRAMEBUFFER, DEPTH_ATTACHMENT, TEXTURE_2D, s_uDepthTexture, 0 );

        if( CheckFramebufferStatus( FRAMEBUFFER ) != FRAMEBUFFER_COMPLETE )
        {
            if( s_uDeferredBuffer != GLToy_MaxUint )
            {
                DeleteFramebuffers( 1, &s_uDeferredBuffer );
                s_uDeferredBuffer = GLToy_MaxUint;
            }

            if( s_uDiffuseTexture != GLToy_MaxUint )
            {
                GLToy_Texture_System::DestroyFrameBufferTexture( s_uDiffuseTexture );
                s_uDiffuseTexture = GLToy_MaxUint;
            }

            if( s_uNormalTexture != GLToy_MaxUint )
            {
                GLToy_Texture_System::DestroyFrameBufferTexture( s_uNormalTexture );
                s_uNormalTexture = GLToy_MaxUint;
            }
        }

        BindFramebuffer( FRAMEBUFFER, 0 );
    }

    if( HasDeferredBuffer() )
    {
        GLToy_DebugOutput( "Deferred buffer created successfully." );
    }
    else
    {
        GLToy_DebugOutput( "Deferred buffer not created!" );
    }

    return true;
}

void GLToy_Render::Shutdown()
{
    if( s_uFrameBuffer != GLToy_MaxUint )
    {
        DeleteFramebuffers( 1, &s_uFrameBuffer );
        s_uFrameBuffer = GLToy_MaxUint;
    }

    //if( s_uDepthBuffer != GLToy_MaxUint )
    //{
    //    DeleteRenderbuffers( 1, &s_uDepthBuffer );
    //    s_uDepthBuffer = GLToy_MaxUint;
    //}

    if( s_uDepthTexture != GLToy_MaxUint )
    {
        GLToy_Texture_System::DestroyFrameBufferTexture( s_uDepthTexture );
        s_uDepthTexture = GLToy_MaxUint;
    }

    if( s_uFrameTexture != GLToy_MaxUint )
    {
        GLToy_Texture_System::DestroyFrameBufferTexture( s_uFrameTexture );
        s_uFrameTexture = GLToy_MaxUint;
    }

    if( s_uSwapBuffer != GLToy_MaxUint )
    {
        DeleteFramebuffers( 1, &s_uSwapBuffer );
        s_uSwapBuffer = GLToy_MaxUint;
    }

    if( s_uDeferredBuffer != GLToy_MaxUint )
    {
        DeleteFramebuffers( 1, &s_uDeferredBuffer );
        s_uDeferredBuffer = GLToy_MaxUint;
    }

    if( s_uSwapTexture != GLToy_MaxUint )
    {
        GLToy_Texture_System::DestroyFrameBufferTexture( s_uSwapTexture );
        s_uSwapTexture = GLToy_MaxUint;
    }

    if( s_uDiffuseTexture != GLToy_MaxUint )
    {
        GLToy_Texture_System::DestroyFrameBufferTexture( s_uDiffuseTexture );
        s_uDiffuseTexture = GLToy_MaxUint;
    }

    if( s_uNormalTexture != GLToy_MaxUint )
    {
        GLToy_Texture_System::DestroyFrameBufferTexture( s_uNormalTexture );
        s_uNormalTexture = GLToy_MaxUint;
    }

    if( s_uSpecularTexture != GLToy_MaxUint )
    {
        GLToy_Texture_System::DestroyFrameBufferTexture( s_uSpecularTexture );
        s_uSpecularTexture = GLToy_MaxUint;
    }

    if( s_uFrameBufferNoDepth != GLToy_MaxUint )
    {
        DeleteFramebuffers( 1, &s_uFrameBufferNoDepth );
        s_uFrameBufferNoDepth = GLToy_MaxUint;
    }

    if( s_uSwapBufferNoDepth != GLToy_MaxUint )
    {
        DeleteFramebuffers( 1, &s_uSwapBufferNoDepth );
        s_uSwapBufferNoDepth = GLToy_MaxUint;
    }
    
    Project_Shutdown();

    GLToy_FSFX_System::Shutdown();

#ifndef GLTOY_DEMO
    GLToy_Font_System::Shutdown();
#endif

    GLToy_Material_System::Shutdown();
    GLToy_Light_System::Shutdown();
    GLToy_Texture_System::Shutdown();
    GLToy_Shader_System::Shutdown();

    Platform_Shutdown();
}

void GLToy_Render::BeginRender()
{
    Platform_GLToy_Render::Clear();

    if( HasFrameBuffer() )
    {
        BindFramebuffer( FRAMEBUFFER, *s_puCurrentBuffer );
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

void GLToy_Render::TransparentPass()
{
    EnableBlending();
    SetBlendFunction( BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA );
    EnableDepthTesting();
    DisableDepthWrites();

    // the magic of binary trees draws everything depth sorted
    GLToy_IndirectRenderTransparentFunctor< const GLToy_Renderable > xFunctor;
    s_xTransparents.Traverse( xFunctor );

    // clean up for next frame
    s_xTransparents.Clear();
}

void GLToy_Render::DeferredPass()
{
    if( HasDeferredBuffer() )
    {
        BindFramebuffer( FRAMEBUFFER, s_uDeferredBuffer );
        SetViewport( 0, 0, GLToy::GetWindowViewportWidth(), GLToy::GetWindowViewportHeight() );

        SetBlendFunction( BLEND_ONE, BLEND_ZERO );
        DisableBlending();
                
        const u_int auBuffers[] =
        {
            COLOR_ATTACHMENT0,
            COLOR_ATTACHMENT1,
            COLOR_ATTACHMENT2,
        };

        GLToy_Render::DrawBuffers( sizeof( auBuffers ) / sizeof( u_int ), auBuffers );

        GLToy_Shader_System::BindShaderProgram( GLToy_Hash_Constant( "Deferred_Clear" ) );

        DisableDepthWrites();
        DisableDepthTesting();
        
        StartSubmittingQuads();
        SubmitTexturedQuad2D( GLToy_Vector_2( -0.5f * GLToy_Render::Get2DWidth(), -1.0f ), GLToy_Vector_2( GLToy_Render::Get2DWidth(), 2.0f ), 0.0f, 1.0f, 1.0f, 0.0f );
        EndSubmit();

        EnableDepthWrites();
        EnableDepthTesting();

        UseProgram( 0 );
        
        s_xDeferredRenderables.Traverse( GLToy_IndirectRenderDeferredFunctor< const GLToy_Renderable >() );

        UseProgram( 0 );
        GLToy_Render::DrawBuffers( 1, auBuffers );

        BindFramebuffer( FRAMEBUFFER, *s_puCurrentBuffer );

        // now accumulate lighting into the frame buffer from the deferred buffers...
        // TODO: use a seperate buffer so that it can be combined with the current framebuffer properly
        // and with bloom and HDR if there is a good enough card to perform well with it
        BindDiffuseTexture( 1 );
        BindNormalTexture( 2 );
        BindDepthTexture( 3 );

        EnableBlending();
        DisableDepthWrites();
        DisableDepthTesting();
        SetBlendFunction( BLEND_ONE, BLEND_ONE );

        GLToy_Light_System::Render();

        BindFramebuffer( FRAMEBUFFER, *s_puCurrentBuffer );
    }
    else
    {
        s_xDeferredRenderables.Traverse( GLToy_IndirectRenderFunctor< const GLToy_Renderable >() );

    }

    // clean up for next frame
    s_xDeferredRenderables.Clear();
}

void GLToy_Render::Render()
{
    Project_Render();

    // everything up to here should have been forward rendered into the framebuffer/depth buffer (TODO: z-prepass?)

    DeferredPass();

    TransparentPass();

    if( HasFrameBuffer() )
    {
        GLToy_FSFX_System::Render();

        BindFramebuffer( FRAMEBUFFER, 0 );

        SetOrthogonalProjectionMatrix();
        PushViewMatrix();
        SetIdentityViewMatrix();
        DisableDepthTesting();
        DisableBlending();

        GLToy_Texture_System::BindFrameBufferTexture( *s_puCurrentTexture );
        StartSubmittingQuads();
        SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );
        SubmitTexturedQuad2D( GLToy_Vector_2( -0.5f * GLToy_Render::Get2DWidth(), -1.0f ), GLToy_Vector_2( GLToy_Render::Get2DWidth(), 2.0f ), 0.0f, 1.0f, 1.0f, 0.0f );
        EndSubmit();

        if( s_bDrawNormals && HasDeferredBuffer() )
        {
            GLToy_Texture_System::BindFrameBufferTexture( s_uNormalTexture );
            StartSubmittingQuads();
            SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );
            SubmitTexturedQuad2D( GLToy_Vector_2( -0.5f * GLToy_Render::Get2DWidth(), -1.0f ), GLToy_Vector_2( GLToy_Render::Get2DWidth(), 2.0f ), 0.0f, 1.0f, 1.0f, 0.0f );
            EndSubmit();
        }

        if( s_bDrawBuffers && HasDeferredBuffer() )
        {
            switch( s_uCurrentDebugBuffer )
            {
                case 0: GLToy_Texture_System::BindFrameBufferTexture( *s_puCurrentTexture ); break;
                case 1:
                case 7: BindDiffuseTexture(); break;
                case 2:
                case 8: BindNormalTexture(); break;
                case 9:
                case 3: BindDepthTexture(); break;
                case 4:
                case 5:
                case 6: 
                case 10: BindSpecularTexture(); break;
                default: break;
            }

            GLToy_Hash uShaderHash = 0;
            switch( s_uCurrentDebugBuffer )
            {
                case 1: uShaderHash = GLToy_Hash_Constant( "Debug_Diffuse" ); break;
                case 2: uShaderHash = GLToy_Hash_Constant( "Debug_Normals" ); break;
                case 3: uShaderHash = GLToy_Hash_Constant( "Debug_Depth" ); break;
                case 4: uShaderHash = GLToy_Hash_Constant( "Debug_SpecularMask" ); break;
                case 5: uShaderHash = GLToy_Hash_Constant( "Debug_SpecularPower" ); break;
                case 6: uShaderHash = GLToy_Hash_Constant( "Debug_SpecularFresnel" ); break;
                case 7: uShaderHash = GLToy_Hash_Constant( "Debug_Fog" ); break;
                case 8: uShaderHash = GLToy_Hash_Constant( "Debug_Height" ); break;
                case 9: uShaderHash = GLToy_Hash_Constant( "Debug_AO" ); break;
                case 10: uShaderHash = GLToy_Hash_Constant( "Debug_SubsurfaceDepth" ); break;

                default: break;
            }

            GLToy_ShaderProgram* pxShader = GLToy_Shader_System::FindShader( uShaderHash );
            if( pxShader )
            {
                pxShader->Bind();
                pxShader->SetUniform( "xSampler", 0 );
            }
            
            StartSubmittingQuads();
            SubmitTexturedQuad2D( GLToy_Vector_2( -0.5f * ( ( s_uCurrentDebugBuffer == 0 ) ? GLToy_Render::Get2DWidth() : 2.0f ), -1.0f ), GLToy_Vector_2( 0.25f * ( ( s_uCurrentDebugBuffer == 0 ) ? GLToy_Render::Get2DWidth() : 2.0f ), 0.5f ), 0.0f, 1.0f, 1.0f, 0.0f );
            EndSubmit();

            UseProgram( 0 );
        }

        SetPerspectiveProjectionMatrix();
        PopViewMatrix();
    }
}

void GLToy_Render::Render2D()
{
    // Project_Render2D();

#ifndef GLTOY_DEM0
	if( s_bDrawFPS || s_bDrawTimers || s_bDrawCounters || ( s_bDrawBuffers && HasDeferredBuffer() ) )
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

            if( s_bDrawCounters )
            {
                const GLToy_Vector_4 xColourGood( 1.0f, 1.0f, 1.0f, 1.0f );
                const GLToy_Vector_4 xColourBad( 1.0f, 0.0f, 0.0f, 1.0f );
                szString.SetToFormatString( "Triangles: %d (%d) / %d", GLToy_Render_Metrics::GetTriangleCount(), GLToy_Render_Metrics::GetTrianglePeak(), GLToy_Render_Metrics::uGLTOY_IDEAL_TRIANGLE_LIMIT );
                pxFont->RenderString( szString, GLToy_Render::GetMaxX() - szString.GetLength() * pxFont->GetWidth(), 0.65f - pxFont->GetHeight(),
                    ( GLToy_Render_Metrics::GetTrianglePeak() <= GLToy_Render_Metrics::uGLTOY_IDEAL_TRIANGLE_LIMIT ) ? xColourGood : xColourBad );
                szString.SetToFormatString( "Lights: %d / %d", GLToy_Render_Metrics::GetLightCount(), GLToy_Render_Metrics::uGLTOY_IDEAL_LIGHT_LIMIT );
                pxFont->RenderString( szString, GLToy_Render::GetMaxX() - szString.GetLength() * pxFont->GetWidth(), 0.6f - pxFont->GetHeight(),
                    ( GLToy_Render_Metrics::GetTriangleCount() <= GLToy_Render_Metrics::uGLTOY_IDEAL_TRIANGLE_LIMIT ) ? xColourGood : xColourBad );

			
            }

            if( s_bDrawBuffers && HasDeferredBuffer() )
            {
                // TODO: something to scroll through different buffers/channels...
                switch( s_uCurrentDebugBuffer )
                {
                    case 0: szString = "Frame buffer"; break;
                    case 1: szString = "Diffuse"; break;
                    case 2: szString = "Normals"; break;
                    case 3: szString = "Depth buffer"; break;
                    case 4: szString = "Specular mask"; break;
                    case 5: szString = "Specular power"; break;
                    case 6: szString = "Fresnel term"; break;
                    case 7: szString = "Fog"; break;
                    case 8: szString = "Height"; break;
                    case 9: szString = "Ambient occlusion"; break;
                    case 10: szString = "Subsurface depth"; break;
                }
                    
                pxFont->RenderString( szString, GLToy_Render::GetMinX(), -0.5f - pxFont->GetHeight() );
            }
        }
    }
#endif

}


void GLToy_Render::EndRender()
{
    Flush();

    Platform_EndRender();

    GLToy_Render_Metrics::ResetCounters();
}

void GLToy_Render::RegisterDeferred( const GLToy_Renderable* const pxDeferred )
{
    s_xDeferredRenderables.Append( pxDeferred );
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
        
        GLToy_Texture_System::BindFrameBufferTexture( *s_puCurrentTexture, uTextureUnit );

        if( s_puCurrentBuffer == &s_uFrameBuffer )
        {
            s_puCurrentBuffer = &s_uSwapBuffer;
            s_puCurrentBufferNoDepth = &s_uSwapBufferNoDepth;
            s_puCurrentTexture = &s_uSwapTexture;
        }
        else
        {
            s_puCurrentBuffer = &s_uFrameBuffer;
            s_puCurrentBufferNoDepth = &s_uFrameBufferNoDepth;
            s_puCurrentTexture = &s_uFrameTexture;
        }

        BindFramebuffer( FRAMEBUFFER, *s_puCurrentBuffer );
    }
}

