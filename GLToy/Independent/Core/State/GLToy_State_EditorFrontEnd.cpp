/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010 Semi Essessi
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
static u_int g_uSelection;
static GLToy_Font* g_pxFont;
static float g_fStrobe = 0.0f;

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

        if( uKey == GLToy_Input_System::GetUpKey() )
        {
            if( g_uSelection > 0 )
            {
                --g_uSelection;
            }
        }
        else if( uKey == GLToy_Input_System::GetDownKey() )
        {
            if( g_uSelection < 2 )
            {
                ++g_uSelection;
            }
        }
        else if( uKey == GLToy_Input_System::GetLeftKey() )
        {
            if( g_uSelection == 1 )
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
        }
        else if( uKey == GLToy_Input_System::GetRightKey() )
        {
            if( g_uSelection == 1 )
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
        }
        else if( ( uKey == GLToy_Input_System::GetExecuteKey() )
            || ( uKey == GLToy_Input_System::GetReturnKey() ) )
        {
            switch( g_uSelection )
            {
                case 0:
                {
                    GLToy_Environment_System::CreateTestEnvironment();
                    GLToy_State_System::ChangeState( GLToy_Hash_Constant( "Editor" ) );
                    break;
                }

                case 1:
                {
                    GLToy_Environment_System::LoadEnvironmentFile(
                        xEnvTree[ g_uCurrentEnvironment ]->GetName() );

                    GLToy_State_System::ChangeState( GLToy_Hash_Constant( "Editor" ) );
                    break;
                }

                case 2:
                {
                    GLToy::Quit();
                    break;
                }
            }
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
}

void GLToy_State_EditorFrontEnd::Shutdown()
{
    if( g_pxFont )
    {
        g_pxFont->Shutdown();
    }

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

    const float fTextX = -0.7f;

    g_pxFont->RenderString( "GLToy Editor", -0.95f, 0.85f );

    GLToy_Render::EnableBlending();
    GLToy_Render::SetBlendFunction( BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA );

    GLToy_Texture_System::BindTexture( "Widgets/File.png" );
    
    GLToy_Render::StartSubmittingQuads();
    
    GLToy_Render::SubmitColour(
        ( g_uSelection == 0 )
            ? GLToy_Vector_4( 0.92f + g_fStrobe, 0.92f + g_fStrobe, 0.92f + g_fStrobe, 1.0f )
            : GLToy_Vector_4( 0.84f, 0.84f, 0.84f, 0.84f ) );

    GLToy_Render::SubmitTexturedQuad2D( -0.95f, 0.45f, -0.75f, 0.65f );
    
    GLToy_Render::EndSubmit();

    g_pxFont->RenderString(
        "New environment",
        fTextX, 0.5f,
        ( g_uSelection == 0 )
            ? GLToy_Vector_4( 0.35f + g_fStrobe, 0.8f + g_fStrobe, 0.35f + g_fStrobe, 1.0f )
            : GLToy_Vector_4( 0.7f, 0.7f, 0.7f, 1.0f ) );

    GLToy_Render::EnableBlending();
    GLToy_Render::SetBlendFunction( BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA );

    GLToy_Texture_System::BindTexture( "Widgets/Folder.png" );
    
    GLToy_Render::StartSubmittingQuads();

    GLToy_Render::SubmitColour(
        ( g_uSelection == 1 )
            ? GLToy_Vector_4( 0.92f + g_fStrobe, 0.92f + g_fStrobe, 0.92f + g_fStrobe, 1.0f )
            : GLToy_Vector_4( 0.84f, 0.84f, 0.84f, 0.84f ) );
    
    GLToy_Render::SubmitTexturedQuad2D( -0.95f, 0.15f, -0.75f, 0.35f );
    
    GLToy_Render::EndSubmit();

    GLToy_HashTree< GLToy_EnvironmentFile* >& xEnvTree =
        GLToy_Environment_System::GetEnvironmentFileTree();
    if( xEnvTree.GetCount() > 0 )
    {
        g_pxFont->RenderString(
            "Load environment:",
            fTextX, 0.2f,
            ( g_uSelection == 1 )
                ? GLToy_Vector_4( 0.35f + g_fStrobe, 0.8f + g_fStrobe, 0.35f + g_fStrobe, 1.0f )
                : GLToy_Vector_4( 0.7f, 0.7f, 0.7f, 1.0f ) );

        g_pxFont->RenderString(
            xEnvTree[ g_uCurrentEnvironment ]->GetFilename(),
            fTextX, 0.05f,
            ( g_uSelection == 1 )
                ? GLToy_Vector_4( 0.35f + g_fStrobe, 0.8f + g_fStrobe, 0.35f + g_fStrobe, 1.0f )
                : GLToy_Vector_4( 0.7f, 0.7f, 0.7f, 1.0f ) );
    }
    else
    {
        g_pxFont->RenderString(
            "No environment files found!",
            fTextX, 0.2f,
            ( g_uSelection == 1 )
                ? GLToy_Vector_4( 0.35f + g_fStrobe, 0.8f + g_fStrobe, 0.35f + g_fStrobe, 1.0f )
                : GLToy_Vector_4( 0.7f, 0.7f, 0.7f, 1.0f ) );
    }
    
    GLToy_Render::EnableBlending();
    GLToy_Render::SetBlendFunction( BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA );

    GLToy_Texture_System::BindTexture( "Widgets/Shutdown.png" );
    
    GLToy_Render::StartSubmittingQuads();
    
    GLToy_Render::SubmitColour(
        ( g_uSelection == 2 )
            ? GLToy_Vector_4( 0.92f + g_fStrobe, 0.92f + g_fStrobe, 0.92f + g_fStrobe, 1.0f )
            : GLToy_Vector_4( 0.84f, 0.84f, 0.84f, 0.84f ) );

    GLToy_Render::SubmitTexturedQuad2D( -0.95f, -0.85f, -0.75f, -0.65f );
    
    GLToy_Render::EndSubmit();

    g_pxFont->RenderString(
        "Quit",
        fTextX, -0.8f,
        ( g_uSelection == 2 )
            ? GLToy_Vector_4( 0.35f + g_fStrobe, 0.8f + g_fStrobe, 0.35f + g_fStrobe, 1.0f )
            : GLToy_Vector_4( 0.7f, 0.7f, 0.7f, 1.0f ) );
}

void GLToy_State_EditorFrontEnd::Update()
{
    g_fStrobe = 0.08f * GLToy_Maths::Cos( 7.5f * GLToy_Timer::GetTime() );
}
