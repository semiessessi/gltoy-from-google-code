/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010, 2011 Semi Essessi
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

// This file's header
#include <Render/GLToy_Texture_Renderable.h>

// GLToy
#include <Core/Data Structures/GLToy_Pair.h>
#include <Core/Data Structures/GLToy_Stack.h>
#include <Render/GLToy_Texture_System.h>

// Win32
#include <Windows.h>

// GL
#include <gl/gl.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

struct GLToy_Viewport
{
    int m_iX;
    int m_iY;
    int m_iWidth;
    int m_iHeight;
};

/////////////////////////////////////////////////////////////////////////////////////////////
// M A C R O S
/////////////////////////////////////////////////////////////////////////////////////////////

// texture edge clamp extension...
#define GL_CLAMP_TO_EDGE ( 0x812F )

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

static int g_aiViewport[ 4 ];
static GLToy_Stack< GLToy_Pair< GLToy_Viewport, u_int > > g_xFBStack;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Texture_Renderable::Platform_Create()
{
    // set up framebuffer
    GLToy_Render::GenFramebuffers( 1, &m_uFrameBuffer );
    GLToy_Render::BindFramebuffer( FRAMEBUFFER, m_uFrameBuffer );
    GLToy_Render::GenRenderbuffers( 1, &m_uDepthBuffer );
    GLToy_Render::BindRenderbuffer( RENDERBUFFER, m_uDepthBuffer );
    GLToy_Render::RenderbufferStorage( RENDERBUFFER, DEPTH_COMPONENT24, m_uWidth, m_uHeight );
    GLToy_Render::FramebufferRenderbuffer( FRAMEBUFFER, DEPTH_ATTACHMENT, RENDERBUFFER, m_uDepthBuffer );
    GLToy_Texture_System::CreateFrameBufferTexture( m_uID, m_uWidth, m_uHeight );
    GLToy_Render::FramebufferTexture2D( FRAMEBUFFER, COLOR_ATTACHMENT0, TEXTURE_2D, m_uID, 0 );

    if( GLToy_Render::CheckFramebufferStatus( FRAMEBUFFER ) != FRAMEBUFFER_COMPLETE )
    {
        GLToy_Assert( false, "Framebuffer incomplete!" );
        if( m_iFrameBuffer != -1 )
        {
            GLToy_Render::DeleteFramebuffers( 1, &m_uFrameBuffer );
            m_iFrameBuffer = -1;
        }

        if( m_iDepthBuffer != -1 )
        {
            GLToy_Render::DeleteRenderbuffers( 1, &m_uDepthBuffer );
            m_iDepthBuffer = -1;
        }
    }

    GLToy_Render::BindFramebuffer( FRAMEBUFFER, 0 );
}

void GLToy_Texture_Renderable::Platform_Destroy()
{
    if( m_iID != -1 )
    {
        glDeleteTextures( 1, &m_uID );
        m_iID = -1;
    }

    if( m_iFrameBuffer != -1 )
    {
        GLToy_Render::DeleteFramebuffers( 1, &m_uFrameBuffer );
        m_iFrameBuffer = -1;
    }

    if( m_iDepthBuffer != -1 )
    {
        GLToy_Render::DeleteRenderbuffers( 1, &m_uDepthBuffer );
        m_iDepthBuffer = -1;
    }
}

void GLToy_Texture_Renderable::Platform_Bind( const u_int uTextureUnit ) const
{
    GLToy_Texture_System::BindFrameBufferTexture( m_uID, uTextureUnit );
}

void GLToy_Texture_Renderable::Platform_BeginRender() const
{
    int aiViewport[ 4 ];
    glGetIntegerv( GL_VIEWPORT, aiViewport );

    GLToy_Viewport xViewport = { aiViewport[ 0 ], aiViewport[ 1 ], aiViewport[ 2 ], aiViewport[ 3 ] };
    u_int uFBID;

    glGetIntegerv( DRAW_FRAMEBUFFER_BINDING, reinterpret_cast< int* const >( &uFBID ) );

    g_xFBStack.Push( GLToy_Pair< GLToy_Viewport, u_int >( xViewport, uFBID ) );

    glViewport( 0, 0, m_uWidth, m_uHeight );
    GLToy_Render::BindFramebuffer( FRAMEBUFFER, m_uFrameBuffer );
}

void GLToy_Texture_Renderable::Platform_EndRender() const
{
    const GLToy_Pair< GLToy_Viewport, u_int >& xStackTop = g_xFBStack.Peek();
    glViewport( xStackTop.First().m_iX, xStackTop.First().m_iY, xStackTop.First().m_iWidth, xStackTop.First().m_iHeight );
    GLToy_Render::BindFramebuffer( FRAMEBUFFER, xStackTop.Second() );

    g_xFBStack.Pop();
}
