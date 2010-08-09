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
#include <Input/GLToy_Input.h>

// C++
#include <stdarg.h>
#include <stdio.h>
#include <wchar.h>

// X11
#include<X11/X.h>
#include<X11/Xlib.h>

// OpenGL
#include<GL/gl.h>
#include<GL/glx.h>
#include<GL/glu.h>


/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////
static Display* g_xDisplay;
static XVisualInfo* g_xVisualInfo;
static Colormap g_xColormap;
static Window g_xWindow;
static GLXContext g_xRenderContext;
static XWindowAttributes g_xWindowAttributes;
static XEvent g_xEvent;

// note, these are minimum values
static const unsigned int uGLTOY_COMPONENT_DEPTH    = 4;
static const unsigned int uGLTOY_Z_DEPTH            = 16;
static const unsigned int uGLTOY_STENCIL_DEPTH    	= 4;
static const unsigned int uGLTOY_MAX_VSCWPRINTF		= 4096;

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
	g_xDisplay = XOpenDisplay( NULL );
 
	if( !g_xDisplay )
	{
		GLToy_Assert( g_xDisplay != NULL, "XOpenDisplay failed" );
		return false;
	}

	Window xRootWindow = DefaultRootWindow( g_xDisplay );

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

	g_xVisualInfo = glXChooseVisual( g_xDisplay, 0, aiAttributes );

	if( !g_xVisualInfo )
	{
		GLToy_Assert( g_xVisualInfo != NULL, "glXChooseVisual failed" );
		return false;
	}

	GLToy_DebugOutput( "Visual %p selected", g_xVisualInfo->visualid );

	g_xColormap = XCreateColormap( g_xDisplay, xRootWindow, g_xVisualInfo->visual, AllocNone );

	XSetWindowAttributes xSWAttributes;
	xSWAttributes.colormap = g_xColormap;
	xSWAttributes.event_mask =
		ExposureMask
		| KeyPressMask
		| ButtonPressMask
		| EnterWindowMask
		| LeaveWindowMask
		| StructureNotifyMask;

	g_xWindow = XCreateWindow(
		g_xDisplay,
		xRootWindow,
		0, 0,
		600, 600,
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

	XMapWindow( g_xDisplay, g_xWindow );
	XStoreName( g_xDisplay, g_xWindow, "GLToy" );

	g_xRenderContext = glXCreateContext( g_xDisplay, g_xVisualInfo, NULL, GL_TRUE );
 	glXMakeCurrent( g_xDisplay, g_xWindow, g_xRenderContext );

	XFree( g_xVisualInfo );

    return true;
}

bool GLToy::Platform_LateInitialise()
{
    return true;
}

void GLToy::Platform_Shutdown()
{
	glXMakeCurrent( g_xDisplay, None, NULL );
	glXDestroyContext( g_xDisplay, g_xRenderContext );
	XDestroyWindow( g_xDisplay, g_xWindow );
	XCloseDisplay( g_xDisplay );
}

bool GLToy::Platform_MainLoop()
{
	while( XPending( g_xDisplay ) > 0 )
	{
		XNextEvent( g_xDisplay, &g_xEvent );
		switch( g_xEvent.type )
		{
			case DestroyNotify:
			{
				if( g_xEvent.xdestroywindow.window == g_xWindow )
				{
					return false;
				}
				break;
			}

			case EnterNotify:
			{
				if( g_xEvent.xfocus.window == g_xWindow )
				{
					GLToy::GiveFocus();
				}
				break;
			}

			case LeaveNotify:
			{
				if( g_xEvent.xfocus.window == g_xWindow )
				{
					GLToy::LoseFocus();
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

			case Expose:
			{
				// update for render...
    			XGetWindowAttributes( g_xDisplay, g_xWindow, &g_xWindowAttributes );
				break;
			}

			default:
			{
				GLToy_DebugOutput( false, "Unhandled X11 Event, type = %X", g_xEvent.type );
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
	glXSwapBuffers( g_xDisplay, g_xWindow );
}

void GLToy::Platform_DebugOutput( const char* const szString )
{
    printf( szString );
}

void GLToy::Platform_ChangeWindowTitle( const char* const szNewTitle )
{
	XStoreName( g_xDisplay, g_xWindow, szNewTitle );
}

void GLToy::Platform_ChangeWindowIcon( const char* const szTextureName )
{
}
