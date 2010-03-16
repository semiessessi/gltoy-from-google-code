#ifndef __GLTOY_H_
#define __GLTOY_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// this should always come first
#include <Core/Platform_GLToy.h>

// core
#include <Core/GLToy_Assert.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// P R A G M A S
/////////////////////////////////////////////////////////////////////////////////////////////

#pragma warning( disable : 4250 ) // inheritance by dominance - which I think is pretty desirable behaviour...

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
    #define GLToy_IsDebugBuild() ( true )
    #define GLToy_DebugVar static
    #define GLToy_DebugOutput( format, ... ) GLToy::DebugOutput( format, __VA_ARGS__ )
#else
    #define GLTOY_RELEASE
    #define GLToy_IsDebugBuild() ( false )
    #define GLToy_DebugVar static const
    #define GLToy_DebugOutput( format, ... ) ;
#endif

#define GLToy_DebugOutput_Release( format, ... ) GLToy::DebugOutput( format, __VA_ARGS__ )

// (('2'<<24) + ('P'<<16) + ('D'<<8) + 'I')

#define GLToy_HeaderBytes( string ) ( ( string[ 3 ] << 24 ) | ( string[ 2 ] << 16 ) | ( string[ 1 ] << 8 ) | string[ 0 ] )

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

    static void Quit() { s_bQuitFlag = true; }

    static void LoseFocus() { s_bHasFocus = false; }
    static void GiveFocus() { s_bHasFocus = true; }
    static bool HasFocus() { return s_bHasFocus; }

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

    static bool Project_Initialise();
    static void Project_Shutdown();

    static void Platform_DebugOutput( const char* sString );

    static bool s_bFullscreen;
    static int s_iWidth;
    static int s_iHeight;

    static bool s_bQuitFlag;
    static bool s_bHasFocus;

#ifdef WIN32
    friend unsigned int WndProc( unsigned int uWindowHandle, unsigned int uMessage,
                                    unsigned int uWParam, unsigned int uLParam );
#endif

};

#endif
