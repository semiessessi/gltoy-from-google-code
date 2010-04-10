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

// Win 32
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
    s_uExecuteKeyCode = VK_EXECUTE;
    s_uUpKeyCode = VK_UP;
    s_uDownKeyCode = VK_DOWN;
    s_uLeftKeyCode = VK_LEFT;
    s_uRightKeyCode = VK_RIGHT;
    s_uPageUpKeyCode = VK_PRIOR;
    s_uPageDownKeyCode = VK_NEXT;
    s_uSpaceKeyCode = VK_SPACE;

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

    switch( uKey ) 
    { 
        case VK_LEFT:
        {
            m_uCaret = max( m_uCaret - 1, 0 );
            break; 
        }
        case VK_RIGHT:
        {
            m_uCaret = min( m_uCaret + 1, uInputLength );
            break;
        }

        case VK_HOME:
        {
            m_uCaret = 0; 
            break;
        }

        case VK_END:
        {
            m_uCaret = uInputLength;
            break;
        }

        case VK_INSERT:
        {
            m_bInsert = !m_bInsert; 
            break;
        }

        case VK_DELETE:
        { 
            if( m_uCaret < m_szInput.GetLength() )
            {
                m_szInput.RemoveAt( m_uCaret );
            }
            break;
        }

        case VK_BACK:
        {
            if( m_uCaret > 0 )
            {
                m_szInput.RemoveAt( m_uCaret - 1 );
                --m_uCaret;
            }            
            break;
        }
    }
}