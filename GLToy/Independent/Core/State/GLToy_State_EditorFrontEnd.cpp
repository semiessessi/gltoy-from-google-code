/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Core/State/GLToy_State_EditorFrontEnd.h>

// GLToy
#include <Core/Data Structures/GLToy_HashTree.h>
#include <Core/State/GLToy_State_System.h>
#include <Entity/GLToy_Entity_System.h>
#include <Environment/GLToy_Environment_System.h>
#include <File/GLToy_EnvironmentFile.h>
#include <Input/GLToy_Input.h>
#include <Render/Font/GLToy_Font.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

static u_int g_uCurrentEnvironment = 0;
static u_int g_uSelection;
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
        GLToy_HashTree< GLToy_EnvironmentFile* >& xEnvTree = GLToy_Environment_System::GetEnvironmentFileTree();

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
                    GLToy_Environment_System::LoadEnvironmentFile( xEnvTree[ g_uCurrentEnvironment ]->GetName() );
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
}

void GLToy_State_EditorFrontEnd::Shutdown()
{
    if( g_pxFont )
    {
        g_pxFont->Shutdown();
    }

    GLToy_Input_System::SetKeyInputHandler( NULL );
}

void GLToy_State_EditorFrontEnd::Render2D() const
{
    if( g_pxFont )
    {
        g_pxFont->RenderString( "New environment", -1.0f, 0.8f, ( g_uSelection == 0 ) ? GLToy_Vector_4( 0.35f, 0.8f, 0.35f, 1.0f ) : GLToy_Vector_4( 0.7f, 0.7f, 0.7f, 1.0f ) );
    }

    GLToy_HashTree< GLToy_EnvironmentFile* >& xEnvTree = GLToy_Environment_System::GetEnvironmentFileTree();
    if( xEnvTree.GetCount() > 0 )
    {
        if( g_pxFont )
        {
            g_pxFont->RenderString( "Load environment:", -1.0f, 0.65f, ( g_uSelection == 1 ) ? GLToy_Vector_4( 0.35f, 0.8f, 0.35f, 1.0f ) : GLToy_Vector_4( 0.7f, 0.7f, 0.7f, 1.0f ) );
            g_pxFont->RenderString( xEnvTree[ g_uCurrentEnvironment ]->GetFilename(), -1.0f, 0.5f, ( g_uSelection == 1 ) ? GLToy_Vector_4( 0.35f, 0.8f, 0.35f, 1.0f ) : GLToy_Vector_4( 0.7f, 0.7f, 0.7f, 1.0f ) );
        }
    }
    else
    {
        if( g_pxFont )
        {
            g_pxFont->RenderString( "No environment files found!", -1.0f, 0.8f, ( g_uSelection == 1 ) ? GLToy_Vector_4( 0.35f, 0.8f, 0.35f, 1.0f ) : GLToy_Vector_4( 0.7f, 0.7f, 0.7f, 1.0f ) );
        }
    }

    if( g_pxFont )
    {
        if( g_pxFont )
        {
            g_pxFont->RenderString( "Quit", -1.0f, -1.0f, ( g_uSelection == 2 ) ? GLToy_Vector_4( 0.35f, 0.8f, 0.35f, 1.0f ) : GLToy_Vector_4( 0.7f, 0.7f, 0.7f, 1.0f ) );
        }
    }
}

void GLToy_State_EditorFrontEnd::Update()
{
}
