/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/Physics.h>

// This file's header
#include <Entity/Physics/Physics_Entity_PhysicsBox.h>

// Physics
#include <Physics/Physics_Physics_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

Physics_Entity_PhysicsBox::Physics_Entity_PhysicsBox( const GLToy_Hash uHash, const u_int uType )
: GLToy_Parent( uHash, uType )
, m_pxPhysicsObject( NULL )
{
}

void Physics_Entity_PhysicsBox::Spawn( const GLToy_AABB& xBB, const GLToy_Vector_3& xVelocity )
{
    m_pxPhysicsObject = Physics_Physics_System::CreatePhysicsBox( GetHash(), xBB, xVelocity );
}

void Physics_Entity_PhysicsBox::Render() const
{
    if( m_pxPhysicsObject )
    {
        // get the obb and render it
        m_pxPhysicsObject->GetOBB().Render();
    }
}