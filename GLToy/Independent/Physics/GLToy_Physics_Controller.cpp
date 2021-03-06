/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010 Semi Essessi
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
#include <Physics/GLToy_Physics_Controller.h>

// GLToy
#include <Input/GLToy_Input_System.h>
#include <Physics/GLToy_Physics_System.h>
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_Render.h>

#include <Core/GLToy_Memory_DebugOff.h>

// Havok
#ifdef GLTOY_USE_HAVOK_PHYSICS

#undef const_cast

// physics
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
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

void GLToy_Physics_Controller::Create( const GLToy_Vector_3& xPosition )
{
    if( m_bActive )
    {
        return;
    }

    m_bActive = true;

#ifdef GLTOY_USE_HAVOK_PHYSICS
    
    const GLToy_Vector_3 xScaledPosition = xPosition * fHAVOK_SCALE;

    hkpWorld* pxWorld = GLToy_Physics_System::GetHavokWorld();

    if( !pxWorld )
    {
        return;
    }

    // Define the shapes for the controller
    hkVector4 xVertexA( 0.0f, 0.0f, 0.1f );
    hkVector4 xVertexB( 0.0f, 0.0f, -0.1f );
    m_pxStandShape = new hkpCapsuleShape( xVertexA, xVertexB, 1.1f );        
    xVertexA.setZero4();
    m_pxCrouchShape = new hkpCapsuleShape( xVertexA, xVertexB, 0.85f );


    // Construct a character rigid body
    hkpCharacterRigidBodyCinfo xCharacterRigidBodyCInfo;
    xCharacterRigidBodyCInfo.m_mass = 100.0f;
    xCharacterRigidBodyCInfo.m_shape = m_pxStandShape;

    xCharacterRigidBodyCInfo.m_maxForce = 1000.0f;
    xCharacterRigidBodyCInfo.m_up.set( 0.0f, 1.0f, 0.0f );
    xCharacterRigidBodyCInfo.m_position.set( xScaledPosition[ 0 ], xScaledPosition[ 1 ], xScaledPosition[ 2 ] );
    xCharacterRigidBodyCInfo.m_maxSlope = GLToy_Maths::Deg2Rad( 70.0f );
    xCharacterRigidBodyCInfo.m_friction = 0.6f;
    
    m_pxHavokRigidBody = new hkpCharacterRigidBody( xCharacterRigidBodyCInfo );

    hkpCharacterRigidBodyListener* pxListener = new hkpCharacterRigidBodyListener();
    m_pxHavokRigidBody->setListener( pxListener );
    pxListener->removeReference();

    pxWorld->lock();
    GLToy_Havok_MarkForWrite();

    pxWorld->addEntity( m_pxHavokRigidBody->getRigidBody() );

    hkpCharacterState* pxState;
    hkpCharacterStateManager* pxManager = new hkpCharacterStateManager();

    pxState = new hkpCharacterStateOnGround();
    pxManager->registerState( pxState, HK_CHARACTER_ON_GROUND );
    pxState->removeReference();

    pxState = new hkpCharacterStateInAir();
    pxManager->registerState( pxState, HK_CHARACTER_IN_AIR );
    pxState->removeReference();

    pxState = new hkpCharacterStateJumping();
    pxManager->registerState( pxState, HK_CHARACTER_JUMPING );
    pxState->removeReference();

    pxState = new hkpCharacterStateClimbing();
    pxManager->registerState( pxState, HK_CHARACTER_CLIMBING );
    pxState->removeReference();

    m_pxHavokContext = new hkpCharacterContext( pxManager, HK_CHARACTER_ON_GROUND );
    pxManager->removeReference();

    m_pxHavokContext->setCharacterType( hkpCharacterContext::HK_CHARACTER_RIGIDBODY );

    //m_xPreviousGroundInfo = new hkpSurfaceInfo();
    //m_uFramesInAir = 0;

    GLToy_Havok_UnmarkForWrite();
    pxWorld->unlock();

#else

    GLToy_Assert( false, "Physics controllers require GLTOY_USE_HAVOK_PHYSICS for now..." );

#endif

}

void GLToy_Physics_Controller::Destroy()
{
    if( !m_bActive )
    {
        return;
    }

    m_bActive = false;

#ifdef GLTOY_USE_HAVOK_PHYSICS
    
    hkpWorld* pxWorld = GLToy_Physics_System::GetHavokWorld();

    if( !pxWorld )
    {
        return;
    }

    m_pxHavokRigidBody->removeReference();
    m_pxStandShape->removeReference();
    m_pxCrouchShape->removeReference();

    delete m_pxHavokContext;

    m_pxHavokRigidBody = NULL;

#endif

}

void GLToy_Physics_Controller::Update( const float fTimestep )
{

#ifdef GLTOY_USE_HAVOK_PHYSICS

    hkpWorld* pxWorld = GLToy_Physics_System::GetHavokWorld();

    if( !pxWorld )
    {
        return;
    }

    if( !m_pxHavokRigidBody )
    {
        return;
    }

    const bool bJump = m_bCameraControl && ( !m_bOldJump && GLToy_Input_System::IsKeyDown( GLToy_Input_System::GetSpaceKey() ) );
    const bool bCrouch = m_bCameraControl && GLToy_Input_System::IsKeyDown( 'C' );
    m_bOldJump = m_bCameraControl && GLToy_Input_System::IsKeyDown( GLToy_Input_System::GetSpaceKey() );

    pxWorld->lock();
    GLToy_Havok_MarkForWrite();

    // TODO - implement something similar to this
    ////
    //// Detect ladder
    ////
    //hkBool atLadder = false;
    //// Initialize these so gcc doesn't complain
    //hkVector4 ladderNorm; ladderNorm.setZero4();
    //hkVector4 ladderVelocity; ladderVelocity.setZero4();

    //// Process all collisions points to see if there is a collision at a ladder. This could be done in a callback, however
    //// it is done this way here to ensure compatibility with SPU simulation.
    //hkpLinkedCollidable* coll = m_characterRigidBody->getRigidBody()->getLinkedCollidable();
    //hkArray<struct hkpLinkedCollidable::CollisionEntry> collisionEntriesTmp;
    //coll->getCollisionEntriesSorted(collisionEntriesTmp);
    //const hkArray<struct hkpLinkedCollidable::CollisionEntry>& collisionEntries = collisionEntriesTmp;

    //for ( int i = 0; i < collisionEntries.getSize(); ++i )
    //{
    //    hkpRigidBody* rb = hkpGetRigidBody( collisionEntries[i].m_partner );
    //    if ( rb != HK_NULL && rb->hasProperty(HK_OBJECT_IS_LADDER) ) 
    //    {
    //        if ( collisionEntries[i].m_agentEntry->m_contactMgr->m_type == hkpContactMgr::TYPE_SIMPLE_CONSTRAINT_CONTACT_MGR )
    //        {
    //            hkpSimpleConstraintContactMgr* mgr = (hkpSimpleConstraintContactMgr*)(collisionEntries[i].m_agentEntry->m_contactMgr);
    //            if (mgr->m_contactConstraintData.getNumContactPoints() > 0)
    //            {
    //                atLadder = true;
    //                hkContactPoint* contactPoints = mgr->m_contactConstraintData.m_atom->getContactPoints();
    //                ladderNorm = contactPoints[0].getNormal();
    //                rb->getPointVelocity( contactPoints[0].getPosition(), ladderVelocity );
    //                break;
    //            }
    //        }
    //    }
    //}

    // TODO - the input code belongs in a player class or something
    hkpCharacterInput xInput;
    hkpCharacterOutput xOutput;

    float fDX = 0.0f;
    float fDZ = 0.0f;

    if( m_bCameraControl && GLToy_Camera::IsControllerCamEnabled() )
    {
        if( GLToy_Input_System::IsKeyDown( 'W' )
            || GLToy_Input_System::IsKeyDown( GLToy_Input_System::GetUpKey() ) )
        {
            fDZ -= 0.5f;
        }

        if( GLToy_Input_System::IsKeyDown( 'S' )
            || GLToy_Input_System::IsKeyDown( GLToy_Input_System::GetDownKey() ) )
        {
            fDZ += 0.5f;
        }

        if( GLToy_Input_System::IsKeyDown( 'A' ) )
        {
            fDX -= 0.5f;
        }

        if( GLToy_Input_System::IsKeyDown( 'D' ) )
        {
            fDX += 0.5f;
        }
    }
    else
    {
        fDX = m_xMovement[ 0 ];
        fDZ = m_xMovement[ 1 ];
    }

    xInput.m_inputLR = fDX;
    xInput.m_inputUD = fDZ;

    xInput.m_wantJump = bJump;
    xInput.m_atLadder = false;

    xInput.m_up.set( 0.0f, 1.0f, 0.0f );
    xInput.m_forward.set( GLToy_Camera::GetDirection()[ 0 ], 0.0f, GLToy_Camera::GetDirection()[ 2 ] );
    xInput.m_forward.normalize3();

    hkStepInfo xStepInfo;
    xStepInfo.m_deltaTime = fTimestep;
    xStepInfo.m_invDeltaTime = 1.0f / fTimestep;
    xInput.m_stepInfo = xStepInfo;

    xInput.m_characterGravity.set( 0.0f, -16.0f, 0.0f );
    
    hkpRigidBody* const pxRigidBody = m_pxHavokRigidBody->getRigidBody();
    
    GLToy_Assert( pxRigidBody != NULL, "A character's Havok rigid body pointer has been nulled. Get ready for a crash!" );
    
    xInput.m_velocity = pxRigidBody->getLinearVelocity();
    xInput.m_position = pxRigidBody->getPosition();

    m_pxHavokRigidBody->checkSupport( xStepInfo, xInput.m_surfaceInfo );

    // TODO - implement something similar to this
    //    // Only climb the ladder when the character is either unsupported or wants to go up.
    //    if ( atLadder && ( ( input.m_inputUD < 0 ) || ( input.m_surfaceInfo.m_supportedState != hkpSurfaceInfo::SUPPORTED ) ) )
    //    {
    //        hkVector4 right, ladderUp;
    //        right.setCross( UP, ladderNorm );
    //        ladderUp.setCross( ladderNorm, right );
    //        // Calculate the up vector for the ladder
    //        if (ladderUp.lengthSquared3() > HK_REAL_EPSILON)
    //        {
    //            ladderUp.normalize3();
    //        }

    //        // Reorient the forward vector so it points up along the ladder
    //        input.m_forward.addMul4( -ladderNorm.dot3(input.m_forward), ladderNorm);
    //        input.m_forward.add4( ladderUp );
    //        input.m_forward.normalize3();

    //        input.m_surfaceInfo.m_supportedState = hkpSurfaceInfo::UNSUPPORTED;
    //        input.m_surfaceInfo.m_surfaceNormal = ladderNorm;
    //        input.m_surfaceInfo.m_surfaceVelocity = ladderVelocity;
    //    }

    const hkpCollidable* const pxCollidable = pxRigidBody->getCollidable();

    GLToy_Assert( pxRigidBody != NULL, "A character's Havok collidable pointer has been nulled. We are going to assume they aren't crouching." );

    const bool bIsCrouched = pxCollidable ? ( pxRigidBody->getCollidable()->getShape() == m_pxCrouchShape ) : false;

    if ( bIsCrouched && !bCrouch )
    {
        pxRigidBody->setShape( m_pxStandShape );
    }

    if ( !bIsCrouched && bCrouch )
    {
        pxRigidBody->setShape( m_pxCrouchShape );
    }

    m_pxHavokContext->update( xInput, xOutput );

    m_pxHavokRigidBody->setLinearVelocity( xOutput.m_velocity, fTimestep );

    GLToy_Havok_UnmarkForWrite();
    pxWorld->unlock();

#endif

}

void GLToy_Physics_Controller::LateUpdate()
{

    // TODO - third person cam here
    const bool bCrouch = GLToy_Input_System::IsKeyDown( 'C' );
    const GLToy_Vector_3 xCameraOffset = GLToy_Vector_3( 0.0f, bCrouch ? 0.4f : 0.8f, 0.0f );
#ifdef GLTOY_USE_HAVOK_PHYSICS

    hkpWorld* pxWorld = GLToy_Physics_System::GetHavokWorld();

    if( !pxWorld )
    {
        return;
    }

    if( m_bCameraControl && GLToy_Camera::IsControllerCamEnabled() && m_pxHavokRigidBody )
    {
        pxWorld->lockReadOnly();
        GLToy_Havok_MarkForRead();

        GLToy_Camera::SetPosition(
            ( GLToy_Vector_3(
                m_pxHavokRigidBody->getPosition()( 0 ),
                m_pxHavokRigidBody->getPosition()( 1 ),
                m_pxHavokRigidBody->getPosition()( 2 ) ) + xCameraOffset ) * fINVERSE_HAVOK_SCALE );

        GLToy_Havok_UnmarkForRead();
        pxWorld->unlockReadOnly();
    }

#endif

}

void GLToy_Physics_Controller::SetPosition( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xVelocity )
{

    Destroy();
    Create( xPosition );

    SetVelocity( xVelocity );

}

void GLToy_Physics_Controller::SetVelocity( const GLToy_Vector_3& xVelocity )
{
}

#include <Core/GLToy_Memory_DebugOn.h>
