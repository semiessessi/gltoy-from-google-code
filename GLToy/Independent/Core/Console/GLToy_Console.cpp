/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010, 2011 Semi Essessi
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
#include <Core/Console/GLToy_Console.h>

// GLToy
#include <Core/Console/GLToy_ConsoleCommand.h>
#include <Core/Console/GLToy_ConsoleVariable.h>
#include <Core/Data Structures/GLToy_HashMap.h>
#include <Core/GLToy_Timer.h>
#include <File/GLToy_TextFile.h>
#include <Input/GLToy_InputHandler.h>
#include <Input/GLToy_Input_System.h>
#include <Maths/GLToy_Maths.h>
#include <Render/Font/GLToy_Font.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const float fGLTOY_CONSOLE_TOP = 1.1f;
static const float fGLTOY_CONSOLE_BOTTOM = 0.0f;
static const float fGLTOY_CONSOLE_SPEED = 3.0f;

static const u_int uGLTOY_CONSOLE_MAX_LOG_LINES = 256;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Console_InputHandler
: public GLToy_InputHandler
{
    
    typedef GLToy_InputHandler GLToy_Parent;

protected:

    virtual void HandleCharacter( const wchar_t wcCharacter )
    {
        if( wcCharacter == L'`' )
        {
            return;
        }

        GLToy_Parent::HandleCharacter( wcCharacter );
        GLToy_Console::HandleCharacter( wcCharacter );
    }

    virtual void HandleKey( const unsigned int uKey )
    {
        GLToy_Parent::HandleKey( uKey );
        GLToy_Console::HandleKey( uKey );
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Array< GLToy_String > GLToy_Console::s_xHistory;
GLToy_Array< GLToy_String > GLToy_Console::s_xLog;

GLToy_Console_InputHandler GLToy_Console::s_xInputHandler;
bool GLToy_Console::s_bConsoleDown = false;
float GLToy_Console::s_fSlideOffset = fGLTOY_CONSOLE_TOP;

u_int GLToy_Console::s_uHistoryPosition = 0;
u_int GLToy_Console::s_uVerticalPosition = 0;

GLToy_Font* GLToy_Console::s_pxFont = NULL;

GLToy_HashMap< GLToy_ConsoleCommand* > GLToy_Console::s_xCommandTree;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Console::ClearLog()
{
    s_xLog.Clear();
}

void GLToy_Console::Print( const GLToy_String& szLine )
{
#ifndef GLTOY_DEMO
    GLToy_String szCopy = szLine;
    while( szCopy.GetLength() > 0 )
    {
        if( s_xLog.GetCount() > uGLTOY_CONSOLE_MAX_LOG_LINES )
        {
            s_xLog.RemoveAt( s_xLog.GetCount() - uGLTOY_CONSOLE_MAX_LOG_LINES );
        }

        s_xLog.Append( szCopy.RemoveFirstLine() );
    }
#endif
}

void GLToy_Console::ExecuteFile( const GLToy_String& szFilename )
{
#ifndef GLTOY_DEMO
    GLToy_TextFile xFile( szFilename );

    if( !xFile.GetSize() )
    {
        return;
    }

    const GLToy_String szSource = xFile.GetString();

    const GLToy_Array< GLToy_String > xLines = szSource.Split( '\n' );
    for( GLToy_ConstIterator< GLToy_String > xIterator; !xIterator.Done( xLines ); xIterator.Next() )
{
const GLToy_String& szLine = xIterator.Current( xLines );
        GLToy_String szCopy = szLine;
        szCopy.TrimTrailingWhiteSpace();
        ExecuteLine( szCopy, false );
    }
#endif
}

void GLToy_Console::ExecuteLine( const GLToy_String& szLine, const bool bStoreInHistory )
{
#ifndef GLTOY_DEMO
    GLToy_String szParameters = szLine;
    const GLToy_String szCommand = szParameters.RemoveFirstWord();

    GLToy_ConsoleCommand* pxCommand = LookUpCommand( szCommand );
    if( pxCommand )
    {
        pxCommand->Execute( szParameters );
        Print( GLToy_String( "> " ) + szLine );
    }
    else
    {
        Print( GLToy_String( "Unrecognised command \"" ) + szCommand + "\"" );
    }

    if( bStoreInHistory )
    {
        s_xHistory.Append( szLine );
        s_uHistoryPosition = s_xHistory.GetCount();
    }
#endif
}

void GLToy_Console::Toggle()
{
#ifndef GLTOY_DEMO
    s_bConsoleDown = !s_bConsoleDown;

    if( s_bConsoleDown )
    {
        GLToy_Input_System::PushInputHandler( &s_xInputHandler );
    }
    else
    {
        GLToy_Input_System::PopInputHandler();
    }
#endif
}

void GLToy_Console::RegisterCommand( const GLToy_String& szName, void ( *pfnFunction )() )
{
#ifndef GLTOY_DEMO
    s_xCommandTree.AddNode( new GLToy_ConsoleCommand_Void( szName, pfnFunction ), szName.GetHash() );
#endif
}

void GLToy_Console::RegisterCommand( const GLToy_String& szName, void ( *pfnFunction )( const bool ) )
{
#ifndef GLTOY_DEMO
    s_xCommandTree.AddNode( new GLToy_ConsoleCommand_Bool( szName, pfnFunction ), szName.GetHash() );
#endif
}

void GLToy_Console::RegisterCommand( const GLToy_String& szName, void ( *pfnFunction )( const u_int ) )
{
#ifndef GLTOY_DEMO
    s_xCommandTree.AddNode( new GLToy_ConsoleCommand_Uint( szName, pfnFunction ), szName.GetHash() );
#endif
}

void GLToy_Console::RegisterCommand( const GLToy_String& szName, void ( *pfnFunction )( const GLToy_String& ) )
{
#ifndef GLTOY_DEMO
    s_xCommandTree.AddNode( new GLToy_ConsoleCommand_String( szName, pfnFunction ), szName.GetHash() );
#endif
}

void GLToy_Console::RegisterVariable( const GLToy_String& szName, bool* pbVariable )
{
#ifndef GLTOY_DEMO
    s_xCommandTree.AddNode( new GLToy_ConsoleVariable_Bool( szName, pbVariable ), szName.GetHash() );
#endif
}

void GLToy_Console::RegisterVariable( const GLToy_String& szName, u_int* puVariable )
{
#ifndef GLTOY_DEMO
    s_xCommandTree.AddNode( new GLToy_ConsoleVariable_Uint( szName, puVariable ), szName.GetHash() );
#endif
}

void GLToy_Console::RegisterVariable( const GLToy_String& szName, float* pfVariable )
{
#ifndef GLTOY_DEMO
    s_xCommandTree.AddNode( new GLToy_ConsoleVariable_Float( szName, pfVariable ), szName.GetHash() );
#endif
}

GLToy_ConsoleCommand* GLToy_Console::LookUpCommand( const GLToy_String& szName )
{
    GLToy_ConsoleCommand** ppxCommand = s_xCommandTree.FindData( szName.GetHash() );
    return ppxCommand ? *ppxCommand : NULL;
}

void GLToy_Console::ListAll()
{
    for( GLToy_Iterator< GLToy_ConsoleCommand* > xIterator; !xIterator.Done( s_xCommandTree ); xIterator.Next() )
    {
        GLToy_ConsoleCommand*& pxCommand = xIterator.Current( s_xCommandTree );
        GLToy_Console::Print( pxCommand->GetName() );
    }
}

void GLToy_Console::HandleCharacter( const wchar_t wcCharacter )
{
}

void GLToy_Console::HandleKey( const unsigned int uKey )
{
#ifndef GLTOY_DEMO
    if( ( uKey == GLToy_Input_System::GetReturnKey() )
        || ( uKey == GLToy_Input_System::GetExecuteKey() ) )
    {
        ExecuteLine( s_xInputHandler.GetInputBuffer() );
        s_xInputHandler.ClearInputBuffer();

    }
    else if( uKey == GLToy_Input_System::GetUpKey() )
    {
        if( ( s_xHistory.GetCount() > 0 ) && ( s_uHistoryPosition > 0 ) )
        {
            --s_uHistoryPosition;
            s_xInputHandler.ReplaceInputBuffer( s_xHistory[ s_uHistoryPosition ] );
        }
    }
    else if( uKey == GLToy_Input_System::GetDownKey() )
    {
        if( ( s_xHistory.GetCount() > 0 ) && ( s_uHistoryPosition < ( s_xHistory.GetCount() - 1 ) ) )
        {
            ++s_uHistoryPosition;
            s_xInputHandler.ReplaceInputBuffer( s_xHistory[ s_uHistoryPosition ] );
        }
        else
        {
            s_uHistoryPosition = s_xHistory.GetCount();
            s_xInputHandler.ClearInputBuffer();
        }
    }
    else if( uKey == GLToy_Input_System::GetPageUpKey() )
    {
        if( ( s_xLog.GetCount() > 0 ) && ( s_uVerticalPosition < ( s_xLog.GetCount() - 1 ) ) )
        {
            ++s_uVerticalPosition;
        }
    }
    else if( uKey == GLToy_Input_System::GetPageDownKey() )
    {
        if( ( s_xLog.GetCount() > 0 ) && ( s_uVerticalPosition > 0 ) )
        {
            --s_uVerticalPosition;
        }
    }
#endif
}

bool GLToy_Console::Initialise()
{
#ifndef GLTOY_DEMO
    GLToy_Font_System::InitialiseFont( "Console" );
    s_pxFont = GLToy_Font_System::FindFont( GLToy_Hash_Constant( "Console" ) );

    RegisterCommand( "echo", Print );
    RegisterCommand( "help", ListAll ); // TODO - a real help function and help...
    RegisterCommand( "listall", ListAll );
    RegisterCommand( "print", Print );
#endif

    return true;
}

void GLToy_Console::Shutdown()
{
#ifndef GLTOY_DEMO
    GLToy_Font_System::ShutdownFont( "Console" );
    s_pxFont = NULL;

    s_xCommandTree.Clear();
#endif
}

void GLToy_Console::Render2D()
{
#ifndef GLTOY_DEMO
    if( !s_bConsoleDown && s_fSlideOffset >= 1.1f )
    {
        return;
    }

    GLToy_Render::SetOrthogonalProjectionMatrix();
    GLToy_Render::SetIdentityViewMatrix();

    GLToy_Render::EnableBlending();
    GLToy_Render::SetBlendFunction( BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA );

    GLToy_Texture_System::BindWhite();

    GLToy_Render::StartSubmittingQuads();
    GLToy_Render::SubmitColour( GLToy_Vector_4( 0.4f, 0.4f, 0.4f, 0.85f ) );
    GLToy_Render::SubmitTexturedQuad2D( GLToy_Render::GetMinX(), 0.0f + s_fSlideOffset, GLToy_Render::GetMaxX(), 1.0f + s_fSlideOffset );
    GLToy_Render::EndSubmit();

    if( !s_pxFont )
    {
        GLToy_Render::DisableBlending();
        return;
    }

    // render input string
    const float fOffset = 0.01f + s_fSlideOffset;
    const float fLineSpace = 1.1f;
    s_pxFont->RenderString( s_xInputHandler.GetInputBuffer(), GLToy_Render::GetMinX(), fOffset );

    // render caret
    GLToy_Render::EnableBlending();
    GLToy_Render::SetBlendFunction( BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA );

    GLToy_Texture_System::BindWhite();

    GLToy_Render::StartSubmittingLines();

    const bool bFlash = ( GLToy_Timer::GetTime() - GLToy_Maths::Floor( GLToy_Timer::GetTime() ) ) > 0.5f;
    GLToy_Render::SubmitColour( bFlash ? GLToy_Maths::ZeroVector4 : GLToy_Vector_4( 0.8f, 0.8f, 0.8f, 1.0f ) );
    const float fCaretX = GLToy_Render::GetMinX() + static_cast< float >( s_xInputHandler.GetCaret() ) * s_pxFont->GetWidth();
    GLToy_Render::SubmitVertex( fCaretX, fOffset, 0.0f );
    GLToy_Render::SubmitVertex( fCaretX, fOffset + s_pxFont->GetHeight(), 0.0f );

    GLToy_Render::EndSubmit();

    // render log
    // TODO - this is some crappy loopage, I'm certain it can be rewritten to something cleaner
    float fYPos = fOffset + s_pxFont->GetHeight() * fLineSpace;
    const u_int uStartPos = s_xLog.GetCount() - s_uVerticalPosition - 1;
    const u_int uEndPos = ( uStartPos < 24 ) ? 0 : uStartPos - 24;
    for( u_int u = uStartPos; ( u >= uEndPos ) && ( u != 0xffffffff ); --u )
    {
        s_pxFont->RenderString( s_xLog[ u ], GLToy_Render::GetMinX(), fYPos );
        fYPos += s_pxFont->GetHeight() * fLineSpace;
    }

    GLToy_Render::DisableBlending();
#endif
}

void GLToy_Console::Update()
{
#ifndef GLTOY_DEMO
    if( s_bConsoleDown )
    {
        if( s_fSlideOffset > fGLTOY_CONSOLE_BOTTOM )
        {
            s_fSlideOffset -= fGLTOY_CONSOLE_SPEED * GLToy_Timer::GetFrameTime();
        }

        if( s_fSlideOffset < fGLTOY_CONSOLE_BOTTOM )
        {
            s_fSlideOffset = fGLTOY_CONSOLE_BOTTOM;
        }
    }
    else
    {
        if( s_fSlideOffset < fGLTOY_CONSOLE_TOP )
        {
            s_fSlideOffset += fGLTOY_CONSOLE_SPEED * GLToy_Timer::GetFrameTime();
        }

        if( s_fSlideOffset > fGLTOY_CONSOLE_TOP )
        {
            s_fSlideOffset = fGLTOY_CONSOLE_TOP;
        }
    }
#endif
}
