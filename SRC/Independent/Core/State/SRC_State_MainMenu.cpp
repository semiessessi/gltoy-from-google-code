/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/SRC.h>

// This file's header
#include <Core/State/SRC_State_MainMenu.h>

// GLToy
#include <Core/State/GLToy_State_System.h>
#include <Entity/GLToy_Entity_System.h>
#include <Environment/GLToy_Environment_System.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture.h>
#include <UI/GLToy_UI_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void SRC_State_MainMenu_NewCallback( void* const pData )
{
    SRC::CreateTestEnvironment();
    GLToy_State_System::ChangeState( GLToy_Hash_Constant( "Game" ) );
}

void SRC_State_MainMenu_LoadCallback( void* const pData )
{
    // TODO: create a load state - I think we can actually recycle the .ent files to load/save, we shall see...
}

void SRC_State_MainMenu_EditorCallback( void* const pData )
{
    GLToy_State_System::ChangeState( GLToy_Hash_Constant( "Editor" ) );
}

void SRC_State_MainMenu_QuitCallback( void* const pData )
{
    GLToy_UI_System::ShowQuitDialog();
}

void SRC_State_MainMenu::Initialise()
{
    GLToy_Entity_System::DestroyEntities();
    GLToy_Environment_System::SwitchEnvironment( NULL );

    GLToy_Texture_System::CreateTexture( "Widgets/File.png" );
    GLToy_Texture_System::CreateTexture( "Widgets/Folder.png" );
    GLToy_Texture_System::CreateTexture( "Widgets/Shutdown.png" );

    GLToy_Entity_System::SetRender( false );
    GLToy_Environment_System::SetRender( false );

    GLToy_UI_System::ShowPointer( true );

    const float fBaseX = GLToy_Render::GetMinX() + 0.05f;
    GLToy_UI_System::CreateLabel( "Super Robo Cross", fBaseX, 0.85f );
    
    GLToy_UI_System::CreateImageButton(
        "Widgets/File.png", "New",
        SRC_State_MainMenu_NewCallback,
        fBaseX, 0.45f, 0.2f, 0.2f );

    GLToy_UI_System::CreateImageButton(
        "Widgets/Folder.png", "Load",
        SRC_State_MainMenu_LoadCallback,
        fBaseX, 0.15f, 0.2f, 0.2f );

    GLToy_UI_System::CreateImageButton(
        "Widgets/Settings.png", "Editor",
        SRC_State_MainMenu_EditorCallback,
        fBaseX, -0.15f, 0.2f, 0.2f );
    
    GLToy_UI_System::CreateImageButton(
        "Widgets/Shutdown.png", "Quit",
        SRC_State_MainMenu_QuitCallback,
        fBaseX, -0.85f, 0.2f, 0.2f );
}

void SRC_State_MainMenu::Shutdown()
{
    GLToy_UI_System::ClearWidgets();
}
