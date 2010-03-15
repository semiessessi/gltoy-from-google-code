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

private:

    static void Platform_ChangeLayout();
    static bool Platform_IsKeyDown( const u_int uKey );
    static void Platform_Update();

    static GLToy_KeyInputHandler* s_pxKeyInputHandler;
    
    static u_int s_uConsoleKeyCode;
    static u_int s_uExecuteKeyCode;
    static u_int s_uReturnKeyCode;
    static u_int s_uUpKeyCode;
    static u_int s_uDownKeyCode;
    static u_int s_uLeftKeyCode;
    static u_int s_uRightKeyCode;
    static u_int s_uPageUpKeyCode;
    static u_int s_uPageDownKeyCode;

};

#endif