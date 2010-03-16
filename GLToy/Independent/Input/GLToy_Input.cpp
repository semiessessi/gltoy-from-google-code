/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Input/GLToy_Input.h>

// GLToy
#include <Core/Console/GLToy_Console.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

static GLToy_KeyInputHandler g_xDefaultKeyInputHandler;

u_int GLToy_Input_System::s_uConsoleKeyCode = 0;
u_int GLToy_Input_System::s_uExecuteKeyCode = 0;
u_int GLToy_Input_System::s_uReturnKeyCode = 0;
u_int GLToy_Input_System::s_uUpKeyCode = 0;
u_int GLToy_Input_System::s_uDownKeyCode = 0;
u_int GLToy_Input_System::s_uLeftKeyCode = 0;
u_int GLToy_Input_System::s_uRightKeyCode = 0;
u_int GLToy_Input_System::s_uPageUpKeyCode = 0;
u_int GLToy_Input_System::s_uPageDownKeyCode = 0;

GLToy_KeyInputHandler* GLToy_Input_System::s_pxKeyInputHandler = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Input_System::SetKeyInputHandler( GLToy_KeyInputHandler* pxKeyInputHandler )
{
    s_pxKeyInputHandler = pxKeyInputHandler;
}

bool GLToy_Input_System::Initialise()
{
    ChangeLayout();

    return true;
}

void GLToy_Input_System::Update()
{
    Platform_Update();
}

void GLToy_Input_System::ChangeLayout()
{
    Platform_ChangeLayout();

    // this would be the approriate time to change key labels...
}

void GLToy_Input_System::HandleCharacter( const wchar_t wcCharacter )
{
    if( s_pxKeyInputHandler )
    {
        s_pxKeyInputHandler->HandleCharacter( wcCharacter );
    }
}

void GLToy_Input_System::HandleKey( const u_int uKey )
{
    if( s_pxKeyInputHandler )
    {
        s_pxKeyInputHandler->HandleKey( uKey );
    }

    if( uKey == GLToy_Input_System::GetConsoleKey() )
    {
        GLToy_Console::Toggle();
        return;
    }
}

bool GLToy_Input_System::IsKeyDown( const u_int uKey )
{
    return s_pxKeyInputHandler ? false : Platform_IsKeyDown( uKey );
}

GLToy_KeyInputHandler::GLToy_KeyInputHandler()
: m_uCaret( 0 )
, m_bInsert( true )
, m_szInput()
, m_bHandleStringInput( true )
{
}

void GLToy_KeyInputHandler::HandleCharacter( const wchar_t wcCharacter )
{
    if( !m_bHandleStringInput )
    {
        return;
    }

    Platform_HandleCharacter( wcCharacter );
}

void GLToy_KeyInputHandler::HandleKey( const unsigned int uKey )
{
    Platform_HandleKey( uKey );
}
