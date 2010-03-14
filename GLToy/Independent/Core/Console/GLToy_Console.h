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

    static void Update();
    static void Render();

    static void ClearLog();
    static void Print( const GLToy_String& szLine );
    static void ExecuteLine( const GLToy_String& szLine, const bool bStoreInHistory = true );
    static void Toggle();

    static void RegisterCommand( const GLToy_String& szName, void ( *pfnFunction )() );
    static void RegisterCommand( const GLToy_String& szName, void ( *pfnFunction )( const bool ) );
    static void RegisterCommand( const GLToy_String& szName, void ( *pfnFunction )( const u_int ) );
    static void RegisterCommand( const GLToy_String& szName, void ( *pfnFunction )( const GLToy_String& ) );

    static void RegisterVariable( const GLToy_String& szName, bool* pbVariable );

private:

    static GLToy_ConsoleCommand* LookUpCommand( const GLToy_String& szName );

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