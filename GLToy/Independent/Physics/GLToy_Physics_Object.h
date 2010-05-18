/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
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

#ifndef __GLTOY_PHYSICS_OBJECT_
#define __GLTOY_PHYSICS_OBJECT_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Core/Data Structures/GLToy_Array.h>
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Physics_ObjectCollision
{

public:

    union
    {
        struct
        {
            u_int m_bHitEnvironment : 1;
            u_int m_bHitEntity : 1;

        };

        u_int m_uFlags;
    };

    u_int m_uEntityHash;
};

class GLToy_Physics_Object
{

#ifdef GLTOY_USE_HAVOK_PHYSICS

    friend class GLToy_Havok_PhysicsCollisionListener;

#endif

public:

    GLToy_Physics_Object( const GLToy_Hash uHash )
    : m_uHash( uHash )
    , m_xCollisions()
#ifdef GLTOY_USE_HAVOK_PHYSICS
    , m_pxHavokRigidBody( NULL )
#endif
    {
    }

    virtual ~GLToy_Physics_Object() { m_pxHavokRigidBody = NULL; }

    void SetPosition( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xVelocity = GLToy_Maths::ZeroVector3 );
    void SetVelocity( const GLToy_Vector_3& xVelocity );

    const GLToy_Array< GLToy_Physics_ObjectCollision >& GetCollisions() const { return m_xCollisions; }
    void Destroy();
    void ResetCollisions() { m_xCollisions.Clear(); }

    GLToy_OBB GetOBB();
    GLToy_Vector_3 GetPosition() const;
    GLToy_Inline GLToy_Hash GetHash() const { return m_uHash; }

#ifdef GLTOY_USE_HAVOK_PHYSICS

    void SetHavokRigidBodyPointer( class hkpRigidBody* const pxRigidBody ) { m_pxHavokRigidBody = pxRigidBody; }
    const hkpRigidBody* GetHavokRigidBodyPointer() const { return m_pxHavokRigidBody; }

#endif

protected:

    GLToy_Hash m_uHash;
    GLToy_Array< GLToy_Physics_ObjectCollision > m_xCollisions;

#ifdef GLTOY_USE_HAVOK_PHYSICS
    
    class hkpRigidBody* m_pxHavokRigidBody;

#endif

};


#endif
