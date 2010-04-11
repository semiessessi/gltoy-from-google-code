/////////////////////////////////////////////////////////////////////////////////////////////
//
// �Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of FPSToy.
//
// FPSToy is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// FPSToy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with FPSToy.
// If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/FPSToy.h>

// This file's header
#include <Physics/FPSToy_Physics_Controller.h>

// GLToy
#include <Input/GLToy_Input.h>
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_Render.h>

// FPSToy
#include <Physics/FPSToy_Physics_System.h>

// Havok
#ifdef GLTOY_USE_HAVOK_PHYSICS

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

void FPSToy_Physics_Controller::Create()
{

#ifdef GLTOY_USE_HAVOK_PHYSICS

    hkpWorld* pxWorld = FPSToy_Physics_System::GetHavokWorld();

    if( !pxWorld )
    {
        return;
    }

	// Define the shapes for the controller
	hkVector4 xVertexA( 0.0f, 0.0f, 0.4f );
	hkVector4 xVertexB( 0.0f, 0.0f, -0.4f );
	m_pxStandShape = new hkpCapsuleShape( xVertexA, xVertexB, 0.3f );		
	xVertexA.setZero4();
	m_pxCrouchShape = new hkpCapsuleShape( xVertexA, xVertexB, 0.3f );


	// Construct a character rigid body
	hkpCharacterRigidBodyCinfo xCharacterRigidBodyCInfo;
	xCharacterRigidBodyCInfo.m_mass = 100.0f;
	xCharacterRigidBodyCInfo.m_shape = m_pxStandShape;

	xCharacterRigidBodyCInfo.m_maxForce = 1000.0f;
	xCharacterRigidBodyCInfo.m_up.set( 0.0f, 1.0f, 0.0f );
	xCharacterRigidBodyCInfo.m_position.set( 0.0f, 2.0f, 0.0f );
    xCharacterRigidBodyCInfo.m_maxSlope = GLToy_Maths::Deg2Rad( 70.0f );
    xCharacterRigidBodyCInfo.m_friction = 0.95f;
	
	m_pxHavokRigidBody = new hkpCharacterRigidBody( xCharacterRigidBodyCInfo );

	hkpCharacterRigidBodyListener* pxListener = new hkpCharacterRigidBodyListener();
	m_pxHavokRigidBody->setListener( pxListener );
	pxListener->removeReference();

    pxWorld->lock();
    FPSToy_Havok_MarkForWrite();

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

    FPSToy_Havok_UnmarkForWrite();
    pxWorld->unlock();

#else

    GLToy_Assert( false, "Physics controllers require GLTOY_USE_HAVOK_PHYSICS for now..." );

#endif

}

void FPSToy_Physics_Controller::Destroy()
{

#ifdef GLTOY_USE_HAVOK_PHYSICS
    
    hkpWorld* pxWorld = FPSToy_Physics_System::GetHavokWorld();

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

void FPSToy_Physics_Controller::Update( const float fTimestep )
{

#ifdef GLTOY_USE_HAVOK_PHYSICS

    hkpWorld* pxWorld = FPSToy_Physics_System::GetHavokWorld();

    if( !pxWorld )
    {
        return;
    }

    if( !m_pxHavokRigidBody )
    {
        return;
    }

    const bool bJump = ( !m_bOldJump && GLToy_Input_System::IsKeyDown( GLToy_Input_System::GetSpaceKey() ) );
    m_bOldJump = GLToy_Input_System::IsKeyDown( GLToy_Input_System::GetSpaceKey() );

    pxWorld->lock();
    FPSToy_Havok_MarkForWrite();

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
	//	hkpRigidBody* rb = hkpGetRigidBody( collisionEntries[i].m_partner );
	//	if ( rb != HK_NULL && rb->hasProperty(HK_OBJECT_IS_LADDER) ) 
	//	{
	//		if ( collisionEntries[i].m_agentEntry->m_contactMgr->m_type == hkpContactMgr::TYPE_SIMPLE_CONSTRAINT_CONTACT_MGR )
	//		{
	//			hkpSimpleConstraintContactMgr* mgr = (hkpSimpleConstraintContactMgr*)(collisionEntries[i].m_agentEntry->m_contactMgr);
	//			if (mgr->m_contactConstraintData.getNumContactPoints() > 0)
	//			{
	//				atLadder = true;
	//				hkContactPoint* contactPoints = mgr->m_contactConstraintData.m_atom->getContactPoints();
	//				ladderNorm = contactPoints[0].getNormal();
	//				rb->getPointVelocity( contactPoints[0].getPosition(), ladderVelocity );
	//				break;
	//			}
	//		}
	//	}
	//}

	hkpCharacterInput xInput;
	hkpCharacterOutput xOutput;

    float fDX = 0.0f;
    float fDZ = 0.0f;

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

	xInput.m_characterGravity.set( 0, -16.0f, 0.0f );
	xInput.m_velocity = m_pxHavokRigidBody->getRigidBody()->getLinearVelocity();
	xInput.m_position = m_pxHavokRigidBody->getRigidBody()->getPosition();

	m_pxHavokRigidBody->checkSupport( xStepInfo, xInput.m_surfaceInfo );

    // TODO - implement something similar to this
	//	// Only climb the ladder when the character is either unsupported or wants to go up.
	//	if ( atLadder && ( ( input.m_inputUD < 0 ) || ( input.m_surfaceInfo.m_supportedState != hkpSurfaceInfo::SUPPORTED ) ) )
	//	{
	//		hkVector4 right, ladderUp;
	//		right.setCross( UP, ladderNorm );
	//		ladderUp.setCross( ladderNorm, right );
	//		// Calculate the up vector for the ladder
	//		if (ladderUp.lengthSquared3() > HK_REAL_EPSILON)
	//		{
	//			ladderUp.normalize3();
	//		}

	//		// Reorient the forward vector so it points up along the ladder
	//		input.m_forward.addMul4( -ladderNorm.dot3(input.m_forward), ladderNorm);
	//		input.m_forward.add4( ladderUp );
	//		input.m_forward.normalize3();

	//		input.m_surfaceInfo.m_supportedState = hkpSurfaceInfo::UNSUPPORTED;
	//		input.m_surfaceInfo.m_surfaceNormal = ladderNorm;
	//		input.m_surfaceInfo.m_surfaceVelocity = ladderVelocity;
	//		
	//		HK_SET_OBJECT_COLOR( (hkUlong) m_characterRigidBody->getRigidBody()->getCollidable(), hkColor::rgbFromChars( 255, 255, 0, 100 ) );
	//	}
	//	else
	//	{
	//		// Change character rigid body color according to its state
	//		if( input.m_surfaceInfo.m_supportedState == hkpSurfaceInfo::SUPPORTED )
	//		{
	//			HK_SET_OBJECT_COLOR( (hkUlong) m_characterRigidBody->getRigidBody()->getCollidable(), hkColor::rgbFromChars( 0, 255, 0, 100 ) );
	//		}
	//		else
	//		{
	//			HK_SET_OBJECT_COLOR( (hkUlong) m_characterRigidBody->getRigidBody()->getCollidable(), hkColor::BLUE );
	//		}

	//	}
	//	HK_TIMER_END();
	//}

    m_pxHavokContext->update( xInput, xOutput );

	m_pxHavokRigidBody->setLinearVelocity( xOutput.m_velocity, fTimestep );

    FPSToy_Havok_UnmarkForWrite();
    pxWorld->unlock();

#endif

}

void FPSToy_Physics_Controller::LateUpdate()
{

    // TODO - third person cam here
    const GLToy_Vector_3 xCameraOffset = GLToy_Vector_3( 0.0f, 0.5f, 0.0f );
#ifdef GLTOY_USE_HAVOK_PHYSICS

    hkpWorld* pxWorld = FPSToy_Physics_System::GetHavokWorld();

    if( !pxWorld )
    {
        return;
    }

    // TODO - when merged with GLToy get rid of this rubbish
    // also do something to allow for multiple controllers for
    // AI and perhaps even remote players...
    if( GLToy_Camera::IsControllerCamEnabled() )
    {
        pxWorld->lockReadOnly();
        FPSToy_Havok_MarkForRead();

        GLToy_Camera::SetPosition(
            ( GLToy_Vector_3(
                m_pxHavokRigidBody->getPosition()( 0 ),
                m_pxHavokRigidBody->getPosition()( 1 ),
                m_pxHavokRigidBody->getPosition()( 2 ) ) + xCameraOffset ) * fINVERSE_HAVOK_SCALE );

        FPSToy_Havok_UnmarkForRead();
        pxWorld->unlockReadOnly();
    }

#endif

}