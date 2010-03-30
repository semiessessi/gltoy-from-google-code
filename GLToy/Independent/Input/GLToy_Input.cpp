/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of GLToy.
//
// GLToy is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// GLToy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with GLToy.
// If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////////////////

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

float GLToy_Input_System::s_fMouseDeltaX = 0.0f;
float GLToy_Input_System::s_fMouseDeltaY = 0.0f;

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
    if( !GLToy::HasFocus() )
    {
        return;
    }

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
    if( !GLToy::HasFocus() )
    {
        return false;
    }

    return s_pxKeyInputHandler ? false : Platform_IsKeyDown( uKey );
}

bool GLToy_Input_System::IsMouseLeftButtonDown()
{
    if( !GLToy::HasFocus() )
    {
        return false;
    }

    return Platform_IsMouseLeftButtonDown();
}

bool GLToy_Input_System::IsMouseMiddleButtonDown()
{
    if( !GLToy::HasFocus() )
    {
        return false;
    }

    return Platform_IsMouseMiddleButtonDown();
}

bool GLToy_Input_System::IsMouseRightButtonDown()
{
    if( !GLToy::HasFocus() )
    {
        return false;
    }

    return Platform_IsMouseRightButtonDown();
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
