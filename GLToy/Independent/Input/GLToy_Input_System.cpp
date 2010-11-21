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
#include <Input/GLToy_Input_System.h>

// GLToy
#include <Core/Console/GLToy_Console.h>
#include <Input/GLToy_InputHandler.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

static GLToy_InputHandler g_xDefaultInputHandler;

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
u_int GLToy_Input_System::s_uCtrlKeyCode = 0;
u_int GLToy_Input_System::s_uAltKeyCode = 0;

float GLToy_Input_System::s_fMouseDeltaX = 0.0f;
float GLToy_Input_System::s_fMouseDeltaY = 0.0f;
GLTOY_MOUSE_SCROLL GLToy_Input_System::s_eMouseScroll = GLTOY_MOUSE_SCROLL_NONE;
int GLToy_Input_System::s_iMouseDelta = 0;

GLToy_Stack< GLToy_InputHandler* > GLToy_Input_System::s_xInputStack;

bool GLToy_Input_System::s_bMouseLeftDebounced = false;
bool GLToy_Input_System::s_bMouseMiddleDebounced = false;
bool GLToy_Input_System::s_bMouseRightDebounced = false;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Input_System::PushInputHandler( GLToy_InputHandler* const pxInputHandler )
{
#ifndef GLTOY_DEMO
    s_xInputStack.Push( pxInputHandler );
#endif
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
    static bool ls_bDelayLeft = false;
    static bool ls_bDelayMiddle = false;
    static bool ls_bDelayRight = false;

    s_bMouseLeftDebounced = ls_bDelayLeft && !Platform_IsMouseLeftButtonDown();
    s_bMouseMiddleDebounced = ls_bDelayMiddle && !Platform_IsMouseMiddleButtonDown();
    s_bMouseRightDebounced = ls_bDelayRight && !Platform_IsMouseRightButtonDown();

    ls_bDelayLeft = Platform_IsMouseLeftButtonDown();
    ls_bDelayMiddle = Platform_IsMouseMiddleButtonDown();
    ls_bDelayRight = Platform_IsMouseRightButtonDown();

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
    if( ( s_xInputStack.GetCount() > 0 ) && s_xInputStack.Peek() )
    {
        s_xInputStack.Peek()->HandleCharacter( wcCharacter );
    }
#endif
}

void GLToy_Input_System::HandleKey( const u_int uKey )
{
#ifndef GLTOY_DEMO
    if( ( s_xInputStack.GetCount() > 0 ) && s_xInputStack.Peek() )
    {
        s_xInputStack.Peek()->HandleKey( uKey );
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

    return ( ( s_xInputStack.GetCount() > 0 ) && s_xInputStack.Peek() ) ? false : Platform_IsKeyDown( uKey );
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

bool GLToy_Input_System::GetDebouncedMouseLeft()
{
    if( !GLToy::HasFocus() )
    {
        return false;
    }

    return s_bMouseLeftDebounced;
}

bool GLToy_Input_System::GetDebouncedMouseMiddle()
{
    if( !GLToy::HasFocus() )
    {
        return false;
    }

    return s_bMouseMiddleDebounced;
}

bool GLToy_Input_System::GetDebouncedMouseRight()
{
    if( !GLToy::HasFocus() )
    {
        return false;
    }

    return s_bMouseRightDebounced;
}
