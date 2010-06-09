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

#include <Core/GLToy.h>

// This file's header
#include <Core/State/GLToy_State_EditorFrontEnd.h>

// GLToy
#include <Core/Data Structures/GLToy_HashTree.h>
#include <Core/State/GLToy_State_System.h>
#include <Core/GLToy_Timer.h>
#include <Entity/GLToy_Entity_System.h>
#include <Environment/GLToy_Environment_System.h>
#include <File/GLToy_EnvironmentFile.h>
#include <Input/GLToy_Input.h>
#include <Maths/GLToy_Maths.h>
#include <Render/Font/GLToy_Font.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture.h>
#include <UI/GLToy_UI_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

static u_int g_uCurrentEnvironment = 0;
static GLToy_Font* g_pxFont;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_EditorFrontEnd_KeyInputHandler : public GLToy_KeyInputHandler
{
    
    typedef GLToy_KeyInputHandler GLToy_Parent;

protected:

    virtual void HandleKey( const unsigned int uKey )
    {
        GLToy_HashTree< GLToy_EnvironmentFile* >& xEnvTree =
            GLToy_Environment_System::GetEnvironmentFileTree();

        if( uKey == GLToy_Input_System::GetLeftKey() )
        {
            if( g_uCurrentEnvironment > 0 )
            {
                --g_uCurrentEnvironment;
            }
            else
            {
                g_uCurrentEnvironment = xEnvTree.GetCount() - 1;
            }
        }
        else if( uKey == GLToy_Input_System::GetRightKey() )
        {
            if( g_uCurrentEnvironment < ( xEnvTree.GetCount() - 1 ) )
            {
                ++g_uCurrentEnvironment;
            }
            else
            {
                g_uCurrentEnvironment = 0;
            }
        }
        else if( ( uKey == GLToy_Input_System::GetExecuteKey() )
            || ( uKey == GLToy_Input_System::GetReturnKey() ) )
        {
            GLToy_Environment_System::LoadEnvironmentFile(
                xEnvTree[ g_uCurrentEnvironment ]->GetName() );

            GLToy_State_System::ChangeState( GLToy_Hash_Constant( "Editor" ) );
        }
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_EditorFrontEnd_KeyInputHandler g_xInputHandler;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_State_EditorFrontEnd_NewEnvironmentCallback( void* const pData )
{
    GLToy_Environment_System::CreateTestEnvironment();
    GLToy_State_System::ChangeState( GLToy_Hash_Constant( "Editor" ) );
}

void GLToy_State_EditorFrontEnd_LoadEnvironmentCallback( void* const pData )
{
    GLToy_HashTree< GLToy_EnvironmentFile* >& xEnvTree =
        GLToy_Environment_System::GetEnvironmentFileTree();

    GLToy_Environment_System::LoadEnvironmentFile(
        xEnvTree[ g_uCurrentEnvironment ]->GetName() );

    GLToy_State_System::ChangeState( GLToy_Hash_Constant( "Editor" ) );
}

void GLToy_State_EditorFrontEnd_QuitCallback( void* const pData )
{
    GLToy_UI_System::ShowQuitDialog();
    //GLToy::Quit();
}

void GLToy_State_EditorFrontEnd::Initialise()
{
    GLToy_Entity_System::DestroyEntities();
    GLToy_Environment_System::SwitchEnvironment( NULL );

    g_pxFont = GLToy_Font_System::FindFont( GLToy_Hash_Constant( "FrontEnd" ) );
    if( g_pxFont )
    {
        g_pxFont->Initialise();
    }

    GLToy_Input_System::SetKeyInputHandler( &g_xInputHandler );

    GLToy_Texture_System::CreateTexture( "Widgets/File.png" );
    GLToy_Texture_System::CreateTexture( "Widgets/Folder.png" );
    GLToy_Texture_System::CreateTexture( "Widgets/Shutdown.png" );

    GLToy_Entity_System::SetRender( false );
    GLToy_Environment_System::SetRender( false );

    GLToy_UI_System::ShowPointer( true );

    const float fBaseX = GLToy_Render::GetMinX() + 0.05f;
    GLToy_UI_System::CreateLabel( "GLToy Editor", fBaseX, 0.85f );
    
    GLToy_UI_System::CreateImageButton(
        "Widgets/File.png", "New environment",
        GLToy_State_EditorFrontEnd_NewEnvironmentCallback,
        fBaseX, 0.45f, 0.2f, 0.2f );

    GLToy_UI_System::CreateImageButton(
        "Widgets/Folder.png", "Load environment",
        GLToy_State_EditorFrontEnd_LoadEnvironmentCallback,
        fBaseX, 0.15f, 0.2f, 0.2f );
    
    GLToy_UI_System::CreateImageButton(
        "Widgets/Shutdown.png", "Quit",
        GLToy_State_EditorFrontEnd_QuitCallback,
        fBaseX, -0.85f, 0.2f, 0.2f );
}

void GLToy_State_EditorFrontEnd::Shutdown()
{
    GLToy_UI_System::ClearWidgets();

    GLToy_Input_System::SetKeyInputHandler( NULL );
}

// TODO - use widgets for this
void GLToy_State_EditorFrontEnd::Render2D() const
{
    if( !g_pxFont )
    {
        g_pxFont = GLToy_Font_System::FindFont( GLToy_Hash_Constant( "FrontEnd" ) );
        if( !g_pxFont )
        {
            return;
        }
        
        g_pxFont->Initialise();
    }

    const float fTextX = GLToy_Render::GetMinX() + 0.3f;
    GLToy_HashTree< GLToy_EnvironmentFile* >& xEnvTree =
        GLToy_Environment_System::GetEnvironmentFileTree();
    if( xEnvTree.GetCount() > 0 )
    {
        g_pxFont->RenderString(
            xEnvTree[ g_uCurrentEnvironment ]->GetFilename(),
            fTextX, 0.05f,
            GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );
    }
    else
    {
        g_pxFont->RenderString(
            "No environment files found!",
            fTextX, 0.2f,
            GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );
    }
}
