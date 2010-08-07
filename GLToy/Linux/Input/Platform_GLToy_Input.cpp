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

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Input_System::Platform_Update()
{

    // TODO: Mouse movement update for Linux

    static bool bFirst = true;
    if( bFirst )
    {
        s_fMouseDeltaX = 0.0f;
        s_fMouseDeltaY = 0.0f;
        bFirst = false;
    }
}

bool GLToy_Input_System::Platform_IsKeyDown( const u_int uKey )
{
    // TODO: Key state detection for Linux
    return false;
}

bool GLToy_Input_System::Platform_IsMouseLeftButtonDown()
{
	return false;
    //return Platform_IsKeyDown( VK_LBUTTON );
}

bool GLToy_Input_System::Platform_IsMouseMiddleButtonDown()
{
    return false;
    //return Platform_IsKeyDown( VK_MBUTTON );
}

bool GLToy_Input_System::Platform_IsMouseRightButtonDown()
{
    return false;
    //return Platform_IsKeyDown( VK_RBUTTON );
}

void GLToy_Input_System::Platform_ChangeLayout()
{
    // TODO: Implement for Linux
    //// get console key code
    //s_uConsoleKeyCode = MapVirtualKeyExA( 0x29, 1, uKeyLayout );

    //s_uReturnKeyCode = VK_RETURN;
    //s_uExecuteKeyCode = VK_EXECUTE;
    //s_uUpKeyCode = VK_UP;
    //s_uDownKeyCode = VK_DOWN;
    //s_uLeftKeyCode = VK_LEFT;
    //s_uRightKeyCode = VK_RIGHT;
    //s_uPageUpKeyCode = VK_PRIOR;
    //s_uPageDownKeyCode = VK_NEXT;
    //s_uSpaceKeyCode = VK_SPACE;

    // this would be the approriate time to change key labels...
}

void GLToy_KeyInputHandler::Platform_HandleCharacter( const wchar_t wcCharacter )
{
    switch ( wcCharacter ) 
    { 
        case 0x08: // backspace
        case 0x0A: // linefeed
        case 0x1B: // escape
        case 0x09: // tab
        case 0x0D: // carriage return
        {
            break;
        }
        
        default: 
        {
            m_szInput.InsertAt( m_uCaret, wcCharacter );
            ++m_uCaret;

            break;
        }
    }
}

void GLToy_KeyInputHandler::Platform_HandleKey( const unsigned int uKey )
{
    const unsigned int uInputLength = m_szInput.GetLength();

    // TODO: Implement for Linux
    //switch( uKey ) 
    //{ 
    //    case VK_LEFT:
    //    {
    //        m_uCaret = max( m_uCaret - 1, 0 );
    //        break; 
    //    }
    //    case VK_RIGHT:
    //    {
    //        m_uCaret = min( m_uCaret + 1, uInputLength );
    //        break;
    //    }

    //    case VK_HOME:
    //    {
    //        m_uCaret = 0; 
    //        break;
    //    }

    //    case VK_END:
    //    {
    //        m_uCaret = uInputLength;
    //        break;
    //    }

    //    case VK_INSERT:
    //    {
    //        m_bInsert = !m_bInsert; 
    //        break;
    //    }

    //    case VK_DELETE:
    //    { 
    //        if( m_uCaret < m_szInput.GetLength() )
    //        {
    //            m_szInput.RemoveAt( m_uCaret );
    //        }
    //        break;
    //    }

    //    case VK_BACK:
    //    {
    //        if( m_uCaret > 0 )
    //        {
    //            m_szInput.RemoveAt( m_uCaret - 1 );
    //            --m_uCaret;
    //        }            
    //        break;
    //    }
    //}
}
