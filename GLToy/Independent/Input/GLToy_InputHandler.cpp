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

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Input/GLToy_InputHandler.h>

// GLToy
#include <Input/GLToy_Input_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_InputHandler::GLToy_InputHandler()
: m_uCaret( 0 )
, m_bInsert( true )
, m_szInput()
, m_bHandleStringInput( true )
{
}

void GLToy_InputHandler::HandleCharacter( const wchar_t wcCharacter )
{
    if( !m_bHandleStringInput )
    {
        return;
    }

    Platform_HandleCharacter( wcCharacter );
}

void GLToy_InputHandler::HandleKey( const unsigned int uKey )
{
    Platform_HandleKey( uKey );
}

bool GLToy_InputHandler::IsKeyDown( const u_int uKey ) const
{
    if( GLToy_Input_System::IsTopInputHandler( this ) )
    {
        return GLToy_Input_System::IsKeyDown( uKey );
    }

    return false;
}

bool GLToy_InputHandler::IsMouseLeftButtonDown() const
{
    if( GLToy_Input_System::IsTopInputHandler( this ) )
    {
        return GLToy_Input_System::IsMouseLeftButtonDown();
    }

    return false;
}

bool GLToy_InputHandler::IsMouseMiddleButtonDown() const
{
    if( GLToy_Input_System::IsTopInputHandler( this ) )
    {
        return GLToy_Input_System::IsMouseMiddleButtonDown();
    }

    return false;
}

bool GLToy_InputHandler::IsMouseRightButtonDown() const
{
    if( GLToy_Input_System::IsTopInputHandler( this ) )
    {
        return GLToy_Input_System::IsMouseRightButtonDown();
    }

    return false;
}

bool GLToy_InputHandler::GetDebouncedMouseLeft() const
{
    if( GLToy_Input_System::IsTopInputHandler( this ) )
    {
        return GLToy_Input_System::GetDebouncedMouseLeft();
    }

    return false;
}

bool GLToy_InputHandler::GetDebouncedMouseMiddle() const
{
    if( GLToy_Input_System::IsTopInputHandler( this ) )
    {
        return GLToy_Input_System::GetDebouncedMouseMiddle();
    }

    return false;
}

bool GLToy_InputHandler::GetDebouncedMouseRight() const
{
    if( GLToy_Input_System::IsTopInputHandler( this ) )
    {
        return GLToy_Input_System::GetDebouncedMouseRight();
    }

    return false;
}

float GLToy_InputHandler::GetMouseDeltaX() const
{
    if( GLToy_Input_System::IsTopInputHandler( this ) )
    {
        return GLToy_Input_System::GetMouseDeltaX();
    }

    return 0.0f;
}

float GLToy_InputHandler::GetMouseDeltaY() const
{
    if( GLToy_Input_System::IsTopInputHandler( this ) )
    {
        return GLToy_Input_System::GetMouseDeltaY();
    }

    return 0.0f;
}