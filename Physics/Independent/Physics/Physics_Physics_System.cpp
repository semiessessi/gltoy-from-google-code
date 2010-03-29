/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/Physics.h>

// This file's header
#include <Physics/Physics_Physics_System.h>

// GLToy
#include <Core/Console/GLToy_Console.h>
#include <Core/Data Structures/GLToy_HashTree.h>
#include <Core/GLToy_Timer.h>
#include <Entity/GLToy_Entity_System.h>
#include <Maths/GLToy_Maths.h>
#include <Maths/GLToy_Plane.h>
#include <Maths/GLToy_Volume.h>
#include <Render/GLToy_Camera.h>

// Physics
#include <Entity/Physics_EntityTypes.h>
#include <Entity/Physics/Physics_Entity_PhysicsBox.h>

// Havok
#include <Physics/Platform_GLToy_Havok_Physics.h>
#ifdef GLTOY_USE_HAVOK_PHYSICS

// base
#include <Common/Base/hkBase.h>
#include <Common/Base/System/Error/hkDefaultError.h>
#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/Memory/System/Util/hkMemoryInitUtil.h>
#include <Common/Base/Memory/System/hkMemorySystem.h>
#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>

// physics
#include <Physics/Collide/Dispatch/hkpAgentRegisterUtil.h>	
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Collide/Shape/HeightField/Plane/hkpPlaneShape.h>
#include <Physics/Dynamics/hkpDynamics.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Physics/Dynamics/World/hkpWorld.h>

// keycode - this needs to go somewhere, this seems as good a place as any
#include <Common/Base/KeyCode.cxx>
#endif

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_HashTree< Physics_Physics_Object* > Physics_Physics_System::s_xPhysicsObjects;

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

static void HK_CALL Physics_Havok_ErrorReport( const char* szMessage, void* pData )
{
    GLToy_DebugOutput_Release( "Havok error: %s", szMessage );
}

#endif

void Physics_Physics_Object::SetPosition( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xVelocity )
{
    SetVelocity( xVelocity );
}

void Physics_Physics_Object::SetVelocity( const GLToy_Vector_3& xVelocity )
{
}

GLToy_OBB Physics_Physics_Object::GetOBB()
{

#ifdef GLTOY_USE_HAVOK_PHYSICS

    if( !m_pxHavokRigidBody )
    {
        return GLToy_OBB();
    }

    const hkVector4& xPos = m_pxHavokRigidBody->getPosition();
    const hkQuaternion& xQuat = m_pxHavokRigidBody->getRotation();
    const hkTransform& xTransform = m_pxHavokRigidBody->getTransform();

    const hkpCollidable* const pxCollidable = m_pxHavokRigidBody->getCollidable();
    if( !pxCollidable )
    {
        // return a point at the position
        return GLToy_OBB( GLToy_Vector_3( xPos( 0 ), xPos( 1 ), xPos( 2 ) ), GLToy_Maths::IdentityMatrix3, 0.0f, 0.0f, 0.0f );
    }

    const hkpShape* const pxShape = pxCollidable->getShape();

    hkAabb xHKAABB;
    pxShape->getAabb( xTransform, 1.0f, xHKAABB );
    
    hkVector4 xHalfExtents;
    xHKAABB.getHalfExtents( xHalfExtents );

    return GLToy_OBB(
        GLToy_Vector_3( xPos( 0 ), xPos( 1 ), xPos( 2 ) ),
        GLToy_Quaternion( xQuat( 0 ), xQuat( 1 ), xQuat( 2 ), xQuat( 3 ) ).GetOrientationMatrix(),
        xHalfExtents( 0 ), xHalfExtents( 1 ), xHalfExtents( 2 ) );

#endif

}

bool Physics_Physics_System::Initialise()
{

    GLToy_Console::RegisterCommand( "testbox", TestBox_Console );

#ifdef GLTOY_USE_HAVOK_PHYSICS

    // this initialisation routine is inspired by the ConsoleExampleMt_win32_9-0 example in the Havok SDK
    hkMemoryRouter* pxMemoryRouter = hkMemoryInitUtil::initDefault();
    hkBaseSystem::init( pxMemoryRouter, Physics_Havok_ErrorReport );

    hkHardwareInfo xHardwareInfo;
    hkGetHardwareInfo( xHardwareInfo );
    g_iMaxThreads = xHardwareInfo.m_numThreads;

    // leave one thread for the renderer and other logic
    hkCpuJobThreadPoolCinfo xThreadPoolCInfo;
    xThreadPoolCInfo.m_numThreads = GLToy_Maths::Max( g_iMaxThreads - 1, 1 );

    //// This line enables timers collection, by allocating 200 Kb per thread.  If you leave this at its default (0),
    //// timer collection will not be enabled.
    //threadPoolCinfo.m_timerBufferPerThreadAllocation = 200000;

    g_pxThreadPool = new hkCpuJobThreadPool( xThreadPoolCInfo );

    hkJobQueueCinfo xJobQueueInfo;
    xJobQueueInfo.m_jobQueueHwSetup.m_numCpuThreads = g_iMaxThreads;
    g_pxJobQueue = new hkJobQueue( xJobQueueInfo );

    ////
    //// Enable monitors for this thread.
    ////

    //// Monitors have been enabled for thread pool threads already (see above comment).
    //hkMonitorStream::getInstance().resize(200000);

    hkpWorldCinfo xHavokWorldInfo;
    xHavokWorldInfo.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_MULTITHREADED;
    xHavokWorldInfo.m_gravity.set( 0.0f, -640.0f, 0.0f );
    xHavokWorldInfo.m_collisionTolerance = 1.0f;
    xHavokWorldInfo.setBroadPhaseWorldSize( 10000.0f );
    xHavokWorldInfo.setupSolverInfo( hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM );
    
    g_pxHavokWorld = new hkpWorld( xHavokWorldInfo );

    g_pxHavokWorld->lock();
    g_pxHavokWorld->markForWrite();
    hkpAgentRegisterUtil::registerAllAgents( g_pxHavokWorld->getCollisionDispatcher() );

#endif

    CreatePhysicsPlane( GLToy_Hash_Constant( "Plane" ), GLToy_Plane( GLToy_Vector_3( 0.0f, 1.0f, 0.0f ), 0.0f ) );

    return true;
}

void Physics_Physics_System::Shutdown()
{

    s_xPhysicsObjects.DeleteAll();

#ifdef GLTOY_USE_HAVOK_PHYSICS
    
    // this is managed for us...
    //delete g_pxHavokWorld;
    
    delete g_pxJobQueue;

    g_pxThreadPool->removeReference();
    
    // this is managed for us...
    //delete g_pxThreadPool;

    hkBaseSystem::quit();
    hkMemoryInitUtil::quit();

#endif

}

void Physics_Physics_System::Update()
{

#ifdef GLTOY_USE_HAVOK_PHYSICS
    
    if( !g_pxHavokWorld )
    {
        return;
    }

    g_pxHavokWorld->unmarkForWrite();
    g_pxHavokWorld->unlock();
    g_pxHavokWorld->stepMultithreaded( g_pxJobQueue, g_pxThreadPool, GLToy_Timer::GetFrameTime() );
    g_pxHavokWorld->lock();
    g_pxHavokWorld->markForWrite();

#endif

}

void Physics_Physics_System::TestBox_Console()
{
    GLToy_String szEntityName;
    szEntityName.SetToFormatString( "Entity%d", GLToy_Entity_System::GetEntityCount() );

    Physics_Entity_PhysicsBox* pxBox = static_cast< Physics_Entity_PhysicsBox* >( GLToy_Entity_System::CreateEntity( szEntityName.GetHash(), PHYSICS_ENTITY_PHYSICSBOX ) );

    pxBox->Spawn( GLToy_AABB( GLToy_Camera::GetPosition(), 25.0f, 25.0f, 25.0f ), GLToy_Camera::GetDirection() * 40.0f );
}

Physics_Physics_Object* Physics_Physics_System::CreatePhysicsPlane( const GLToy_Hash uHash, const GLToy_Plane &xPlane )
{

    Physics_Physics_Object* pxPhysicsObject = new Physics_Physics_Object( uHash );
    s_xPhysicsObjects.AddNode( pxPhysicsObject, uHash );

#ifdef GLTOY_USE_HAVOK_PHYSICS

    hkpPlaneShape* pxPlane = new hkpPlaneShape(
        hkVector4( xPlane.GetNormal()[ 0 ], xPlane.GetNormal()[ 1 ], xPlane.GetNormal()[ 2 ], xPlane.GetDistance() ),
        hkVector4( 0.0f, 0.0f, 0.0f, 0.0f ),
        hkVector4( 4000.0f, 4000.0f, 4000.0f, 0.0f ) );
    
    hkpRigidBodyCinfo xRigidBodyInfo;

    xRigidBodyInfo.m_motionType = hkpMotion::MOTION_FIXED;
    xRigidBodyInfo.m_shape = pxPlane;
    xRigidBodyInfo.m_position = hkVector4( 0.0f, 0.0f, 0.0f, 0.0f );

    hkpRigidBody* pxRigidBody = new hkpRigidBody( xRigidBodyInfo );

    g_pxHavokWorld->lock();
    g_pxHavokWorld->addEntity( pxRigidBody );
    g_pxHavokWorld->unlock();

    pxRigidBody->removeReference();
    pxPlane->removeReference();

#else

    GLToy_Assert( false, "Physics planes require GLTOY_USE_HAVOK_PHYSICS for now..." );

#endif

    return pxPhysicsObject;
}

Physics_Physics_Object* Physics_Physics_System::CreatePhysicsBox( const GLToy_Hash uHash, const GLToy_AABB &xAABB, const GLToy_Vector_3& xVelocity )
{

    Physics_Physics_Object* pxPhysicsObject = new Physics_Physics_Object( uHash );
    s_xPhysicsObjects.AddNode( pxPhysicsObject, uHash );

#ifdef GLTOY_USE_HAVOK_PHYSICS

    const GLToy_Vector_3 xExtents = xAABB.GetExtents();
    hkpBoxShape* pxBox = new hkpBoxShape( hkVector4( xExtents[ 0 ], xExtents[ 1 ], xExtents[ 2 ], 0.0f ), 0 );
    
    hkpRigidBodyCinfo xRigidBodyInfo;

    xRigidBodyInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA; // maybe use sphere inertia as optimisation?
    xRigidBodyInfo.m_shape = pxBox;
    xRigidBodyInfo.m_position = hkVector4( xAABB.GetPosition()[ 0 ] , xAABB.GetPosition()[ 1 ], xAABB.GetPosition()[ 2 ], 0.0f );
    xRigidBodyInfo.m_linearVelocity = hkVector4( xVelocity[ 0 ], xVelocity[ 1 ], xVelocity[ 2 ], 0.0f );

    hkpRigidBody* pxRigidBody = new hkpRigidBody( xRigidBodyInfo );

    g_pxHavokWorld->lock();
    g_pxHavokWorld->addEntity( pxRigidBody );
    g_pxHavokWorld->unlock();

    pxPhysicsObject->SetHavokRigidBodyPointer( pxRigidBody );

    pxRigidBody->removeReference();
    pxBox->removeReference();

#else

    GLToy_Assert( false, "Physics boxes require GLTOY_USE_HAVOK_PHYSICS for now..." );

#endif

    return pxPhysicsObject;
}