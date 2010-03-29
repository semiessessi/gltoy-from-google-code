/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/Physics.h>

// This file's header
#include <Entity/Physics_EntityTypes.h>

// GLToy
#include <Entity/GLToy_Entity.h>

// Physics
#include <Entity/Physics/Physics_Entity_PhysicsBox.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Entity* Physics_CreateEntity( const GLToy_Hash uHash, const u_int uType )
{
    switch( uType )
    {
        case PHYSICS_ENTITY_PHYSICSBOX:
        {
            return new Physics_Entity_PhysicsBox( uHash, uType );
            break;
        }

        default:
        {
            break;
        }
    }
    return NULL;
}