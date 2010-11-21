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

// This file's headers
#include <Input/GLToy_InputHandler.h>
#include <Input/GLToy_Input_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// M A C R O S
/////////////////////////////////////////////////////////////////////////////////////////////

#define max( a, b ) ( ( a > b ) ? a : b )
#define min( a, b ) ( ( a < b ) ? a : b )

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

static bool g_abKeyDown[ 1 ];

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
    // TODO: fix
    char acState[ 32 ];
    XQueryKeymap( g_pxDisplay, acState );

    return acState[ uKey >> 3 ] & ( 1 << ( uKey & 0x7 ) );
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
    // set console key code
    s_uConsoleKeyCode = 49;

    s_uReturnKeyCode = 36;
	// TODO: s_uEscapeKeyCode = VK_ESCAPE;
    s_uExecuteKeyCode = 108;
    s_uUpKeyCode = 98;
    s_uDownKeyCode = 104;
    s_uLeftKeyCode = 100;
    s_uRightKeyCode = 102;
    s_uPageUpKeyCode = 99;
    s_uPageDownKeyCode = 105;
    s_uSpaceKeyCode = 65;

    // this would be the approriate time to change key labels...
}

void GLToy_InputHandler::Platform_HandleCharacter( const wchar_t wcCharacter )
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

void GLToy_InputHandler::Platform_HandleKey( const unsigned int uKey )
{
    const unsigned int uInputLength = m_szInput.GetLength();

    switch( uKey ) 
    { 
        case 100: // Left
        {
            m_uCaret = max( m_uCaret - 1, 0 );
            break; 
        }
        case 102: // Right
        {
            m_uCaret = min( m_uCaret + 1, uInputLength );
            break;
        }

        case 97: // Home
        {
            m_uCaret = 0; 
            break;
        }

        case 103: // End
        {
            m_uCaret = uInputLength;
            break;
        }

        case 106: // Insert
        {
            m_bInsert = !m_bInsert; 
            break;
        }

        case 107: // Delete
        { 
            if( m_uCaret < m_szInput.GetLength() )
            {
                m_szInput.RemoveAt( m_uCaret );
            }
            break;
        }

        case 22: // Backspace
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
