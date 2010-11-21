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
#include <Input/GLToy_InputHandler.h>

// Win32
#include <windows.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

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
