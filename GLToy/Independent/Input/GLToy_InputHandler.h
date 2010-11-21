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

#ifndef __GLTOY_INPUTHANDLER_H_
#define __GLTOY_INPUTHANDLER_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <String/GLToy_String.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const unsigned int uGLTOY_INPUT_TEXT_BUFFER_SIZE = 1024;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_InputHandler
{
    
    friend class GLToy_Input_System;

public:

    GLToy_InputHandler();
    virtual ~GLToy_InputHandler() {}

    void ClearInputBuffer() { m_szInput.Clear(); m_uCaret = 0; }
    const GLToy_String& GetInputBuffer() const { return m_szInput; }
    void ReplaceInputBuffer( const GLToy_String& xString ) { m_szInput = xString; m_uCaret = m_szInput.GetLength(); }

    void DisableStringInput() { m_bHandleStringInput = false; }
    void EnableStringInput() { m_bHandleStringInput = true; }
    
    GLToy_Inline u_int GetCaret() const { return m_uCaret; }

    bool IsKeyDown( const u_int uKey ) const;
    bool IsMouseLeftButtonDown() const;
    bool IsMouseMiddleButtonDown() const;
    bool IsMouseRightButtonDown() const;
    bool GetDebouncedMouseLeft() const;
    bool GetDebouncedMouseMiddle() const;
    bool GetDebouncedMouseRight() const;
    float GetMouseDeltaX() const;
    float GetMouseDeltaY() const;

protected:

    virtual void HandleCharacter( const wchar_t wcCharacter );
    virtual void HandleKey( const unsigned int uKey );

    virtual void Platform_HandleCharacter( const wchar_t wcCharacter );
    virtual void Platform_HandleKey( const unsigned int uKey );

    u_int m_uCaret;
    bool m_bInsert;
    GLToy_String m_szInput;
    bool m_bHandleStringInput;

};

#endif
