/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/Physics.h>

// This file's header
#include <Physics/Physics_Physics_System.h>

// GLToy
#include <Core/GLToy_Timer.h>

// Havok
#include <Physics/Platform_GLToy_Havok_Physics.h>
#ifdef GLTOY_USE_HAVOK_PHYSICS
#include <Physics/Dynamics/hkpDynamics.h>
#include <Physics/Dynamics/World/hkpWorld.h>

#include <Common/Base/KeyCode.cxx>
#endif

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

#ifdef GLTOY_USE_HAVOK_PHYSICS
hkpWorld* g_pxHavokWorld = NULL;
#endif

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool Physics_Physics_System::Initialise()
{

#ifdef GLTOY_USE_HAVOK_PHYSICS

    hkpWorldCinfo xHavokWorldInfo;
    xHavokWorldInfo.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_DISCRETE;
    xHavokWorldInfo.m_gravity.set( 0.0f, -9.8f, 0.0f );
    xHavokWorldInfo.m_collisionTolerance = 0.1f;
    xHavokWorldInfo.setBroadPhaseWorldSize( 150.0f );
    xHavokWorldInfo.setupSolverInfo( hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM );
    
    g_pxHavokWorld = new hkpWorld( xHavokWorldInfo );

    // for now set up a plane to match the test plane
    // - worry about hooking up with environment when this moves into GLToy

#endif

    return true;
}

void Physics_Physics_System::Shutdown()
{

#ifdef GLTOY_USE_HAVOK_PHYSICS
    
    delete g_pxHavokWorld;

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