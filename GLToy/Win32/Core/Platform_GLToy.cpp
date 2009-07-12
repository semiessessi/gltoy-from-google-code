/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// This file's header
#include <Core/GLToy.h>

// C++
#include <stdio.h>

// Win32
#include <windows.h>

// GL
#include <gl/gl.h>
#include <gl/glu.h>

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

LRESULT CALLBACK WndProc( HWND uWindowHandle, unsigned int uMessage,
                            WPARAM uWParam, LPARAM uLParam )
{
    switch( uMessage )
    {
        
    case WM_SIZE:
        
        GLToy::Resize( LOWORD( uLParam ), HIWORD( uLParam ) );
        return 0;

    case WM_CLOSE:

        PostMessage( uWindowHandle, WM_QUIT, 0, 0 );
        return 0;

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
    xWindowClass.hCursor			= LoadCursor( g_uInstance, IDC_ARROW );
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
        GLToy_Assert( false, "Failed to create window!" );
        return false;
    }

    g_uDeviceContext = GetDC( g_uWindowHandle );

    if( !g_uDeviceContext )
    {
        GLToy_Assert( false, "Failed to create device context!" );
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
        GLToy_Assert( false, "Failed to choose pixel format!" );
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
        GLToy_Assert( false, "Failed to create render context!" );
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
        GLToy_Assert( false, "Shutting down before render context was set" );
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
    }

    return true;
}

bool GLToy::Platform_Resize( const int& iWidth, const int& iHeight )
{
    glViewport( 0, 0, iWidth, iHeight );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    gluPerspective( 90.0f, static_cast<float>( iWidth ) / static_cast<float>( iHeight ), 0.1f, 100.0f );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    return true;
}

void GLToy::Platform_UpdateBuffers()
{
    glFlush();

    SwapBuffers( g_uDeviceContext );
}

void GLToy::Platform_DebugOutput(const char* sString )
{
    OutputDebugString( sString );
}