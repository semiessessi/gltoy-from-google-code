/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010 Semi Essessi
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

#ifndef __GLTOY_INPUT_H_
#define __GLTOY_INPUT_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Core/Data Structures/GLToy_Stack.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_InputHandler;

/////////////////////////////////////////////////////////////////////////////////////////////
// E N U M E R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

enum GLTOY_MOUSE_SCROLL
{
	GLTOY_MOUSE_SCROLL_NONE,
	GLTOY_MOUSE_SCROLL_POSITIVE,
	GLTOY_MOUSE_SCROLL_NEGATIVE
};

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Input_System
{
    
public:

    static bool Initialise();

    static void Update();

    static void ChangeLayout();
    static void HandleCharacter( const wchar_t wcCharacter );
    static void HandleKey( const u_int uKey );
    static bool IsKeyDown( const u_int uKey );
    static bool IsMouseLeftButtonDown();
    static bool IsMouseMiddleButtonDown();
    static bool IsMouseRightButtonDown();
    static bool GetDebouncedMouseLeft();
    static bool GetDebouncedMouseMiddle();
    static bool GetDebouncedMouseRight();
    static float GetMouseDeltaX() { return s_fMouseDeltaX; }
    static float GetMouseDeltaY() { return s_fMouseDeltaY; }

	static void SetMouseWheelDelta( int iDelta ) { s_iMouseDelta = iDelta; }
	static GLTOY_MOUSE_SCROLL GetMouseWheelScroll() { return s_eMouseScroll; }

    static GLToy_ForceInline GLToy_InputHandler* GetInputHandler() { return s_xInputStack.Peek(); }
    static void PushInputHandler( GLToy_InputHandler* const pxInputHandler );
    static GLToy_ForceInline void PopInputHandler() { s_xInputStack.Pop(); }

    static GLToy_ForceInline u_int GetConsoleKey() { return s_uConsoleKeyCode; }
	static GLToy_ForceInline u_int GetEscapeKey() { return s_uEscapeKeyCode; }
    static GLToy_ForceInline u_int GetExecuteKey() { return s_uExecuteKeyCode; }
    static GLToy_ForceInline u_int GetReturnKey() { return s_uReturnKeyCode; }
    static GLToy_ForceInline u_int GetUpKey() { return s_uUpKeyCode; }
    static GLToy_ForceInline u_int GetDownKey() { return s_uDownKeyCode; }
    static GLToy_ForceInline u_int GetLeftKey() { return s_uLeftKeyCode; }
    static GLToy_ForceInline u_int GetRightKey() { return s_uRightKeyCode; }
    static GLToy_ForceInline u_int GetPageUpKey() { return s_uPageUpKeyCode; }
    static GLToy_ForceInline u_int GetPageDownKey() { return s_uPageDownKeyCode; }
    static GLToy_ForceInline u_int GetSpaceKey() { return s_uSpaceKeyCode; }
    static GLToy_ForceInline u_int GetCtrlKey() { return s_uCtrlKeyCode; }
    static GLToy_ForceInline u_int GetAltKey() { return s_uAltKeyCode; }

    static bool IsTopInputHandler( const GLToy_InputHandler* const pxInputHandler ) { return pxInputHandler == s_xInputStack.Peek(); }

private:

    static void Platform_ChangeLayout();
    static bool Platform_IsKeyDown( const u_int uKey );
    static bool Platform_IsMouseLeftButtonDown();
    static bool Platform_IsMouseMiddleButtonDown();
    static bool Platform_IsMouseRightButtonDown();
    static void Platform_Update();    
    
    static u_int s_uConsoleKeyCode;
    static u_int s_uEscapeKeyCode;
	static u_int s_uExecuteKeyCode;
    static u_int s_uReturnKeyCode;
    static u_int s_uUpKeyCode;
    static u_int s_uDownKeyCode;
    static u_int s_uLeftKeyCode;
    static u_int s_uRightKeyCode;
    static u_int s_uPageUpKeyCode;
    static u_int s_uPageDownKeyCode;
    static u_int s_uSpaceKeyCode;
    static u_int s_uCtrlKeyCode;
    static u_int s_uAltKeyCode;
    
    static float s_fMouseDeltaX;
    static float s_fMouseDeltaY;
	static GLTOY_MOUSE_SCROLL s_eMouseScroll;
	static int s_iMouseDelta;

    static GLToy_Stack< GLToy_InputHandler* > s_xInputStack;

    static bool s_bMouseLeftDebounced;
    static bool s_bMouseMiddleDebounced;
    static bool s_bMouseRightDebounced;

};

#endif
