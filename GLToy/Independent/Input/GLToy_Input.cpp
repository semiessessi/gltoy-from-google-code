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
u_int GLToy_Input_System::s_uEscapeKeyCode = 0;
u_int GLToy_Input_System::s_uExecuteKeyCode = 0;
u_int GLToy_Input_System::s_uReturnKeyCode = 0;
u_int GLToy_Input_System::s_uUpKeyCode = 0;
u_int GLToy_Input_System::s_uDownKeyCode = 0;
u_int GLToy_Input_System::s_uLeftKeyCode = 0;
u_int GLToy_Input_System::s_uRightKeyCode = 0;
u_int GLToy_Input_System::s_uPageUpKeyCode = 0;
u_int GLToy_Input_System::s_uPageDownKeyCode = 0;
u_int GLToy_Input_System::s_uSpaceKeyCode = 0;

float GLToy_Input_System::s_fMouseDeltaX = 0.0f;
float GLToy_Input_System::s_fMouseDeltaY = 0.0f;

GLToy_KeyInputHandler* GLToy_Input_System::s_pxKeyInputHandler = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Input_System::SetKeyInputHandler( GLToy_KeyInputHandler* pxKeyInputHandler )
{
#ifndef GLTOY_DEMO
    s_pxKeyInputHandler = pxKeyInputHandler;
#endif
}

bool GLToy_Input_System::Initialise()
{
#ifndef GLTOY_DEMO
    ChangeLayout();
#endif
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
#ifndef GLTOY_DEMO
    if( s_pxKeyInputHandler )
    {
        s_pxKeyInputHandler->HandleCharacter( wcCharacter );
    }
#endif
}

void GLToy_Input_System::HandleKey( const u_int uKey )
{
#ifndef GLTOY_DEMO
    if( s_pxKeyInputHandler )
    {
        s_pxKeyInputHandler->HandleKey( uKey );
    }

    if( uKey == GLToy_Input_System::GetConsoleKey() )
    {
        GLToy_Console::Toggle();
        return;
    }
#endif

	if( GLToy::GetGloballyQuitWithEscape() && ( uKey == GLToy_Input_System::GetEscapeKey() ) )
	{
		GLToy::QuitDialog();
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
