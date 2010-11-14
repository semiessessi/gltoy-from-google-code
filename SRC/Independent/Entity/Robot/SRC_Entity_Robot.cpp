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
#include <Entity/Robot/SRC_Entity_Robot.h>

// GLToy
#include <Entity/GLToy_Entity_System.h>
#include <Physics/GLToy_Physics_Object.h>
#include <Physics/GLToy_Physics_System.h>
#include <Render/GLToy_Camera.h>

// SRC
#include <Entity/SRC_EntityTypes.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Array< SRC_Entity_Robot* > SRC_Entity_Robot::s_xInstanceList;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

SRC_Entity_Robot::SRC_Entity_Robot( const GLToy_Hash uHash, const u_int uType )
: GLToy_Parent( uHash, uType )
{
    s_xInstanceList.Append( this );
}

SRC_Entity_Robot::~SRC_Entity_Robot()
{
    s_xInstanceList.RemoveByValue( this );
}

void SRC_Entity_Robot::Spawn( const GLToy_Vector_3& xPosition )
{
    SetPhysicsObject( GLToy_Physics_System::CreateControlledCapsule( GetHash(), xPosition ) );
}

void SRC_Entity_Robot::Render() const
{
    GLToy_Physics_Object* pxPhysicsObject = GetPhysicsObject();
    if( pxPhysicsObject )
    {
        pxPhysicsObject->GetOBB().Render();
    }
}

void SRC_Entity_Robot::SpawnRobot_Console()
{
    static u_int ls_uID = 0;
    SRC_Entity_Robot* const pxRobot = static_cast< SRC_Entity_Robot* >( GLToy_Entity_System::CreateEntity( ( GLToy_String( "Robot " ) + ls_uID ).GetHash(), SRC_ENTITY_ROBOT ) );

    pxRobot->Spawn( GLToy_Camera::GetPosition() );

    ++ls_uID;
}
