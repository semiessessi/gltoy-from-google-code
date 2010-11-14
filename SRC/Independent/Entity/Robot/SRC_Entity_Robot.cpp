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
    SetBB( GLToy_AABB( GLToy_Vector_3( 32.0f, 32.0f, 32.0f ), GLToy_Vector_3( GLToy_Maths::ZeroVector3 ) ) );

    s_xInstanceList.Append( this );
}

SRC_Entity_Robot::~SRC_Entity_Robot()
{
    s_xInstanceList.RemoveByValue( this );
}

void SRC_Entity_Robot::Render() const
{
    GLToy_Parent::RenderOBB();
}

void SRC_Entity_Robot::SpawnRobot_Console()
{
    static u_int ls_uID = 0;
    GLToy_Entity* const pxRobot = GLToy_Entity_System::CreateEntity( ( GLToy_String( "Robot " ) + ls_uID ).GetHash(), SRC_ENTITY_ROBOT );

    pxRobot->SetPosition( GLToy_Camera::GetPosition() );

    ++ls_uID;
}
