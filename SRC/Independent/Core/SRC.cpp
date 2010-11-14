/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi, Thomas Young
//
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// This file's header
#include <Core/SRC.h>

// GLToy
#include <Core/Console/GLToy_Console.h>
#include <Core/State/GLToy_State_System.h>
#include <Entity/GLToy_Entity_System.h>
#include <Environment/GLToy_Environment_System.h>
#include "Render/GLToy_Camera.h"

// SRC
#include <Core/State/SRC_State_Game.h>
#include <Core/State/SRC_State_MainMenu.h>
#include <Entity/SRC_EntityTypes.h>
#include <Entity/Robot/SRC_Entity_Robot.h>
#include <Environment/SRC_Environment.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void SRC::CreateTestEnvironment()
{
    GLToy_Entity_System::DestroyEntities();
    GLToy_Environment_System::SwitchEnvironment( GLToy_Environment_System::CreateEnvironmentFromType( uSRC_ENVIRONMENT_TYPE ) );
}

bool SRC::Initialise()
{
    GLToy_State_System::RegisterState( new SRC_State_Game() ); 
    GLToy_State_System::RegisterState( new SRC_State_MainMenu() );

    GLToy_State_System::ChangeState( GLToy_Hash_Constant( "Splash" ) );
    GLToy_State_System::SetNextState( GLToy_Hash_Constant( "MainMenu" ) );

    GLToy::ChangeWindowTitle( "Super Robo Cross" );

    GLToy_Entity_System::SetProjectEntityCreateCallback( SRC_CreateEntity );
    GLToy_Environment_System::SetProjectCreateCallback( SRC_CreateEnvironment );

    GLToy_Console::RegisterCommand( "spawn.cube", SRC_Console_SpawnCube );
    GLToy_Console::RegisterCommand( "spawn.plank", SRC_Console_SpawnPlank );
    GLToy_Console::RegisterCommand( "spawn.square", SRC_Console_SpawnSquare );
    GLToy_Console::RegisterCommand( "spawn.fixedcube", SRC_Console_SpawnFixedCube );
    GLToy_Console::RegisterCommand( "spawn.robot", SRC_Entity_Robot::SpawnRobot_Console );
	GLToy_Console::RegisterCommand( "worldtest", SRC::CreateTestEnvironment );



    return true;
}

void SRC::Shutdown()
{
}

void SRC::Update()
{
}
