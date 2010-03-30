/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of Physics.
//
// Physics is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// Physics is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with Physics.
// If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __PHYSICS_PHYSICS_SYSTEM_
#define __PHYSICS_PHYSICS_SYSTEM_

// TODO - there is absolutely no good reason to use a hash tree for physics objects
// ( unless laziness counts as a good reason - which obviously it does )

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// so far this include is just needed for the default parameter GLToy_Maths::ZeroVector3

// GLToy
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_AABB;
template < class T > class GLToy_HashTree;
class GLToy_OBB;
class GLToy_Plane;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class Physics_Physics_Object
{

public:

    Physics_Physics_Object( const GLToy_Hash uHash )
    : m_uHash( uHash )
#ifdef GLTOY_USE_HAVOK_PHYSICS
    , m_pxHavokRigidBody( NULL )
#endif
    {
    }

    void SetPosition( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xVelocity = GLToy_Maths::ZeroVector3 );
    void SetVelocity( const GLToy_Vector_3& xVelocity );

    GLToy_OBB GetOBB();

#ifdef GLTOY_USE_HAVOK_PHYSICS

    void SetHavokRigidBodyPointer( class hkpRigidBody* const pxRigidBody ) { m_pxHavokRigidBody = pxRigidBody; }
    const hkpRigidBody* GetHavokRigidBodyPointer() const { return m_pxHavokRigidBody; }

#endif

protected:

    GLToy_Hash m_uHash;

#ifdef GLTOY_USE_HAVOK_PHYSICS
    
    class hkpRigidBody* m_pxHavokRigidBody;

#endif

};

class Physics_Physics_System
{

public:

    static bool Initialise();
    static void Shutdown();
    
    static void Update();

    static void TestBox_Console();

    static Physics_Physics_Object* CreatePhysicsPlane( const GLToy_Hash uHash, const GLToy_Plane& xPlane );
    static Physics_Physics_Object* CreatePhysicsBox( const GLToy_Hash uHash, const GLToy_AABB& xAABB, const GLToy_Vector_3& xVelocity = GLToy_Maths::ZeroVector3 );
    static Physics_Physics_Object* CreatePhysicsBox( const GLToy_Hash uHash, const GLToy_OBB& xOBB, const GLToy_Vector_3& xVelocity = GLToy_Maths::ZeroVector3 );

private:

    static GLToy_HashTree< Physics_Physics_Object* > s_xPhysicsObjects;

};

#endif