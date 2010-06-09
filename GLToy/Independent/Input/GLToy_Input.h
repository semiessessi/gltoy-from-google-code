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

#ifndef __GLTOY_INPUT_H_
#define __GLTOY_INPUT_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <String/GLToy_String.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const unsigned int uGLTOY_INPUT_TEXT_BUFFER_SIZE = 1024;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_KeyInputHandler
{
    
    friend class GLToy_Input_System;

public:

    GLToy_KeyInputHandler();
    virtual ~GLToy_KeyInputHandler() {}

    void ClearInputBuffer() { m_szInput.Clear(); m_uCaret = 0; }
    const GLToy_String& GetInputBuffer() const { return m_szInput; }
    void ReplaceInputBuffer( const GLToy_String& xString ) { m_szInput = xString; m_uCaret = m_szInput.GetLength(); }

    void DisableStringInput() { m_bHandleStringInput = false; }
    void EnableStringInput() { m_bHandleStringInput = true; }
    
    GLToy_Inline u_int GetCaret() const { return m_uCaret; }

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
    static float GetMouseDeltaX() { return s_fMouseDeltaX; }
    static float GetMouseDeltaY() { return s_fMouseDeltaY; }
    
    static GLToy_KeyInputHandler* GetKeyInputHandler() { return s_pxKeyInputHandler; }
    static void SetKeyInputHandler( GLToy_KeyInputHandler* pxKeyInputHandler );

    static GLToy_Inline u_int GetConsoleKey() { return s_uConsoleKeyCode; }
    static GLToy_Inline u_int GetExecuteKey() { return s_uExecuteKeyCode; }
    static GLToy_Inline u_int GetReturnKey() { return s_uReturnKeyCode; }
    static GLToy_Inline u_int GetUpKey() { return s_uUpKeyCode; }
    static GLToy_Inline u_int GetDownKey() { return s_uDownKeyCode; }
    static GLToy_Inline u_int GetLeftKey() { return s_uLeftKeyCode; }
    static GLToy_Inline u_int GetRightKey() { return s_uRightKeyCode; }
    static GLToy_Inline u_int GetPageUpKey() { return s_uPageUpKeyCode; }
    static GLToy_Inline u_int GetPageDownKey() { return s_uPageDownKeyCode; }
    static GLToy_Inline u_int GetSpaceKey() { return s_uSpaceKeyCode; }

private:

    static void Platform_ChangeLayout();
    static bool Platform_IsKeyDown( const u_int uKey );
    static bool Platform_IsMouseLeftButtonDown();
    static bool Platform_IsMouseMiddleButtonDown();
    static bool Platform_IsMouseRightButtonDown();
    static void Platform_Update();    
    
    static u_int s_uConsoleKeyCode;
    static u_int s_uExecuteKeyCode;
    static u_int s_uReturnKeyCode;
    static u_int s_uUpKeyCode;
    static u_int s_uDownKeyCode;
    static u_int s_uLeftKeyCode;
    static u_int s_uRightKeyCode;
    static u_int s_uPageUpKeyCode;
    static u_int s_uPageDownKeyCode;
    static u_int s_uSpaceKeyCode;
    
    static float s_fMouseDeltaX;
    static float s_fMouseDeltaY;

    static GLToy_KeyInputHandler* s_pxKeyInputHandler;

};

#endif
