/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
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

// This file's header
#include <Core/GLToy.h>

// GLToy
#include <Input/GLToy_Input_System.h>

// C++
#include <stdarg.h>
#include <stdio.h>
#include <wchar.h>

// OpenGL
#include<GL/gl.h>
#include<GL/glx.h>
#include<GL/glu.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

Display* g_pxDisplay;
static XVisualInfo* g_xVisualInfo;
static Colormap g_xColormap;
static Window g_xWindow;
static GLXContext g_xRenderContext;
static XWindowAttributes g_xWindowAttributes;
static XEvent g_xEvent;

// note, these are minimum values
static const unsigned int uGLTOY_COMPONENT_DEPTH    = 4;
static const unsigned int uGLTOY_Z_DEPTH            = 16;
static const unsigned int uGLTOY_STENCIL_DEPTH        = 4;
static const unsigned int uGLTOY_MAX_VSCWPRINTF        = 4096;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

int _vscwprintf( const wchar_t* szFormat, va_list xArguments )
{
    static wchar_t szBuffer[ uGLTOY_MAX_VSCWPRINTF ];
    return vswprintf( szBuffer, uGLTOY_MAX_VSCWPRINTF, szFormat, xArguments );
}

int _vscprintf( const char* szFormat, va_list xArguments )
{
    static char szBuffer[ uGLTOY_MAX_VSCWPRINTF ];
    return vsnprintf( szBuffer, uGLTOY_MAX_VSCWPRINTF, szFormat, xArguments );
}

bool GLToy::Platform_EarlyInitialise()
{
    g_pxDisplay = XOpenDisplay( NULL );
 
    if( !g_pxDisplay )
    {
        GLToy_Assert( g_pxDisplay != NULL, "XOpenDisplay failed" );
        return false;
    }

    Window xRootWindow = DefaultRootWindow( g_pxDisplay );

    const int aiAttributes[] =
    {
        GLX_RGBA,
        GLX_DOUBLEBUFFER,
        GLX_RED_SIZE, uGLTOY_COMPONENT_DEPTH,
        GLX_GREEN_SIZE, uGLTOY_COMPONENT_DEPTH,
        GLX_BLUE_SIZE, uGLTOY_COMPONENT_DEPTH,
        GLX_DEPTH_SIZE, uGLTOY_Z_DEPTH,
        GLX_STENCIL_SIZE, uGLTOY_STENCIL_DEPTH,
        None
    };

    g_xVisualInfo = glXChooseVisual( g_pxDisplay, 0, aiAttributes );

    if( !g_xVisualInfo )
    {
        GLToy_Assert( g_xVisualInfo != NULL, "glXChooseVisual failed" );
        return false;
    }

    GLToy_DebugOutput( "  Visual %p selected", g_xVisualInfo->visualid );

    g_xColormap = XCreateColormap( g_pxDisplay, xRootWindow, g_xVisualInfo->visual, AllocNone );

    XSetWindowAttributes xSWAttributes;
    xSWAttributes.colormap = g_xColormap;
    xSWAttributes.event_mask =
        ExposureMask
        | KeyPressMask
        | ButtonPressMask
        | KeyReleaseMask
        | ButtonReleaseMask
        | KeymapStateMask
        | PointerMotionMask
        | StructureNotifyMask;

    g_xWindow = XCreateWindow(
        g_pxDisplay,
        xRootWindow,
        0, 0,
        s_iWidth, s_iHeight,
        0,
        g_xVisualInfo->depth,
        InputOutput,
        g_xVisualInfo->visual,
        CWColormap | CWEventMask,
        &xSWAttributes );

    if( !g_xWindow )
    {
        GLToy_Assert( g_xWindow != NULL, "XCreateWindow failed" );
        return false;
    }

    XMapWindow( g_pxDisplay, g_xWindow );
    XStoreName( g_pxDisplay, g_xWindow, "GLToy" );

    g_xRenderContext = glXCreateContext( g_pxDisplay, g_xVisualInfo, NULL, GL_TRUE );
     glXMakeCurrent( g_pxDisplay, g_xWindow, g_xRenderContext );

    XFree( g_xVisualInfo );

    return true;
}

bool GLToy::Platform_LateInitialise()
{
    return true;
}

void GLToy::Platform_Shutdown()
{
    glXMakeCurrent( g_pxDisplay, None, NULL );
    glXDestroyContext( g_pxDisplay, g_xRenderContext );
    XDestroyWindow( g_pxDisplay, g_xWindow );
    XCloseDisplay( g_pxDisplay );
}

bool GLToy::Platform_MainLoop()
{
    Window xWindow;
    int iState;
    XGetInputFocus( g_pxDisplay, &xWindow, &iState );

    if( xWindow == g_xWindow )
    {
        GLToy::GiveFocus();
        XGrabKeyboard( g_pxDisplay, xWindow, true, GrabModeAsync, GrabModeAsync, CurrentTime );
    }
    else
    {
        GLToy::LoseFocus();
        XUngrabKeyboard( g_pxDisplay, CurrentTime );
    }

    while( XPending( g_pxDisplay ) > 0 )
    {
        XNextEvent( g_pxDisplay, &g_xEvent );
        switch( g_xEvent.type )
        {
            case ConfigureNotify:
            {
                // TODO: resize with g_xEvent.xconfigure.width and g_xEvent.xconfigure.height
                break;
            }
            case DestroyNotify:
            {
                if( g_xEvent.xdestroywindow.window == g_xWindow )
                {
                    return false;
                }
                break;
            }

            case KeyPress:
            {
                if( GLToy::HasFocus() )
                {
                    GLToy_Input_System::HandleKey( g_xEvent.xkey.keycode );
                }
                break;
            }

            case KeyRelease:
            {
                if( GLToy::HasFocus() )
                {
                    char szBuffer[ 32 ];
                    KeySym xKeySym;
                    int iLength = XLookupString( &g_xEvent.xkey, szBuffer, 32, &xKeySym, NULL );
                    if( iLength > 0 )
                    {
                        GLToy_Input_System::HandleCharacter( szBuffer[ 0 ] );
                    }
                }
                break;
            }

            case KeymapNotify:
            {
                XRefreshKeyboardMapping( &g_xEvent.xmapping );
                break;
            }

            case Expose:
            {
                // update for render...
                XGetWindowAttributes( g_pxDisplay, g_xWindow, &g_xWindowAttributes );
                break;
            }

            default:
            {
                GLToy_DebugOutput( "Unhandled X11 Event, type = %X\r\n", g_xEvent.type );
                break;
            }
        }
    }

    return true;
}

bool GLToy::Platform_Resize( const int& iWidth, const int& iHeight )
{
    return true;
}

void GLToy::Platform_UpdateBuffers()
{
    glXSwapBuffers( g_pxDisplay, g_xWindow );
}

void GLToy::Platform_DebugOutput( const char* const szString )
{
    printf( szString );
}

void GLToy::Platform_ChangeWindowTitle( const char* const szNewTitle )
{
    XStoreName( g_pxDisplay, g_xWindow, szNewTitle );
}

void GLToy::Platform_ChangeWindowIcon( const char* const szTextureName )
{
}
