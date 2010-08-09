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

#ifndef __GLTOY_H_
#define __GLTOY_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// these should always come first
#include <Core/Platform_GLToy.h>
#include <Core/GLToy_Config.h>

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

// prevent const_cast and dynamic cast
// TODO: work out a better way, this fails spectacularly with VC 2010 which feels compelled to #include <exception> somewhere
// presumably for the standard library bits that I use, despite the fact that exceptions are disabled
//#define const_cast CONST_CAST_IS_FORBIDDEN_if_you_really_must_then_undef_const_cast
//#define dynamic_cast DYNAMIC_CAST_IS_FORBIDDEN_dont_undef_dynamic_cast

// is this a debug build?
// also set all debug related defines here
#ifdef _DEBUG
    #define GLTOY_DEBUG
    #define GLToy_IsDebugBuild() ( true )
    #define GLToy_DebugVar static
    #define GLToy_DebugOutput GLToy::DebugOutput
#else
    #define GLTOY_RELEASE
    #define GLToy_IsDebugBuild() ( false )
    #define GLToy_DebugVar static const
    #define GLToy_DebugOutput( format, ... ) ;
#endif

#define GLToy_DebugOutput_Release GLToy::DebugOutput

#define GLToy_HeaderBytes( string ) ( ( string[ 3 ] << 24 ) | ( string[ 2 ] << 16 ) | ( string[ 1 ] << 8 ) | string[ 0 ] )

#define GLToy_InitialiserCall( system ) GLToy_DebugOutput( "\r\n  " #system "\r\n" ); if( !system::Initialise() ) { GLToy_DebugOutput( "\r\n  Failed to initialise " #system "!\r\n" ); return false; }

#define GLToy_IsValidHandle( handle ) ( ( handle ) > 0 )

/////////////////////////////////////////////////////////////////////////////////////////////
// T Y P E D E F S
/////////////////////////////////////////////////////////////////////////////////////////////

typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;

typedef int GLToy_Handle;
typedef unsigned int GLToy_Hash;

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const u_int uDEBUGOUTPUT_MAX_LENGTH = 512;

static const GLToy_Handle uGLTOY_INVALID_HANDLE = -1;
static const GLToy_Hash uGLTOY_BAD_HASH = 0;

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_String;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy
{
    
public:

    static int EntryPoint();
    static bool Resize( const int& iWidth, const int& iHeight );
    static void UpdateBuffers();

    static void DebugOutput( const GLToy_String& szString );
    static void DebugOutput( const char* szFormatString, ... );

    static u_int GetWindowViewportWidth();
    static u_int GetWindowViewportHeight();

    static void Quit() { s_bQuitFlag = true; }

    static void LoseFocus() { s_bHasFocus = false; }
    static void GiveFocus() { s_bHasFocus = true; }
    static bool HasFocus() { return s_bHasFocus; }

    static void ChangeWindowTitle( const char* const szNewTitle );
    static void ChangeWindowIcon( const char* const szTextureName );

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

    static void Project_Update();

    static void Platform_DebugOutput( const char* const szString );

    static void Platform_ChangeWindowTitle( const char* const szNewTitle );
    static void Platform_ChangeWindowIcon( const char* const szTextureName );

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
