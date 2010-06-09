/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009, 2010 Semi Essessi
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

// This file's header
#include <Core/GLToy.h>

// GLToy
#include <Input/GLToy_Input.h>
#include <String/GLToy_String.h>

// C++
#include <stdio.h>

// Win32
#include <windows.h>
#include <gdiplus.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// U S I N G   N A M E S P A C E S
/////////////////////////////////////////////////////////////////////////////////////////////

using namespace Gdiplus; 

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

static HGLRC           g_uRenderContext             = 0;
static HDC             g_uDeviceContext             = 0;
static HWND            g_uWindowHandle              = 0;
static HINSTANCE       g_uInstance                  = 0;
static bool            g_bFullscreen                = false;

static const unsigned int uGLTOY_COLOUR_DEPTH       = 32;
static const unsigned int uGLTOY_Z_DEPTH            = 24;
static const unsigned int uGLTOY_STENCIL_DEPTH      = 8;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

u_int Win32_GLToy_GetDC()
{
    return reinterpret_cast< u_int >( g_uDeviceContext );
}

LRESULT CALLBACK WndProc( HWND uWindowHandle, unsigned int uMessage,
                            WPARAM uWParam, LPARAM uLParam )
{
    switch( uMessage )
    {
        
        case WM_SIZE:
        {
            GLToy::Resize( LOWORD( uLParam ), HIWORD( uLParam ) );
            return 0;
        }

        case WM_CLOSE:
        {
            PostMessage( uWindowHandle, WM_QUIT, 0, 0 );
            return 0;
        }

        case WM_INPUTLANGCHANGE:
        {
            GLToy_Input_System::ChangeLayout();
            break;
        }

        case WM_SETFOCUS:
        {
            GLToy::GiveFocus();
            ShowCursor( FALSE );
            break;
        }

        case WM_KILLFOCUS:
        {
            GLToy::LoseFocus();
            ShowCursor( TRUE );
            break;
        }
        //case WM_MOUSEWHEEL:

        case WM_KEYDOWN:
        {
            if( GLToy::HasFocus() )
            {
                GLToy_Input_System::HandleKey( uWParam );
            }
            break;
        }

        case WM_CHAR:
        {
            if( GLToy::HasFocus() )
            {
                GLToy_Input_System::HandleCharacter( static_cast< wchar_t >( uWParam ) );
            }
            break;
        }

    }

    return DefWindowProc( uWindowHandle, uMessage, uWParam, uLParam );
}

bool GLToy::Platform_EarlyInitialise()
{
    g_uInstance = GetModuleHandle( NULL );

    WNDCLASSA xWindowClass;

    memset( &xWindowClass, 0, sizeof( WNDCLASSA ) );

    xWindowClass.lpszClassName      = "GLToy";
    xWindowClass.style              = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    xWindowClass.hIcon              = LoadIcon( g_uInstance, IDI_WINLOGO );
    xWindowClass.hCursor            = LoadCursor( g_uInstance, IDC_ARROW );
    xWindowClass.hInstance          = g_uInstance;
    xWindowClass.lpfnWndProc        = WndProc;

    if( !RegisterClassA( &xWindowClass ) )
    {
        GLToy_Assert( false, "Failed to register window class" );
        return false;
    }

    if( s_bFullscreen )
    {
        DEVMODE xDeviceMode;

        memset( &xDeviceMode, 0, sizeof( DEVMODE ) );

        xDeviceMode.dmSize = sizeof( DEVMODE );
        xDeviceMode.dmPelsWidth = s_iWidth;
        xDeviceMode.dmPelsHeight = s_iHeight;
        xDeviceMode.dmBitsPerPel = uGLTOY_COLOUR_DEPTH;

        xDeviceMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

        if( ChangeDisplaySettings( &xDeviceMode, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
        {
            GLToy_Assert( false, "Failed to change display settings - bad screen mode" );
            
            s_bFullscreen = false;
        }
    }

    unsigned int uStyle = 0;
    unsigned int uStyleEx = 0;

    if( s_bFullscreen )
    {
        uStyle = WS_POPUP;
        uStyleEx = WS_EX_APPWINDOW;
        ShowCursor( FALSE );
    }
    else
    {
        uStyle = WS_OVERLAPPEDWINDOW;
        uStyleEx = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    }

    RECT xWindowRectangle;

    xWindowRectangle.left = 0;
    xWindowRectangle.right = s_iWidth;
    xWindowRectangle.top = 0;
    xWindowRectangle.bottom = s_iHeight;

    AdjustWindowRectEx( &xWindowRectangle, uStyle, FALSE, uStyleEx );

    g_uWindowHandle = CreateWindowExA(      uStyleEx,
                                            "GLToy",
                                            "GLToy",
                                            WS_CLIPSIBLINGS | WS_CLIPCHILDREN | uStyle,
                                            0, 0,
                                            xWindowRectangle.right - xWindowRectangle.left,
                                            xWindowRectangle.bottom - xWindowRectangle.top,
                                            NULL,
                                            NULL,
                                            g_uInstance,
                                            NULL
                                        );

    if( !g_uWindowHandle )
    {
        GLToy_Assert( g_uWindowHandle != NULL, "Failed to create window!" );
        return false;
    }

    g_uDeviceContext = GetDC( g_uWindowHandle );

    if( !g_uDeviceContext )
    {
        GLToy_Assert( g_uDeviceContext != NULL, "Failed to create device context!" );
        return false;
    }

    PIXELFORMATDESCRIPTOR xPFD;

    memset( &xPFD, 0, sizeof( PIXELFORMATDESCRIPTOR ) );

    xPFD.nSize = sizeof( PIXELFORMATDESCRIPTOR );
    xPFD.nVersion = 1;
    xPFD.cColorBits = uGLTOY_COLOUR_DEPTH;
    xPFD.cDepthBits = uGLTOY_Z_DEPTH;
    xPFD.cStencilBits = uGLTOY_STENCIL_DEPTH;
    xPFD.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    xPFD.dwLayerMask = PFD_MAIN_PLANE;

    unsigned int uPixelFormat = ChoosePixelFormat( g_uDeviceContext, &xPFD );

    if( !uPixelFormat )
    {
        GLToy_Assert( uPixelFormat != NULL, "Failed to choose pixel format!" );
        return false;
    }

    if( !SetPixelFormat( g_uDeviceContext, uPixelFormat, &xPFD ) )
    {
        GLToy_Assert( false, "Failed to set pixel format!" );
        return false;
    }

    g_uRenderContext = wglCreateContext( g_uDeviceContext );

    if( !g_uRenderContext )
    {
        GLToy_Assert( g_uRenderContext != NULL, "Failed to create render context!" );
        return false;
    }

    if( !wglMakeCurrent( g_uDeviceContext, g_uRenderContext ) )
    {
        GLToy_Assert( false, "Failed to bind render context!" );
        return false;
    }

    return true;
}

bool GLToy::Platform_LateInitialise()
{
    ShowWindow( g_uWindowHandle, SW_SHOW );
    SetForegroundWindow( g_uWindowHandle );
    SetFocus( g_uWindowHandle );

    s_bHasFocus = ( GetFocus() == g_uWindowHandle );

    return true;
}

void GLToy::Platform_Shutdown()
{
    if( s_bFullscreen )
    {
        ChangeDisplaySettings( 0, 0 );
        ShowCursor( TRUE );
    }

    if( g_uRenderContext )
    {
        if( !wglMakeCurrent( NULL, NULL ) )
        {
            GLToy_Assert( false, "Failed to unbind render context" );
        }

        if( !wglDeleteContext( g_uRenderContext ) )
        {
            GLToy_Assert( false, "Failed to delete render context" );
        }
        
        g_uRenderContext = 0;

        if( g_uDeviceContext && g_uWindowHandle ) 
        {
            if ( !ReleaseDC( g_uWindowHandle, g_uDeviceContext ) )
            {
                GLToy_Assert( false, "Failed to release device context" );
            }

            g_uDeviceContext = 0;

            if( !DestroyWindow( g_uWindowHandle ) )
            {
                GLToy_Assert( false, "Failed to destroy window" );
            }

            g_uWindowHandle = 0;
        }
        else
        {
            GLToy_Assert( false, "Trying to shutdown before device context or window handle have been setup" );
        }

        if( !UnregisterClassA( "GLToy", g_uInstance ) )
        {
            GLToy_Assert( false, "Failed to unregister \"GLToy\" window class" );
        }

    }
    else
    {
        GLToy_Assert( g_uRenderContext != NULL, "Shutting down before render context was set" );
    }
}

bool GLToy::Platform_MainLoop()
{
    MSG xMessage;
    
    while( PeekMessage( &xMessage, NULL, 0, 0, PM_REMOVE ) )
    {
        if( xMessage.message == WM_QUIT )
        {
            return false;
        }

        TranslateMessage( &xMessage );
        DispatchMessage( &xMessage );

        // sometimes this falls out of sync if relying on the messages
        // I probably don't know enough of them or something...
        s_bHasFocus = ( GetFocus() == g_uWindowHandle );
    }

    return true;
}

bool GLToy::Platform_Resize( const int& iWidth, const int& iHeight )
{
    return true;
}

void GLToy::Platform_UpdateBuffers()
{
    SwapBuffers( g_uDeviceContext );
}

void GLToy::Platform_DebugOutput( const char* const szString )
{
    OutputDebugString( szString );
}

void GLToy::Platform_ChangeWindowTitle( const char* const szNewTitle )
{
    SetWindowText( g_uWindowHandle, szNewTitle );
}

void GLToy::Platform_ChangeWindowIcon( const char* const szTextureName )
{
    const GLToy_String szPath = GLToy_String( "textures/" ) + szTextureName;

    Bitmap* pxBitmap = new Bitmap( szPath.GetWideString() );

    if( !pxBitmap )
    {
        GLToy_Assert( pxBitmap != NULL, "Failed to load image file \"%S\" with GDI+", szPath.GetWideString() );
        return;
    }

    HICON uHIcon = 0;
    pxBitmap->GetHICON( &uHIcon );
    SetClassLong( g_uWindowHandle, GCL_HICON, reinterpret_cast< LONG >( uHIcon ) );

    delete pxBitmap;
}
