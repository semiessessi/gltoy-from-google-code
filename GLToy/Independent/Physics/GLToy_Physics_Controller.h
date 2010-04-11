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

#ifndef __GLTOY_PHYSICS_CONTROLLER_
#define __GLTOY_PHYSICS_CONTROLLER_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parent
#include <Core/GLToy_Updateable.h>

// so far this include is just needed for the default parameter GLToy_Maths::ZeroVector3

// GLToy
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Physics_Controller
{

public:

    GLToy_Physics_Controller( const GLToy_Hash uHash )
    : m_uHash( uHash )
    , m_bOldJump( false )
#ifdef GLTOY_USE_HAVOK_PHYSICS
    , m_pxHavokRigidBody( NULL )
    , m_pxHavokContext( NULL )
    , m_pxStandShape( NULL )
    , m_pxCrouchShape( NULL )
#endif
    {
    }

    virtual void Create();
    virtual void Destroy();

    virtual void Update( const float fTimestep );
    virtual void LateUpdate();

    void SetPosition( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xVelocity = GLToy_Maths::ZeroVector3 );
    void SetVelocity( const GLToy_Vector_3& xVelocity );

#ifdef GLTOY_USE_HAVOK_PHYSICS

    void SetHavokRigidBodyPointer( class hkpCharacterRigidBody* const pxRigidBody ) { m_pxHavokRigidBody = pxRigidBody; }
    const class hkpCharacterRigidBody* GetHavokRigidBodyPointer() const { return m_pxHavokRigidBody; }
    void SetHavokContextPointer( class hkpCharacterContext* const pxContext ) { m_pxHavokContext = pxContext; }
    const class hkpCharacterContext* GetHavokContextPointer() const { return m_pxHavokContext; }

#endif

protected:

    GLToy_Hash m_uHash;
    bool m_bOldJump;

#ifdef GLTOY_USE_HAVOK_PHYSICS
    
    class hkpCharacterRigidBody* m_pxHavokRigidBody;
    class hkpCharacterContext* m_pxHavokContext;
    class hkpCapsuleShape* m_pxStandShape;
    class hkpCapsuleShape* m_pxCrouchShape;

#endif

};

#endif