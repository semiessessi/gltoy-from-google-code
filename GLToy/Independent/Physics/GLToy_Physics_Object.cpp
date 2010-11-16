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

#include <Core/GLToy_Memory_DebugOff.h>

// Havok
#ifdef GLTOY_USE_HAVOK_PHYSICS

#undef const_cast

// physics
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Dynamics/hkpDynamics.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Physics/Dynamics/World/hkpWorld.h>
#include <Physics/Utilities/CharacterControl/CharacterRigidBody/hkpCharacterRigidBody.h>
#include <Physics/Utilities/CharacterControl/CharacterRigidBody/hkpCharacterRigidBodyListener.h>
#include <Physics/Utilities/CharacterControl/StateMachine/hkpCharacterContext.h>
#include <Physics/Utilities/CharacterControl/StateMachine/hkpDefaultCharacterStates.h>


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

GLToy_OBB GLToy_Physics_Object::GetOBB() const
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

void GLToy_Physics_Object::Update()
{
#ifdef GLTOY_USE_HAVOK_PHYSICS

    if( m_pxHavokContext )
    {
        hkpCharacterRigidBody* pxCharacterRigidBody = reinterpret_cast< hkpCharacterRigidBody* >( m_pHavokData );
        hkpWorld* pxWorld = GLToy_Physics_System::GetHavokWorld();

        if( !pxWorld )
        {
            return;
        }

        if( !pxCharacterRigidBody )
        {
            return;
        }

        pxWorld->lock();
        GLToy_Havok_MarkForWrite();

        
        hkpCharacterInput xInput;
        hkpCharacterOutput xOutput;

        float fDX = m_xMovement[ 0 ];
        float fDZ = m_xMovement[ 1 ];


        xInput.m_inputLR = fDX;
        xInput.m_inputUD = fDZ;

        xInput.m_wantJump = false;
        xInput.m_atLadder = false;

        xInput.m_up.set( 0.0f, 1.0f, 0.0f );
        xInput.m_forward.set( 0.0f, 0.0f, 1.0f );
        xInput.m_forward.normalize3();

        hkStepInfo xStepInfo;
        xStepInfo.m_deltaTime = fPHYSICS_STEP_TIME;
        xStepInfo.m_invDeltaTime = 1.0f / fPHYSICS_STEP_TIME;
        xInput.m_stepInfo = xStepInfo;

        xInput.m_characterGravity.set( 0.0f, -16.0f, 0.0f );
    
        hkpRigidBody* const pxRigidBody = pxCharacterRigidBody->getRigidBody();
    
        GLToy_Assert( pxRigidBody != NULL, "A character's Havok rigid body pointer has been nulled. Get ready for a crash!" );
    
        xInput.m_velocity = pxRigidBody->getLinearVelocity();
        xInput.m_position = pxRigidBody->getPosition();

        pxCharacterRigidBody->checkSupport( xStepInfo, xInput.m_surfaceInfo );

        const hkpCollidable* const pxCollidable = pxRigidBody->getCollidable();

        GLToy_Assert( pxRigidBody != NULL, "A character's Havok collidable pointer has been nulled. We are going to assume they aren't crouching." );

        m_pxHavokContext->update( xInput, xOutput );

        pxCharacterRigidBody->setLinearVelocity( xOutput.m_velocity, fPHYSICS_STEP_TIME );

        GLToy_Havok_UnmarkForWrite();
        pxWorld->unlock();
    }

#endif
}

void GLToy_Physics_Object::LateUpdate()
{
}

void GLToy_Physics_Object::ControlMovement( const GLToy_Vector_2& xMovement )
{
    m_xMovement = xMovement;
}

#include <Core/GLToy_Memory_DebugOn.h>