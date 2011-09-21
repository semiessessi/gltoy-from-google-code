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

#ifndef __GLTOY_H_
#define __GLTOY_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// M A C R O S
/////////////////////////////////////////////////////////////////////////////////////////////

// is this a debug build?
// also set all debug related defines here
#ifdef _DEBUG
    #define GLTOY_DEBUG
    #define GLToy_IsDebugBuild() ( true )
    #define GLToy_DebugVar static
    #define GLToy_DebugOutput GLToy::DebugOutput
#else
    #define GLTOY_RELEASE
    #ifdef _FINAL
        #define GLTOY_FINAL
    #endif
    #define GLToy_IsDebugBuild() ( false )
    #define GLToy_DebugVar static const
    #define GLToy_DebugOutput( format, ... ) ;
#endif

#ifdef _DEMO
    #define GLTOY_DEMO
#endif

#ifdef _NO_MEMORY
    #define GLTOY_NO_MEMORY
#endif

/////////////////////////////////////////////////////////////////////////////////////////////
// T Y P E D E F S
/////////////////////////////////////////////////////////////////////////////////////////////

typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long long u_int64;

typedef int GLToy_Handle;
typedef unsigned int GLToy_Hash;

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Order is important here!!!
#include <Core/Platform_GLToy.h>
#include <Core/GLToy_Config.h>
#include <Core/GLToy_Assert.h>
#include <Core/GLToy_Memory.h>
#include <Core/GLToy_Memory_DebugOn.h>
#include <Core/GLToy_CRTP.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// P R A G M A S
/////////////////////////////////////////////////////////////////////////////////////////////

#pragma warning( disable : 4250 ) // inheritance by dominance - which I think is pretty desirable behaviour...
#pragma warning( disable : 4258 ) // warning about for loop scopes behaving as you would expect any scope to (wtf???)
#pragma warning( disable : 4724 ) // potential mod by zero - this should be checked with asserts - let release builds do whatever...
#pragma warning( disable : 4985 ) // - to allow build with MS v90 toolset

/////////////////////////////////////////////////////////////////////////////////////////////
// M A C R O S
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NULL
    #define NULL ( 0 )
#endif

// prevent const_cast and dynamic cast
// TODO: work out a better way, this fails spectacularly with VC 2010 which feels compelled to #include <exception> somewhere
// presumably for the standard library bits that I use, despite the fact that exceptions are disabled
//#define const_cast CONST_CAST_IS_FORBIDDEN_if_you_really_must_then_undef_const_cast
#define dynamic_cast DYNAMIC_CAST_IS_FORBIDDEN_dont_undef_dynamic_cast

#ifdef GLTOY_DEMO
	#define GLToy_DebugOutput_Release
#else
	#define GLToy_DebugOutput_Release GLToy::DebugOutput
#endif

#define GLToy_MaxUint ( 0xFFFFFFFF )
#define GLToy_HeaderBytes( string ) ( ( string[ 3 ] << 24 ) | ( string[ 2 ] << 16 ) | ( string[ 1 ] << 8 ) | string[ 0 ] )
#define GLToy_InitialiserCall( system ) GLToy_DebugOutput_Release( "\r\n  " #system "\r\n" ); if( !system::Initialise() ) { GLToy_DebugOutput_Release( "\r\n  Failed to initialise " #system "!\r\n" ); return false; }
#define GLToy_IsValidHandle( handle ) ( ( handle ) > 0 )
#define GLToy_XORSwap( a, b ) do { a ^= b; b ^= a; a ^= b; } while( false )

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const u_int uDEBUGOUTPUT_MAX_LENGTH =
#ifdef GLTOY_DEMO
	1;
#else
	512;
#endif

static const GLToy_Handle uGLTOY_INVALID_HANDLE = -1;
static const GLToy_Hash uGLTOY_BAD_HASH = 0;

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_BitStream;
class GLToy_String;
class GLToy_Vector_2;

class GLToy
{
    
public:

    static int EntryPoint( const GLToy_String& szCommandLine );
    static int EntryPoint();
    static void SilentEntryPoint(); // for libraries etc that need systems
    static void SilentShutdown();

    static bool Resize( const int& iWidth, const int& iHeight );
    static void UpdateBuffers();

    static void DebugOutput( const GLToy_String& szString );
    static void DebugOutput( const char* szFormatString, ... );

    static u_int GetWindowViewportWidth();
    static u_int GetWindowViewportHeight();
	static GLToy_Vector_2 GetWindowPos();

    static void Quit() { s_bQuitFlag = true; }
	static void QuitDialog();

    static void LoseFocus() { s_bHasFocus = false; }
    static void GiveFocus() { s_bHasFocus = true; }
    static bool HasFocus() { return s_bHasFocus; }

    static void ChangeWindowTitle( const char* const szNewTitle );
    static void ChangeWindowIcon( const char* const szTextureName );

	// a bit hacky but very useful for small projects
	static void SetGloballyQuitWithEscape( const bool bEscapeQuits = true ) { s_bEscapeQuits = bEscapeQuits; }
	static bool GetGloballyQuitWithEscape() { return s_bEscapeQuits; }

    static bool IsSilent() { return s_bSilent; }

    static void RegisterDebugOutputCallback( void ( *pfnCallback )( const char* const szMessage ) ) { s_pfnDebugOutputCallback = pfnCallback; }

    static float GetRenderProfileTimer() { return s_fRenderTimer; }
    static float GetUpdateProfileTimer() { return s_fUpdateTimer; }
    static float GetGPUProfileTimer() { return s_fGPUTimer; }
    static float GetSyncProfileTimer() { return s_fSyncTimer; }

	static const GLToy_String& GetCommandLine() { return s_szCommandLine; }

private:

    static bool Initialise();
    static void Shutdown();

    static bool MainLoop();

    static void Render();
    static void Update();

    static bool Platform_Resize( const int& iWidth, const int& iHeight );
    static void Platform_UpdateBuffers();

    static bool Platform_EarlyInitialise();
    static bool Platform_LateInitialise();
    static void Platform_Shutdown();

    static bool Platform_MainLoop();

    static bool Project_Initialise();
    static void Project_Shutdown();

    static void Project_Update();

    static void Platform_DebugOutput( const char* const szString );

    static void Platform_ChangeWindowTitle( const char* const szNewTitle );
    static void Platform_ChangeWindowIcon( const char* const szTextureName );
	static GLToy_Vector_2 Platform_GetWindowPos();

    static GLToy_String s_szCommandLine;

    static bool s_bFullscreen;
    static int s_iWidth;
    static int s_iHeight;

    static bool s_bQuitFlag;
    static bool s_bHasFocus;
	static bool s_bEscapeQuits;
    static bool s_bSilent;

    static void ( *s_pfnDebugOutputCallback )( const char* const szMessage );

    static float s_fUpdateTimer;
    static float s_fRenderTimer;
    static float s_fGPUTimer;
    static float s_fSyncTimer;

#ifdef WIN32
    friend unsigned int WndProc( unsigned int uWindowHandle, unsigned int uMessage,
                                    unsigned int uWParam, unsigned int uLParam );
#endif

};

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// SE - help correct initialisation order for statics which are pool allocated
#include <Core/Data Structures/GLToy_Pool.h>

#endif
