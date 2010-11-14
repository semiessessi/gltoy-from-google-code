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
#include <Entity/SRC_EntityTypes.h>

// GLToy
#include <Entity/GLToy_Entity.h>
#include <Entity/GLToy_Entity_System.h>
#include <Render/GLToy_Camera.h>

// SRC
#include <Entity/Goal/SRC_Entity_Goal.h>
#include <Entity/Object/Fixed/SRC_Entity_Fixed_Cube.h>
#include <Entity/Object/Moveable/SRC_Entity_Moveable_Cube.h>
#include <Entity/Object/Moveable/SRC_Entity_Moveable_Plank.h>
#include <Entity/Object/Moveable/SRC_Entity_Moveable_Square.h>
#include <Entity/Robot/SRC_Entity_Robot.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Entity* SRC_CreateEntity( const GLToy_Hash uHash, const u_int uType )
{
    switch( uType )
    {
        case SRC_ENTITY_ROBOT:              return new SRC_Entity_Robot( uHash, uType );
        case SRC_ENTITY_GOAL:               return new SRC_Entity_Goal( uHash, uType );
        case SRC_ENTITY_MOVEABLE_CUBE:      return new SRC_Entity_Moveable_Cube( uHash, uType );
        case SRC_ENTITY_MOVEABLE_PLANK:     return new SRC_Entity_Moveable_Plank( uHash, uType );
        case SRC_ENTITY_MOVEABLE_SQUARE:    return new SRC_Entity_Moveable_Square( uHash, uType );
        case SRC_ENTITY_FIXED_CUBE:         return new SRC_Entity_Fixed_Cube( uHash, uType );

        default:
        {
            break;
        }
    }

    return NULL;
}

void SRC_Console_SpawnCube()
{
    static u_int ls_uID = 0;

    SRC_Entity_Moveable_Cube* pxCube = static_cast< SRC_Entity_Moveable_Cube* >( GLToy_Entity_System::CreateEntity( ( GLToy_String( "Cube " ) + ls_uID ).GetHash(), SRC_ENTITY_MOVEABLE_CUBE ) );
    pxCube->Spawn( GLToy_Camera::GetPosition() + GLToy_Camera::GetDirection() * 32.0f, GLToy_Camera::GetDirection() * 50.0f );

    ++ls_uID;
}

void SRC_Console_SpawnPlank()
{
    static u_int ls_uID = 0;

    SRC_Entity_Moveable_Plank* pxPlank = static_cast< SRC_Entity_Moveable_Plank* >( GLToy_Entity_System::CreateEntity( ( GLToy_String( "Plank " ) + ls_uID ).GetHash(), SRC_ENTITY_MOVEABLE_PLANK ) );
    pxPlank->Spawn( GLToy_Camera::GetPosition() + GLToy_Camera::GetDirection() * 32.0f, GLToy_Camera::GetDirection() * 50.0f );

    ++ls_uID;
}

void SRC_Console_SpawnSquare()
{
    static u_int ls_uID = 0;

    SRC_Entity_Moveable_Square* pxSquare = static_cast< SRC_Entity_Moveable_Square* >( GLToy_Entity_System::CreateEntity( ( GLToy_String( "Square " ) + ls_uID ).GetHash(), SRC_ENTITY_MOVEABLE_SQUARE ) );
    pxSquare->Spawn( GLToy_Camera::GetPosition() + GLToy_Camera::GetDirection() * 32.0f, GLToy_Camera::GetDirection() * 50.0f );

    ++ls_uID;
}

void SRC_Console_SpawnFixedCube()
{
    static u_int ls_uID = 0;

    SRC_Entity_Fixed_Cube* pxCube = static_cast< SRC_Entity_Fixed_Cube* >( GLToy_Entity_System::CreateEntity( ( GLToy_String( "Fixed Cube " ) + ls_uID ).GetHash(), SRC_ENTITY_FIXED_CUBE ) );
    pxCube->Spawn( GLToy_Camera::GetPosition() + GLToy_Camera::GetDirection() * 32.0f );

    ++ls_uID;
}
