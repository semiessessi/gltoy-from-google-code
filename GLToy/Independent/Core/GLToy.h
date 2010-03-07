#ifndef __GLTOY_H_
#define __GLTOY_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// L I B R A R I E S
/////////////////////////////////////////////////////////////////////////////////////////////

#ifdef WIN32
    #pragma comment( lib, "opengl32" )
    #pragma comment( lib, "glu32" )
#endif

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// this should always come first
#include <Core/Platform_GLToy.h>

// core
#include <Core/GLToy_Assert.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// M A C R O S
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NULL
#define NULL (0)
#endif

// is this a debug build?
// also set all debug related defines here
#ifdef _DEBUG
    #define GLTOY_DEBUG
    #define GLToy_DebugVar static
    #define GLToy_DebugOutput( format, ... ) GLToy::DebugOutput( format, __VA_ARGS__ )
#else
    #define GLTOY_RELEASE
    #define GLToy_DebugVar static const
    #define GLToy_DebugOutput( format, ... ) ;
#endif

#define GLToy_DebugOutput_Release( format, ... ) GLToy::DebugOutput( format, __VA_ARGS__ )

#define GLTOY_INITIALISER_CALL( system ) GLToy_DebugOutput( "\r\n  " #system "\r\n" ); if( !system::Initialise() ) { return false; }

/////////////////////////////////////////////////////////////////////////////////////////////
// T Y P E D E F S
/////////////////////////////////////////////////////////////////////////////////////////////

typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;

typedef unsigned int GLToy_Hash;

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const u_int uDEBUGOUTPUT_MAX_LENGTH = 512;

static const GLToy_Hash uGLTOY_BAD_HASH = 0;

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_State;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy
{
    
public:

    static int EntryPoint();
    static bool Resize( const int& iWidth, const int& iHeight );
    static void UpdateBuffers();

    static void DebugOutput( const char* szFormatString, ... );

    static u_int GetWindowViewportWidth();
    static u_int GetWindowViewportHeight();

    static void ChangeState( GLToy_State* pxState );

private:

    static bool Initialise();
    static void Shutdown();

    static bool MainLoop();

    static bool Platform_Resize( const int& iWidth, const int& iHeight );
    static void Platform_UpdateBuffers();

    static bool Platform_EarlyInitialise();
    static bool Platform_LateInitialise();
    static void Platform_Shutdown();

    static bool Platform_MainLoop();

    static void Platform_DebugOutput( const char* sString );

    static bool s_bFullscreen;
    static int s_iWidth;
    static int s_iHeight;

#ifdef WIN32
    friend unsigned int WndProc( unsigned int uWindowHandle, unsigned int uMessage,
                                    unsigned int uWParam, unsigned int uLParam );
#endif

};

#endif
