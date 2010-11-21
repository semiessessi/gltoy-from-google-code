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

// Win32
#include <memory.h>
#include <windows.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Input_System::Platform_Update()
{

    POINT xCentre = { GLToy::GetWindowViewportWidth() >> 1, GLToy::GetWindowViewportHeight() >> 1 };
    POINT xPoint;
    GetCursorPos( &xPoint );

    s_fMouseDeltaX = static_cast< float >( xPoint.x - xCentre.x );
    s_fMouseDeltaY = static_cast< float >( xPoint.y - xCentre.y );

    SetCursorPos( xCentre.x, xCentre.y );

    static bool bFirst = true;
    if( bFirst )
    {
        s_fMouseDeltaX = 0.0f;
        s_fMouseDeltaY = 0.0f;
        bFirst = false;
    }

	if( s_iMouseDelta > 0 )
	{
		s_eMouseScroll = GLTOY_MOUSE_SCROLL_POSITIVE;
	}
	else if( s_iMouseDelta < 0 )
	{
		s_eMouseScroll = GLTOY_MOUSE_SCROLL_NEGATIVE;
	}
	else
	{
		s_eMouseScroll = GLTOY_MOUSE_SCROLL_NONE;
	}
	s_iMouseDelta = 0;
}

bool GLToy_Input_System::Platform_IsKeyDown( const u_int uKey )
{
    return ( GetAsyncKeyState( uKey ) & 0x8000 ) ? true : false;
}

bool GLToy_Input_System::Platform_IsMouseLeftButtonDown()
{
    return Platform_IsKeyDown( VK_LBUTTON );
}

bool GLToy_Input_System::Platform_IsMouseMiddleButtonDown()
{
    return Platform_IsKeyDown( VK_MBUTTON );
}

bool GLToy_Input_System::Platform_IsMouseRightButtonDown()
{
    return Platform_IsKeyDown( VK_RBUTTON );
}

void GLToy_Input_System::Platform_ChangeLayout()
{
    HKL uKeyLayout = GetKeyboardLayout( 0 );

    // get console key code
    s_uConsoleKeyCode = MapVirtualKeyExA( 0x29, 1, uKeyLayout );

    s_uReturnKeyCode = VK_RETURN;
	s_uEscapeKeyCode = VK_ESCAPE;
    s_uExecuteKeyCode = VK_EXECUTE;
    s_uUpKeyCode = VK_UP;
    s_uDownKeyCode = VK_DOWN;
    s_uLeftKeyCode = VK_LEFT;
    s_uRightKeyCode = VK_RIGHT;
    s_uPageUpKeyCode = VK_PRIOR;
    s_uPageDownKeyCode = VK_NEXT;
    s_uSpaceKeyCode = VK_SPACE;
    s_uCtrlKeyCode = VK_CONTROL;
    s_uAltKeyCode = VK_MENU;

    // this would be the approriate time to change key labels...
}
