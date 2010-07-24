/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
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

void GLToy_Physics_Object::Destroy()
{

#ifdef GLTOY_USE_HAVOK_PHYSICS

    if( !m_pxHavokRigidBody )
    {
        return;
    }

    GLToy_Physics_System::GetHavokWorld()->lockAll();
    GLToy_Havok_MarkForWrite();

    GLToy_Physics_System::GetHavokWorld()->removeEntity( m_pxHavokRigidBody );
    m_pxHavokRigidBody = NULL;

    GLToy_Havok_UnmarkForWrite();
    GLToy_Physics_System::GetHavokWorld()->unlockAll();

#endif

}
