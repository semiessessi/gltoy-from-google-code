/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Core/Console/GLToy_Console.h>

// GLToy
#include <Core/Console/GLToy_ConsoleCommand.h>
#include <Core/Console/GLToy_ConsoleVariable.h>
#include <Core/Data Structures/GLToy_HashTree.h>
#include <Core/GLToy_Timer.h>
#include <Input/GLToy_Input.h>
#include <Maths/GLToy_Vector.h>
#include <Render/Font/GLToy_Font.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const float fGLTOY_CONSOLE_TOP = 1.1f;
static const float fGLTOY_CONSOLE_BOTTOM = 0.0f;
static const float fGLTOY_CONSOLE_SPEED = 3.0f;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Console_KeyInputHandler : public GLToy_KeyInputHandler
{
    
    typedef GLToy_KeyInputHandler GLToy_Parent;

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

GLToy_Console_KeyInputHandler GLToy_Console::s_xInputHandler;
bool GLToy_Console::s_bConsoleDown = false;
float GLToy_Console::s_fSlideOffset = fGLTOY_CONSOLE_TOP;

u_int GLToy_Console::s_uHistoryPosition = 0;
u_int GLToy_Console::s_uVerticalPosition = 0;

GLToy_Font* GLToy_Console::s_pxFont = NULL;

GLToy_HashTree< GLToy_ConsoleCommand* > GLToy_Console::s_xCommandTree;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Console::ClearLog()
{
    s_xLog.Clear();
}

void GLToy_Console::Print( const GLToy_String& szLine )
{
    GLToy_String szCopy = szLine;
    while( szCopy.GetLength() > 0 )
    {
        s_xLog.Append( szCopy.RemoveFirstLine() );
    }
}

void GLToy_Console::ExecuteLine( const GLToy_String& szLine, const bool bStoreInHistory )
{
    GLToy_String szParameters = szLine;
    const GLToy_String szCommand = szParameters.RemoveFirstWord();

    GLToy_ConsoleCommand* pxCommand = LookUpCommand( szCommand );
    if( pxCommand )
    {
        pxCommand->Execute( szParameters );
    }

    if( bStoreInHistory )
    {
        s_xHistory.Append( szLine );
        ++s_uHistoryPosition;
    }
}

void GLToy_Console::Toggle()
{
    s_bConsoleDown = !s_bConsoleDown;
    GLToy_Input_System::SetKeyInputHandler( s_bConsoleDown ? &s_xInputHandler : NULL );
}

void GLToy_Console::RegisterCommand( const GLToy_String& szName, void ( *pfnFunction )() )
{
    s_xCommandTree.AddNode( new GLToy_ConsoleCommand_Void( szName, pfnFunction ), szName.GetHash() );
}

void GLToy_Console::RegisterCommand( const GLToy_String& szName, void ( *pfnFunction )( const bool ) )
{
    s_xCommandTree.AddNode( new GLToy_ConsoleCommand_Bool( szName, pfnFunction ), szName.GetHash() );
}

void GLToy_Console::RegisterCommand( const GLToy_String& szName, void ( *pfnFunction )( const u_int ) )
{
    s_xCommandTree.AddNode( new GLToy_ConsoleCommand_Uint( szName, pfnFunction ), szName.GetHash() );
}

void GLToy_Console::RegisterCommand( const GLToy_String& szName, void ( *pfnFunction )( const GLToy_String& ) )
{
    s_xCommandTree.AddNode( new GLToy_ConsoleCommand_String( szName, pfnFunction ), szName.GetHash() );
}

void GLToy_Console::RegisterVariable( const GLToy_String& szName, bool* pbVariable )
{
    s_xCommandTree.AddNode( new GLToy_ConsoleVariable_Bool( szName, pbVariable ), szName.GetHash() );
}

GLToy_ConsoleCommand* GLToy_Console::LookUpCommand( const GLToy_String& szName )
{
    GLToy_ConsoleCommand** ppxCommand = s_xCommandTree.FindData( szName.GetHash() );
    return ppxCommand ? *ppxCommand : NULL;
}

void GLToy_Console::HandleCharacter( const wchar_t wcCharacter )
{
}

void GLToy_Console::HandleKey( const unsigned int uKey )
{
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
}

bool GLToy_Console::Initialise()
{
    GLToy_Font_System::InitialiseFont( "Console" );
    s_pxFont = GLToy_Font_System::LookUpFont( "Console" );

    RegisterCommand( "echo", Print );
    RegisterCommand( "print", Print );

    return true;
}

void GLToy_Console::Shutdown()
{
    GLToy_Font_System::ShutdownFont( "Console" );
    s_pxFont = NULL;
}

void GLToy_Console::Render()
{
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
    GLToy_Render::SubmitTexturedQuad2D( -1.0f, 0.0f + s_fSlideOffset, 1.0f, 1.0f + s_fSlideOffset );
    GLToy_Render::EndSubmit();

    GLToy_Render::DisableBlending();

    if( !s_pxFont )
    {
        return;
    }

    // render input string
    const float fOffset = 0.01f + s_fSlideOffset;
    const float fLineSpace = 1.1f;
    s_pxFont->RenderString( s_xInputHandler.GetInputBuffer(), -1.0f, fOffset );

    float fYPos = fOffset + s_pxFont->GetHeight() * fLineSpace;

    // render log
    // TODO - this is some crappy loopage, I'm certain it can be rewritten to something cleaner
    const u_int uStartPos = s_xLog.GetCount() - s_uVerticalPosition - 1;
    const u_int uEndPos = ( uStartPos < 24 ) ? 0 : uStartPos - 24;
    for( u_int u = uStartPos; ( u >= uEndPos ) && ( u != 0xffffffff ); --u )
    {
        s_pxFont->RenderString( s_xLog[ u ], -1.0f, fYPos );
        fYPos += s_pxFont->GetHeight() * fLineSpace;
    }
}

void GLToy_Console::Update()
{
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

        if( s_fSlideOffset > fGLTOY_CONSOLE_TOP)
        {
            s_fSlideOffset = fGLTOY_CONSOLE_TOP;
        }
    }
}