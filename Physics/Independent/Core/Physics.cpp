/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// This file's header
#include <Core/Physics.h>

// GLToy
#include <Core/State/GLToy_State_System.h>
#include <Entity/GLToy_Entity_System.h>
#include <Environment/GLToy_Environment_System.h>

// Physics
#include <Entity/Physics_EntityTypes.h>
#include <Physics/Physics_Physics_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool Physics::Initialise()
{
    GLTOY_INITIALISER_CALL( Physics_Physics_System );

    GLToy_Entity_System::SetProjectEntityCreateCallback( Physics_CreateEntity );
    GLToy_Environment_System::CreateTestEnvironment();

    GLToy_State_System::ChangeState( GLToy_Hash_Constant( "Editor" ) );

    return true;
}

void Physics::Shutdown()
{
    Physics_Physics_System::Shutdown();
}

void Physics::Update()
{
    Physics_Physics_System::Update();
}