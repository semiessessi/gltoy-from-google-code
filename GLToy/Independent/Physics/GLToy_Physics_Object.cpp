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

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Physics/GLToy_Physics_Object.h>

// GLToy
#include <Maths/GLToy_Volume.h>
#include <Physics/GLToy_Physics_System.h>

// Havok
#ifdef GLTOY_USE_HAVOK_PHYSICS

#undef const_cast

// physics
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Dynamics/hkpDynamics.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Physics/Dynamics/World/hkpWorld.h>

#endif

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Physics_Object::SetPosition( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xVelocity )
{
    SetVelocity( xVelocity );
}

void GLToy_Physics_Object::SetVelocity( const GLToy_Vector_3& xVelocity )
{
}

GLToy_OBB GLToy_Physics_Object::GetOBB()
{

#ifdef GLTOY_USE_HAVOK_PHYSICS

    if( !m_pxHavokRigidBody )
    {
        return GLToy_OBB();
    }

    GLToy_Physics_System::GetHavokWorld()->lockReadOnly();
    GLToy_Havok_MarkForRead();

    const hkVector4& xPos = m_pxHavokRigidBody->getPosition();
    const hkQuaternion& xQuat = m_pxHavokRigidBody->getRotation();
    const hkTransform& xTransform = m_pxHavokRigidBody->getTransform();

    const hkpCollidable* const pxCollidable = m_pxHavokRigidBody->getCollidable();
    if( !pxCollidable )
    {
        // return a point at the position
        return GLToy_OBB( GLToy_Vector_3( xPos( 0 ) * fINVERSE_HAVOK_SCALE, xPos( 1 ) * fINVERSE_HAVOK_SCALE, xPos( 2 ) * fINVERSE_HAVOK_SCALE ), GLToy_Maths::IdentityMatrix3, 0.0f, 0.0f, 0.0f );
    }

    const hkpShape* const pxShape = pxCollidable->getShape();

    GLToy_Havok_UnmarkForRead();
    GLToy_Physics_System::GetHavokWorld()->unlockReadOnly();

    hkVector4 xHalfExtents;
    if( pxShape->getType() == HK_SHAPE_BOX )
    {
        const hkpBoxShape* const pxBox = static_cast< const hkpBoxShape* const >( pxShape );
        xHalfExtents = pxBox->getHalfExtents();
    }
    else
    {
        hkAabb xHKAABB;
        pxShape->getAabb( xTransform, 0.01f, xHKAABB );
        xHKAABB.getHalfExtents( xHalfExtents );
    }

    return GLToy_OBB(
        GLToy_Vector_3( xPos( 0 ), xPos( 1 ), xPos( 2 ) ) * fINVERSE_HAVOK_SCALE,
        GLToy_Quaternion( xQuat( 3 ), -xQuat( 0 ), -xQuat( 1 ), -xQuat( 2 ) ).GetOrientationMatrix(),
        xHalfExtents( 0 ) * fINVERSE_HAVOK_SCALE, xHalfExtents( 1 ) * fINVERSE_HAVOK_SCALE, xHalfExtents( 2 ) * fINVERSE_HAVOK_SCALE );

#else
    
    return GLToy_OBB();

#endif

}

GLToy_Vector_3 GLToy_Physics_Object::GetPosition() const
{

#ifdef GLTOY_USE_HAVOK_PHYSICS

    if( !m_pxHavokRigidBody )
    {
		return GLToy_Maths::ZeroVector3;
    }

    GLToy_Physics_System::GetHavokWorld()->lockReadOnly();
    GLToy_Havok_MarkForRead();

    const hkVector4 xPos = m_pxHavokRigidBody->getPosition();

    GLToy_Havok_UnmarkForRead();
    GLToy_Physics_System::GetHavokWorld()->unlockReadOnly();

    return GLToy_Vector_3( xPos( 0 ), xPos( 1 ), xPos( 2 ) ) * fINVERSE_HAVOK_SCALE;

#else
    
    return GLToy_Maths::ZeroVector3;

#endif

}