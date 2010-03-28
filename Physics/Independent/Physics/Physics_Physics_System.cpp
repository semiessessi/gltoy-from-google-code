/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/Physics.h>

// This file's header
#include <Physics/Physics_Physics_System.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Maths/GLToy_Maths.h>

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

// dynamics
#include <Physics/Dynamics/hkpDynamics.h>
#include <Physics/Dynamics/World/hkpWorld.h>

// keycode - this needs to go somewhere, this seems as good a place as any
#include <Common/Base/KeyCode.cxx>
#endif

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

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

bool Physics_Physics_System::Initialise()
{

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
    xHavokWorldInfo.m_gravity.set( 0.0f, -9.8f, 0.0f );
    xHavokWorldInfo.m_collisionTolerance = 0.1f;
    xHavokWorldInfo.setBroadPhaseWorldSize( 150.0f );
    xHavokWorldInfo.setupSolverInfo( hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM );
    
    g_pxHavokWorld = new hkpWorld( xHavokWorldInfo );

#endif

    return true;
}

void Physics_Physics_System::Shutdown()
{

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

    g_pxHavokWorld->stepDeltaTime( GLToy_Timer::GetFrameTime() );

#endif

}