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

#ifndef __GLTOY_CONSOLE_H_
#define __GLTOY_CONSOLE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <String/GLToy_String.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_ConsoleCommand;
template < class T > class GLToy_HashTree;
class GLToy_Font;
class GLToy_KeyInputHandler;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Console
{
    
    friend class GLToy_Console_KeyInputHandler;

public:

    static bool Initialise();
    static void Shutdown();

    static void Render2D();

    static void Update();

    static void ClearLog();
    static void Print( const GLToy_String& szLine );
    static void ExecuteLine( const GLToy_String& szLine, const bool bStoreInHistory = true );
    static void Toggle();
    static bool IsDown() { return s_bConsoleDown; }

    static const GLToy_Font* GetFont() { return s_pxFont; }

    static void RegisterCommand( const GLToy_String& szName, void ( *pfnFunction )() );
    static void RegisterCommand( const GLToy_String& szName, void ( *pfnFunction )( const bool ) );
    static void RegisterCommand( const GLToy_String& szName, void ( *pfnFunction )( const u_int ) );
    static void RegisterCommand( const GLToy_String& szName, void ( *pfnFunction )( const GLToy_String& ) );

    static void RegisterVariable( const GLToy_String& szName, bool* pbVariable );
    static void RegisterVariable( const GLToy_String& szName, u_int* puVariable );

private:

    static GLToy_ConsoleCommand* LookUpCommand( const GLToy_String& szName );
    static void ListAll();

    static void HandleCharacter( const wchar_t wcCharacter );
    static void HandleKey( const unsigned int uKey );

    static GLToy_Array< GLToy_String > s_xHistory;
    static GLToy_Array< GLToy_String > s_xLog;

    static GLToy_Console_KeyInputHandler s_xInputHandler;
    static bool s_bConsoleDown;
    static float s_fSlideOffset;

    static u_int s_uHistoryPosition;
    static u_int s_uVerticalPosition;

    static GLToy_Font* s_pxFont;

    static GLToy_HashTree< GLToy_ConsoleCommand* > s_xCommandTree;

};

#endif
