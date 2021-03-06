/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010 Semi Essessi, Thomas Young
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
#include <Physics/GLToy_Physics_System.h>

// GLToy
#include <Core/Console/GLToy_Console.h>
#include <Core/Data Structures/GLToy_HashMap.h>
#include <Core/GLToy_Timer.h>
#include <Core/GLToy_UpdateFunctor.h>
#include <Entity/Physics/GLToy_Entity_PhysicsBox.h>
#include <Entity/GLToy_EntityTypes.h>
#include <Entity/GLToy_Entity_System.h>
#include <Environment/GLToy_Environment_Lightmapped.h>
#include <Maths/GLToy_Maths.h>
#include <Maths/GLToy_Plane.h>
#include <Maths/GLToy_Volume.h>
#include <Physics/GLToy_Physics_Controller.h>
#include <Physics/GLToy_Physics_Object.h>
#include <Render/GLToy_Camera.h>

#include <Core/GLToy_Memory_DebugOff.h>

// Bullet
#ifdef GLTOY_USE_BULLET_PHYSICS
// ...
#endif

// Havok
#include <Physics/Platform_GLToy_Havok_Physics.h>
#ifdef GLTOY_USE_HAVOK_PHYSICS

#undef const_cast

// base
#include <Common/Base/hkBase.h>
#include <Common/Base/System/Error/hkDefaultError.h>
#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/Memory/System/Util/hkMemoryInitUtil.h>
#include <Common/Base/Memory/System/hkMemorySystem.h>
#if ( GLTOY_HAVOK_VERSION > 7 )
#include <Common/Base/Memory/Allocator/Malloc/hkMallocAllocator.h>
#endif
#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>
#include <Common/Internal/ConvexHull/hkGeometryUtility.h>

// physics
#include <Physics/Collide/Dispatch/hkpAgentRegisterUtil.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Compound/Tree/MOPP/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Compound/Tree/MOPP/hkpMoppUtility.h>
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesShape.h>
#include <Physics/Collide/Shape/Convex/Triangle/hkpTriangleShape.h>
#include <Physics/Collide/Shape/Convex/Sphere/hkpSphereShape.h>
#include <Physics/Collide/Shape/HeightField/Plane/hkpPlaneShape.h>
#include <Physics/Dynamics/hkpDynamics.h>
#include <Physics/Dynamics/Collide/ContactListener/hkpContactListener.h>
#include <Physics/Dynamics/Entity/hkpEntityListener.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Physics/Dynamics/World/hkpWorld.h>
#include <Physics/Utilities/CharacterControl/CharacterRigidBody/hkpCharacterRigidBody.h>
#include <Physics/Utilities/CharacterControl/CharacterRigidBody/hkpCharacterRigidBodyListener.h>
#include <Physics/Utilities/CharacterControl/StateMachine/hkpCharacterContext.h>
#include <Physics/Utilities/CharacterControl/StateMachine/hkpDefaultCharacterStates.h>
#include <Physics/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>

// keycode - this needs to go somewhere, this seems as good a place as any
#include <Common/Base/KeyCode.cxx>

#define HK_CLASSES_FILE <Common/Serialize/Classlist/hkKeyCodeClasses.h>

#if ( GLTOY_HAVOK_VERSION > 7 )
#undef HK_FEATURE_PRODUCT_AI
#undef HK_FEATURE_PRODUCT_ANIMATION
#undef HK_FEATURE_PRODUCT_CLOTH
#undef HK_FEATURE_PRODUCT_DESTRUCTION
#undef HK_FEATURE_PRODUCT_BEHAVIOR

#define HK_EXCLUDE_LIBRARY_hkgpConvexDecomposition
#define HK_FEATURE_REFLECTION_PHYSICS
#include <Common/Base/Config/hkProductFeatures.cxx>
#else
#include <Common/Serialize/Util/hkBuiltinTypeRegistry.cxx>
#endif

#endif

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_HashMap< GLToy_Physics_Object* > GLToy_Physics_System::s_xPhysicsObjects;
GLToy_Physics_Controller GLToy_Physics_System::s_xDefaultController = GLToy_Physics_Controller( GLToy_Hash_Constant( "DefaultController" ) );

#ifdef GLTOY_USE_HAVOK_PHYSICS

hkpWorld*           g_pxHavokWorld      = NULL;
int                 g_iMaxThreads       = 1;
hkJobThreadPool*    g_pxThreadPool      = NULL;
hkJobQueue*         g_pxJobQueue        = NULL;

#endif

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

#ifdef GLTOY_USE_HAVOK_PHYSICS

static void HK_CALL GLToy_Havok_ErrorReport( const char* szMessage, void* pData )
{
    GLToy_DebugOutput_Release( "Havok error: %s", szMessage );
}

void GLToy_Havok_MarkForRead()
{
#ifdef GLTOY_DEBUG
#ifdef HK_DEBUG

    if( g_pxHavokWorld )
    {
        g_pxHavokWorld->markForRead();
    }

#endif
#endif

}

void GLToy_Havok_MarkForWrite()
{
#ifdef GLTOY_DEBUG
#ifdef HK_DEBUG

    if( g_pxHavokWorld )
    {
        g_pxHavokWorld->markForWrite();
    }

#endif
#endif

}

void GLToy_Havok_UnmarkForRead()
{
#ifdef GLTOY_DEBUG
#ifdef HK_DEBUG

    if( g_pxHavokWorld )
    {
        g_pxHavokWorld->unmarkForRead();
    }

#endif
#endif
}

void GLToy_Havok_UnmarkForWrite()
{
#ifdef GLTOY_DEBUG
#ifdef HK_DEBUG

    if( g_pxHavokWorld )
    {
        g_pxHavokWorld->unmarkForWrite();
    }

#endif
#endif

}

hkpWorld* GLToy_Physics_System::GetHavokWorld()
{
    return g_pxHavokWorld;
}

#endif

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

#ifdef GLTOY_USE_HAVOK_PHYSICS

class GLToy_Havok_PhysicsCollisionListener
: public hkpContactListener
, public hkpEntityListener
{

public:
    
    GLToy_Havok_PhysicsCollisionListener( hkpRigidBody* const pxHavokRigidBody )
    : m_pxHavokRigidBody( pxHavokRigidBody )
    {
    }

    virtual ~GLToy_Havok_PhysicsCollisionListener() {}
    
    virtual void contactPointCallback( const hkpContactPointEvent& xContactPointEvent )
    {
        if( !g_pxHavokWorld )
        {
            return;
        }

        GLToy_Physics_Object* pxObject = GLToy_Physics_System::FindPhysicsObject( m_pxHavokRigidBody->getUserData() );
        hkpRigidBody* const pxRigidBody = xContactPointEvent.getBody( 0 );
        GLToy_Assert( pxRigidBody != NULL, "A Havok has provided bad data into GLToy_Havok_PhysicsCollisionListener::contactPointCallback" );
        if( pxObject && pxRigidBody )
        {
            pxObject->m_xCollisions.Append( GLToy_Physics_ObjectCollision() );
            GLToy_Physics_ObjectCollision& xCollision = pxObject->m_xCollisions.End();

            int iIndex = ( pxRigidBody->getUserData() == pxObject->m_uHash ) ? 1 : 0;
            xCollision.m_uEntityHash = xContactPointEvent.getBody( iIndex )->getUserData();
            xCollision.m_bHitEnvironment = xCollision.m_uEntityHash == GLToy_Hash_Constant( "Environment" );
            xCollision.m_bHitEntity = !xCollision.m_bHitEnvironment;
        }
    }

    virtual void entityDeletedCallback( hkpEntity* pxHavokEntity )
    {
        delete this;
    }

    virtual void entityRemovedCallback( hkpEntity* pxHavokEntity )
    {
        //delete this;
    }

protected:

    hkpRigidBody* m_pxHavokRigidBody;

};

#endif

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Physics_System::Initialise()
{
    GLToy_Console::RegisterCommand( "testbox", TestBox_Console );

#ifdef GLTOY_USE_HAVOK_PHYSICS

    // this initialisation routine is inspired by the ConsoleExampleMt_win32_9-0 example in the Havok SDK
    hkMemoryRouter* pxMemoryRouter = hkMemoryInitUtil::initDefault(
		#if ( GLTOY_HAVOK_VERSION > 7 )
		hkMallocAllocator::m_defaultMallocAllocator, hkMemorySystem::FrameInfo( 500000 )
		#endif
		);
    hkBaseSystem::init( pxMemoryRouter, GLToy_Havok_ErrorReport );

    hkHardwareInfo xHardwareInfo;
    hkGetHardwareInfo( xHardwareInfo );
    g_iMaxThreads = xHardwareInfo.m_numThreads;

    // leave one thread for the renderer and other logic
    hkCpuJobThreadPoolCinfo xThreadPoolCInfo;
    xThreadPoolCInfo.m_numThreads = g_iMaxThreads - 1;
    g_pxThreadPool = new hkCpuJobThreadPool( xThreadPoolCInfo );

    hkJobQueueCinfo xJobQueueInfo;
    xJobQueueInfo.m_jobQueueHwSetup.m_numCpuThreads = g_iMaxThreads;
    g_pxJobQueue = new hkJobQueue( xJobQueueInfo );

    hkpWorldCinfo xHavokWorldInfo;
    xHavokWorldInfo.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_MULTITHREADED;
    xHavokWorldInfo.m_gravity.set( 0.0f, -9.8f, 0.0f );
    xHavokWorldInfo.m_collisionTolerance = 0.001f;
    xHavokWorldInfo.m_expectedMaxLinearVelocity = 40000.0f; // this seems to be the best magic number to prevent penetration.
    xHavokWorldInfo.m_expectedMinPsiDeltaTime = fPHYSICS_STEP_TIME * 0.495f;
    xHavokWorldInfo.setBroadPhaseWorldSize( 1500.0f );
    xHavokWorldInfo.setupSolverInfo( hkpWorldCinfo::SOLVER_TYPE_2ITERS_HARD );
    xHavokWorldInfo.m_solverMicrosteps = 2;
    xHavokWorldInfo.m_contactRestingVelocity = 0.1f;
    xHavokWorldInfo.m_contactPointGeneration = hkpWorldCinfo::CONTACT_POINT_ACCEPT_ALWAYS;
    xHavokWorldInfo.m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_FIX_ENTITY;
    g_pxHavokWorld = new hkpWorld( xHavokWorldInfo );

    g_pxHavokWorld->lock();
    GLToy_Havok_MarkForWrite();

    hkpAgentRegisterUtil::registerAllAgents( g_pxHavokWorld->getCollisionDispatcher() );
    g_pxHavokWorld->registerWithJobQueue( g_pxJobQueue );
    
    GLToy_Havok_UnmarkForWrite();
    g_pxHavokWorld->unlock();

#endif

    return true;
}

void GLToy_Physics_System::Shutdown()
{

    s_xPhysicsObjects.DeleteAll();
    s_xDefaultController.Destroy();

#ifdef GLTOY_USE_HAVOK_PHYSICS
    
    // this is managed for us...
    //delete g_pxHavokWorld;
    g_pxHavokWorld = NULL;
    
    delete g_pxJobQueue;

    g_pxThreadPool->removeReference();
    
    // this is managed for us...
    //delete g_pxThreadPool;
    g_pxThreadPool = NULL;

    hkBaseSystem::quit();
    hkMemoryInitUtil::quit();

#endif

}

void GLToy_Physics_System::Update()
{

#ifdef GLTOY_USE_HAVOK_PHYSICS
    
    if( !g_pxHavokWorld )
    {
        return;
    }

    static float ls_fAccumulatedTimer = 0.0f;
    ls_fAccumulatedTimer += GLToy_Timer::GetFrameTime();

    ResetCollisions();

    // this seems dumb, but it has higher quality and is stupidly fast anyway...
    while( ls_fAccumulatedTimer > fPHYSICS_STEP_TIME )
    {
        s_xPhysicsObjects.Traverse( GLToy_PointerUpdateFunctor< GLToy_Physics_Object* >() );
        s_xDefaultController.Update( fPHYSICS_STEP_TIME );
        g_pxHavokWorld->stepMultithreaded( g_pxJobQueue, g_pxThreadPool, fPHYSICS_STEP_TIME );
        GLToy_QuickFunctorInstance( LateUpdateFunctor, GLToy_Physics_Object*, ppxObject, { ( *ppxObject )->LateUpdate(); }, xFunctor )
        s_xPhysicsObjects.Traverse( xFunctor );
        s_xDefaultController.LateUpdate();

        ls_fAccumulatedTimer -= fPHYSICS_STEP_TIME;
    }

#endif

}

void GLToy_Physics_System::TestBox_Console()
{
    GLToy_String szEntityName;
    szEntityName.SetToFormatString( "Entity%d", GLToy_Entity_System::GetEntityCount() );

    GLToy_Entity_PhysicsBox* pxBox = static_cast< GLToy_Entity_PhysicsBox* >( GLToy_Entity_System::CreateEntity( szEntityName.GetHash(), ENTITY_PHYSICSBOX ) );

    pxBox->Spawn( GLToy_AABB( GLToy_Camera::GetPosition(), 8.0f, 8.0f, 8.0f ) + GLToy_Camera::GetDirection() * 32.0f, GLToy_Camera::GetDirection() * 500.0f );
}

void GLToy_Physics_System::SetDefaultControllerActive( const bool bActive, const GLToy_Vector_3& xPosition )
{
    if( bActive )
    {
        s_xDefaultController.Create( xPosition );
        s_xDefaultController.SetCameraControl( true );
    }
    else
    {
        s_xDefaultController.Destroy();
    }
}

void GLToy_Physics_System::SetDefaultControllerPosition( const GLToy_Vector_3& xVector )
{
    s_xDefaultController.SetPosition( xVector );
}

GLToy_Physics_Object* GLToy_Physics_System::FindPhysicsObject( const GLToy_Hash uHash )
{
    GLToy_Physics_Object** const ppxPhysicsObject = s_xPhysicsObjects.FindData( uHash );

    return ppxPhysicsObject ? *ppxPhysicsObject : NULL;
}

GLToy_Physics_Object* GLToy_Physics_System::CreateControlledCapsule( const GLToy_Hash uHash, const GLToy_Vector_3& xOrigin, const float fScale )
{
    GLToy_Physics_Object* pxPhysicsObject = new GLToy_Physics_Object( uHash );
    s_xPhysicsObjects.AddNode( pxPhysicsObject, uHash );

#ifdef GLTOY_USE_HAVOK_PHYSICS
    
    const GLToy_Vector_3 xScaledPosition = xOrigin * fHAVOK_SCALE;

    hkpWorld* pxWorld = GLToy_Physics_System::GetHavokWorld();

    if( !pxWorld )
    {
        s_xPhysicsObjects.Remove( uHash );
        delete pxPhysicsObject;
        return NULL;
    }

    // Define the shapes for the controller
    hkVector4 xVertexA( 0.0f, 0.0f, 0.1f * fScale );
    hkVector4 xVertexB( 0.0f, 0.0f, -0.1f * fScale );
    hkpShape* pxShape = new hkpCapsuleShape( xVertexA, xVertexB, 1.1f * fScale );

    // Construct a character rigid body
    hkpCharacterRigidBodyCinfo xCharacterRigidBodyCInfo;
    xCharacterRigidBodyCInfo.m_mass = 100.0f;
    xCharacterRigidBodyCInfo.m_shape = pxShape;

    xCharacterRigidBodyCInfo.m_maxForce = 1000.0f;
    xCharacterRigidBodyCInfo.m_up.set( 0.0f, 1.0f, 0.0f );
    xCharacterRigidBodyCInfo.m_position.set( xScaledPosition[ 0 ], xScaledPosition[ 1 ], xScaledPosition[ 2 ] );
    xCharacterRigidBodyCInfo.m_maxSlope = GLToy_Maths::Deg2Rad( 70.0f );
    xCharacterRigidBodyCInfo.m_friction = 0.6f;
    
    hkpCharacterRigidBody* pxHavokRigidBody = new hkpCharacterRigidBody( xCharacterRigidBodyCInfo );

    hkpCharacterRigidBodyListener* pxListener = new hkpCharacterRigidBodyListener();
    pxHavokRigidBody->setListener( pxListener );
    pxListener->removeReference();

    pxWorld->lock();
    GLToy_Havok_MarkForWrite();

    pxWorld->addEntity( pxHavokRigidBody->getRigidBody() );

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

    pxPhysicsObject->SetHavokContextPointer( new hkpCharacterContext( pxManager, HK_CHARACTER_ON_GROUND ) );
    pxManager->removeReference();

    pxPhysicsObject->GetHavokContextPointer()->setCharacterType( hkpCharacterContext::HK_CHARACTER_RIGIDBODY );

    // oh yeah. confusing variable names :)
    hkpRigidBody* pxRigidBody = pxHavokRigidBody->getRigidBody();

    pxRigidBody->setUserData( uHash );
    pxRigidBody->addContactListener( new GLToy_Havok_PhysicsCollisionListener( pxRigidBody ) );

    pxPhysicsObject->SetHavokData( pxHavokRigidBody );
    pxPhysicsObject->SetHavokRigidBodyPointer( pxRigidBody );

    GLToy_Havok_UnmarkForWrite();
    pxWorld->unlock();

#endif

    return pxPhysicsObject;
}

GLToy_Physics_Object* GLToy_Physics_System::CreatePhysicsPlane( const GLToy_Hash uHash, const GLToy_Plane& xPlane )
{

    GLToy_Physics_Object* pxPhysicsObject = new GLToy_Physics_Object( uHash );
    s_xPhysicsObjects.AddNode( pxPhysicsObject, uHash );

#ifdef GLTOY_USE_HAVOK_PHYSICS

    hkpPlaneShape* pxPlane = new hkpPlaneShape(
        hkVector4( xPlane.GetNormal()[ 0 ], xPlane.GetNormal()[ 1 ], xPlane.GetNormal()[ 2 ], xPlane.GetDistance() * fHAVOK_SCALE ),
        hkVector4( 0.0f, 0.0f, 0.0f, 0.0f ),
        hkVector4( 500.0f, 500.0f, 500.0f, 0.0f ) );
    
    hkpRigidBodyCinfo xRigidBodyInfo;

    xRigidBodyInfo.m_motionType = hkpMotion::MOTION_FIXED;
    xRigidBodyInfo.m_shape = pxPlane;
    xRigidBodyInfo.m_position = hkVector4( 0.0f, 0.0f, 0.0f, 0.0f );

    hkpRigidBody* pxRigidBody = new hkpRigidBody( xRigidBodyInfo );

    g_pxHavokWorld->lock();
    GLToy_Havok_MarkForWrite();
    g_pxHavokWorld->addEntity( pxRigidBody );
    pxRigidBody->setQualityType( HK_COLLIDABLE_QUALITY_FIXED );
    pxRigidBody->setUserData( uHash );
    pxRigidBody->addContactListener( new GLToy_Havok_PhysicsCollisionListener( pxRigidBody ) );
    GLToy_Havok_UnmarkForWrite();
    g_pxHavokWorld->unlock();

    pxRigidBody->removeReference();
    pxPlane->removeReference();

#else

    GLToy_Assert( false, "Physics planes require GLTOY_USE_HAVOK_PHYSICS for now..." );

#endif

    return pxPhysicsObject;
}

GLToy_Physics_Object* GLToy_Physics_System::CreatePhysicsEnvironment( const GLToy_Hash uHash, const GLToy_Environment_Lightmapped& xEnvironment )
{

    GLToy_Physics_Object* pxPhysicsObject = new GLToy_Physics_Object( uHash );
    s_xPhysicsObjects.AddNode( pxPhysicsObject, uHash );

#ifdef GLTOY_USE_HAVOK_PHYSICS

    hkArray< hkpShape* > xShapeArray;

    // create the brushes...
    xShapeArray.reserve( xEnvironment.GetBrushCount() );

    for( u_int u = 0; u < xEnvironment.GetBrushCount(); ++u )
    {
        const GLToy_Environment_LightmappedBrush& xBrush = xEnvironment.GetBrush( u );
        if( !xBrush.IsPlayerCollidable() )
        {
            continue;
        }

        hkArray< hkVector4 > xPlanes;
        hkArray< hkVector4 > xVertices;
        xPlanes.reserve( xBrush.m_xPlanes.GetCount() );
        for( u_int v = 0; v < xBrush.m_xPlanes.GetCount(); ++v )
        {
            xPlanes.pushBack( hkVector4(
                ( xBrush.m_xPlanes[ v ].GetNormal()[ 0 ] ),
                ( xBrush.m_xPlanes[ v ].GetNormal()[ 1 ] ),
                ( xBrush.m_xPlanes[ v ].GetNormal()[ 2 ] ),
                ( xBrush.m_xPlanes[ v ].GetDistance() * fHAVOK_SCALE ) ) );
        }

        hkGeometryUtility::createVerticesFromPlaneEquations( xPlanes, xVertices );

        if( xVertices.getSize() == 0 )
        {
            // sometimes there are useless brushes it seems
            // I am probably doing something wrong...
            continue;
        }

        hkStridedVertices xStridedVertices;
        xStridedVertices.m_numVertices = xVertices.getSize();
        xStridedVertices.m_striding = sizeof( hkVector4 );
        xStridedVertices.m_vertices = &( xVertices[ 0 ]( 0 ) );

        xShapeArray.pushBack( new hkpConvexVerticesShape( xStridedVertices, xPlanes ) );
    }

    // old render mesh code - might be useful as a fallback if it can be made stable...
    //// create the triangles...
    //u_int uTriangleCount = 0;
    //for( u_int u = 0; u < xEnvironment.GetFaceCount(); ++u )
    //{
    //    uTriangleCount += xEnvironment.GetVertexCount( u ) - 2;
    //}

    //xShapeArray.reserve( uTriangleCount );

    //for( u_int u = 0; u < xEnvironment.GetFaceCount(); ++u )
    //{
    //    if( xEnvironment.GetVertexCount( u ) > 2 )
    //    {
    //        for( u_int v = 0; v < xEnvironment.GetVertexCount( u ) - 2; ++v )
    //        {
    //            const GLToy_Vector_3 xVertex1 = xEnvironment.GetFaceVertexPosition( u, 0 ) * fHAVOK_SCALE;
    //            const GLToy_Vector_3 xVertex2 = xEnvironment.GetFaceVertexPosition( u, v + 1 ) * fHAVOK_SCALE;
    //            const GLToy_Vector_3 xVertex3 = xEnvironment.GetFaceVertexPosition( u, v + 2 ) * fHAVOK_SCALE;

    //            if( ( ( xVertex1 - xVertex2 ).MagnitudeSquared() < 0.0001f ) || ( ( xVertex2 - xVertex3 ).MagnitudeSquared() < 0.0001f ) || ( ( xVertex3 - xVertex1 ).MagnitudeSquared() < 0.0001f ) )
    //            {
    //                continue;
    //            }

    //            xShapeArray.pushBack(
    //                new hkpTriangleShape(
    //                    hkVector4( xVertex1[ 0 ], xVertex1[ 1 ], xVertex1[ 2 ] ),
    //                    hkVector4( xVertex2[ 0 ], xVertex2[ 1 ], xVertex2[ 2 ] ),
    //                    hkVector4( xVertex3[ 0 ], xVertex3[ 1 ], xVertex3[ 2 ] ) ) );

    //            static_cast< hkpTriangleShape* >( xShapeArray.back() )->setWeldingType( hkpWeldingUtility::WELDING_TYPE_CLOCKWISE );
    //        }
    //    }
    //}

    hkpListShape* pxShapeList = new hkpListShape( &( xShapeArray[ 0 ] ), xShapeArray.getSize() );
    // compile the mopp tree
    // TODO - maybe take this offline and load/store the data in .env files
    //        or maybe implement a hkBvTreeShape derived class for the BSP tree and avoid this altogether
    hkpMoppCompilerInput xMoppCompilerInput;
    xMoppCompilerInput.m_enablePrimitiveSplitting = true;
    xMoppCompilerInput.m_cachePrimitiveExtents = true;

    hkpMoppCode* pxMoppCode = hkpMoppUtility::buildCode( pxShapeList, xMoppCompilerInput );
    hkpMoppBvTreeShape* pxTreeShape = new hkpMoppBvTreeShape( pxShapeList, pxMoppCode );
    
    hkpRigidBodyCinfo xRigidBodyInfo;

    xRigidBodyInfo.m_motionType = hkpMotion::MOTION_FIXED;
    xRigidBodyInfo.m_shape = pxTreeShape;
    xRigidBodyInfo.m_position = hkVector4( 0.0f, 0.0f, 0.0f, 0.0f );

    hkpRigidBody* pxRigidBody = new hkpRigidBody( xRigidBodyInfo );

    g_pxHavokWorld->lock();
    GLToy_Havok_MarkForWrite();
    g_pxHavokWorld->addEntity( pxRigidBody );
    pxRigidBody->setQualityType( HK_COLLIDABLE_QUALITY_FIXED );
    pxRigidBody->setUserData( uHash );
    pxRigidBody->addContactListener( new GLToy_Havok_PhysicsCollisionListener( pxRigidBody ) );
    GLToy_Havok_UnmarkForWrite();
    g_pxHavokWorld->unlock();

    pxRigidBody->removeReference();
    pxTreeShape->removeReference();
    pxMoppCode->removeReference();

#else

    GLToy_Assert( false, "Physics planes require GLTOY_USE_HAVOK_PHYSICS for now..." );

#endif

    return pxPhysicsObject;
}

GLToy_Physics_Object* GLToy_Physics_System::CreatePhysicsBox( const GLToy_Hash uHash, const GLToy_AABB& xAABB, const GLToy_Vector_3& xVelocity )
{

    GLToy_Physics_Object* pxPhysicsObject = new GLToy_Physics_Object( uHash );
    s_xPhysicsObjects.AddNode( pxPhysicsObject, uHash );

#ifdef GLTOY_USE_HAVOK_PHYSICS

    const GLToy_Vector_3 xExtents = xAABB.GetExtents() * fHAVOK_SCALE;
    hkVector4 xHKExtents = hkVector4( xExtents[ 0 ], xExtents[ 1 ], xExtents[ 2 ] );
    hkpBoxShape* pxBox = new hkpBoxShape( xHKExtents, 0 );
    
    hkpRigidBodyCinfo xRigidBodyInfo;

    xRigidBodyInfo.m_motionType = hkpMotion::MOTION_DYNAMIC;
    xRigidBodyInfo.m_shape = pxBox;
    xRigidBodyInfo.m_position = hkVector4( xAABB.GetPosition()[ 0 ] * fHAVOK_SCALE , xAABB.GetPosition()[ 1 ] * fHAVOK_SCALE, xAABB.GetPosition()[ 2 ] * fHAVOK_SCALE );

    xRigidBodyInfo.m_mass = 10.0f;
    hkpMassProperties xMassProperties;
    hkpInertiaTensorComputer::computeBoxVolumeMassProperties( xHKExtents, xRigidBodyInfo.m_mass, xMassProperties );
    xRigidBodyInfo.m_inertiaTensor = xMassProperties.m_inertiaTensor;

    hkpRigidBody* pxRigidBody = new hkpRigidBody( xRigidBodyInfo );

    g_pxHavokWorld->lock();
    GLToy_Havok_MarkForWrite();
    g_pxHavokWorld->addEntity( pxRigidBody );
    pxRigidBody->setQualityType( HK_COLLIDABLE_QUALITY_CRITICAL );
    pxRigidBody->setLinearVelocity( hkVector4( xVelocity[ 0 ] * fHAVOK_SCALE, xVelocity[ 1 ] * fHAVOK_SCALE, xVelocity[ 2 ] * fHAVOK_SCALE ) );
    pxRigidBody->setUserData( uHash );
    pxRigidBody->addContactListener( new GLToy_Havok_PhysicsCollisionListener( pxRigidBody ) );
    GLToy_Havok_UnmarkForWrite();
    g_pxHavokWorld->unlock();

    pxPhysicsObject->SetHavokRigidBodyPointer( pxRigidBody );

    pxRigidBody->removeReference();
    pxBox->removeReference();

#else

    GLToy_Assert( false, "Physics boxes require GLTOY_USE_HAVOK_PHYSICS for now..." );

#endif

    return pxPhysicsObject;
}

GLToy_Physics_Object* GLToy_Physics_System::CreatePhysicsParticle( const GLToy_Hash uHash, const float fRadius, const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xVelocity )
{

    GLToy_Physics_Object* pxPhysicsObject = new GLToy_Physics_Object( uHash );
    s_xPhysicsObjects.AddNode( pxPhysicsObject, uHash );

#ifdef GLTOY_USE_HAVOK_PHYSICS

    hkpSphereShape* pxSphere = new hkpSphereShape( fRadius * fHAVOK_SCALE );
    
    hkpRigidBodyCinfo xRigidBodyInfo;

    xRigidBodyInfo.m_motionType = hkpMotion::MOTION_DYNAMIC;
    xRigidBodyInfo.m_shape = pxSphere;
    xRigidBodyInfo.m_position = hkVector4( xPosition[ 0 ] * fHAVOK_SCALE , xPosition[ 1 ] * fHAVOK_SCALE, xPosition[ 2 ] * fHAVOK_SCALE );

    xRigidBodyInfo.m_mass = 10.0f;
    hkpMassProperties xMassProperties;
    hkpInertiaTensorComputer::computeSphereVolumeMassProperties( fRadius * fHAVOK_SCALE, xRigidBodyInfo.m_mass, xMassProperties );
    xRigidBodyInfo.m_inertiaTensor = xMassProperties.m_inertiaTensor;

    hkpRigidBody* pxRigidBody = new hkpRigidBody( xRigidBodyInfo );

    g_pxHavokWorld->lock();
    GLToy_Havok_MarkForWrite();
    g_pxHavokWorld->addEntity( pxRigidBody );
    pxRigidBody->setQualityType( HK_COLLIDABLE_QUALITY_DEBRIS );
    pxRigidBody->setLinearVelocity( hkVector4( xVelocity[ 0 ] * fHAVOK_SCALE, xVelocity[ 1 ] * fHAVOK_SCALE, xVelocity[ 2 ] * fHAVOK_SCALE ) );
    pxRigidBody->setUserData( uHash );
    GLToy_Havok_UnmarkForWrite();
    g_pxHavokWorld->unlock();

    pxPhysicsObject->SetHavokRigidBodyPointer( pxRigidBody );

    pxRigidBody->removeReference();
    pxSphere->removeReference();

#else

    GLToy_Assert( false, "Physics particles require GLTOY_USE_HAVOK_PHYSICS for now..." );

#endif

    return pxPhysicsObject;
    return NULL;
}

GLToy_Physics_Object* GLToy_Physics_System::CreatePhysicsSphere( const GLToy_Hash uHash, const float fRadius, const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xVelocity )
{

    GLToy_Physics_Object* pxPhysicsObject = new GLToy_Physics_Object( uHash );
    s_xPhysicsObjects.AddNode( pxPhysicsObject, uHash );

#ifdef GLTOY_USE_HAVOK_PHYSICS

    hkpSphereShape* pxSphere = new hkpSphereShape( fRadius * fHAVOK_SCALE );
    
    hkpRigidBodyCinfo xRigidBodyInfo;

    xRigidBodyInfo.m_motionType = hkpMotion::MOTION_DYNAMIC;
    xRigidBodyInfo.m_shape = pxSphere;
    xRigidBodyInfo.m_position = hkVector4( xPosition[ 0 ] * fHAVOK_SCALE , xPosition[ 1 ] * fHAVOK_SCALE, xPosition[ 2 ] * fHAVOK_SCALE );

    xRigidBodyInfo.m_mass = 10.0f;
    hkpMassProperties xMassProperties;
    hkpInertiaTensorComputer::computeSphereVolumeMassProperties( fRadius * fHAVOK_SCALE, xRigidBodyInfo.m_mass, xMassProperties );
    xRigidBodyInfo.m_inertiaTensor = xMassProperties.m_inertiaTensor;

    hkpRigidBody* pxRigidBody = new hkpRigidBody( xRigidBodyInfo );

    g_pxHavokWorld->lock();
    GLToy_Havok_MarkForWrite();
    g_pxHavokWorld->addEntity( pxRigidBody );
    pxRigidBody->setQualityType( HK_COLLIDABLE_QUALITY_CRITICAL );
    pxRigidBody->setLinearVelocity( hkVector4( xVelocity[ 0 ] * fHAVOK_SCALE, xVelocity[ 1 ] * fHAVOK_SCALE, xVelocity[ 2 ] * fHAVOK_SCALE ) );
    pxRigidBody->setUserData( uHash );
    pxRigidBody->addContactListener( new GLToy_Havok_PhysicsCollisionListener( pxRigidBody ) );
    GLToy_Havok_UnmarkForWrite();
    g_pxHavokWorld->unlock();

    pxPhysicsObject->SetHavokRigidBodyPointer( pxRigidBody );

    pxRigidBody->removeReference();
    pxSphere->removeReference();

#else

    GLToy_Assert( false, "Physics spheres require GLTOY_USE_HAVOK_PHYSICS for now..." );

#endif

    return pxPhysicsObject;
    return NULL;
}

GLToy_Physics_Object* GLToy_Physics_System::CreateFixedBox( const GLToy_Hash uHash, const GLToy_AABB& xAABB )
{

    GLToy_Physics_Object* pxPhysicsObject = new GLToy_Physics_Object( uHash );
    s_xPhysicsObjects.AddNode( pxPhysicsObject, uHash );

#ifdef GLTOY_USE_HAVOK_PHYSICS

    const GLToy_Vector_3 xExtents = xAABB.GetExtents() * fHAVOK_SCALE;
    hkVector4 xHKExtents = hkVector4( xExtents[ 0 ], xExtents[ 1 ], xExtents[ 2 ] );
    hkpBoxShape* pxBox = new hkpBoxShape( xHKExtents, 0 );
    
    hkpRigidBodyCinfo xRigidBodyInfo;

    xRigidBodyInfo.m_motionType = hkpMotion::MOTION_FIXED;
    xRigidBodyInfo.m_shape = pxBox;
    xRigidBodyInfo.m_position = hkVector4( xAABB.GetPosition()[ 0 ] * fHAVOK_SCALE , xAABB.GetPosition()[ 1 ] * fHAVOK_SCALE, xAABB.GetPosition()[ 2 ] * fHAVOK_SCALE );

    xRigidBodyInfo.m_mass = 10.0f;
    hkpMassProperties xMassProperties;
    hkpInertiaTensorComputer::computeBoxVolumeMassProperties( xHKExtents, xRigidBodyInfo.m_mass, xMassProperties );
    xRigidBodyInfo.m_inertiaTensor = xMassProperties.m_inertiaTensor;

    hkpRigidBody* pxRigidBody = new hkpRigidBody( xRigidBodyInfo );

    g_pxHavokWorld->lock();
    GLToy_Havok_MarkForWrite();
    g_pxHavokWorld->addEntity( pxRigidBody );
    pxRigidBody->setQualityType( HK_COLLIDABLE_QUALITY_FIXED );
    pxRigidBody->setLinearVelocity( hkVector4( 0.0f, 0.0f, 0.0f ) );
    pxRigidBody->setUserData( uHash );
    pxRigidBody->addContactListener( new GLToy_Havok_PhysicsCollisionListener( pxRigidBody ) );
    GLToy_Havok_UnmarkForWrite();
    g_pxHavokWorld->unlock();

    pxPhysicsObject->SetHavokRigidBodyPointer( pxRigidBody );

    pxRigidBody->removeReference();
    pxBox->removeReference();

#else

    GLToy_Assert( false, "Physics boxes require GLTOY_USE_HAVOK_PHYSICS for now..." );

#endif

    return pxPhysicsObject;
}

void GLToy_Physics_System::DestroyPhysicsObject( const GLToy_Hash uHash )
{
    GLToy_Physics_Object* pxObject = FindPhysicsObject( uHash );
    if( !pxObject )
    {
        return;
    }

    pxObject->Destroy();

    s_xPhysicsObjects.Remove( uHash );
    delete pxObject;
}

void GLToy_Physics_System::ResetCollisions()
{
	GLToy_QuickFunctorInstance(
		CollisionResetFunctor,
		GLToy_Physics_Object*,
		ppxObject,
		( *ppxObject )->ResetCollisions();,
		xFunctor );

    s_xPhysicsObjects.Traverse( xFunctor );
}

GLToy_Physics_Object* GLToy_Physics_System::CreatePhysicsObject( const GLToy_Hash uHash )
{
	 GLToy_Physics_Object* pxPhysicsObject = new GLToy_Physics_Object( uHash );
    s_xPhysicsObjects.AddNode( pxPhysicsObject, uHash );
	return pxPhysicsObject;

}

#ifdef GLTOY_USE_HAVOK_PHYSICS

void GLToy_Physics_System::CreateCollisionListener( hkpRigidBody* pxRigidBody )
{
	pxRigidBody->addContactListener( new GLToy_Havok_PhysicsCollisionListener( pxRigidBody ) );
}

#endif

#include <Core/GLToy_Memory_DebugOn.h>