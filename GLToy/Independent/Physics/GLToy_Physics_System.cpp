/////////////////////////////////////////////////////////////////////////////////////////////
//
// �Copyright 2010 Semi Essessi
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
#include <Physics/GLToy_Physics_System.h>

// GLToy
#include <Core/Console/GLToy_Console.h>
#include <Core/Data Structures/GLToy_HashTree.h>
#include <Core/GLToy_Timer.h>
#include <Entity/GLToy_Entity_System.h>
#include <Environment/GLToy_Environment_Lightmapped.h>
#include <Maths/GLToy_Maths.h>
#include <Maths/GLToy_Plane.h>
#include <Maths/GLToy_Volume.h>
#include <Render/GLToy_Camera.h>

// GLToy
#include <Entity/GLToy_EntityTypes.h>
#include <Entity/Physics/GLToy_Entity_PhysicsBox.h>
#include <Physics/GLToy_Physics_Controller.h>

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
#include <Common/Internal/ConvexHull/hkGeometryUtility.h>

// physics
#include <Physics/Collide/Dispatch/hkpAgentRegisterUtil.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Compound/Tree/MOPP/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Compound/Tree/MOPP/hkpMoppUtility.h>
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesShape.h>
#include <Physics/Collide/Shape/Convex/Triangle/hkpTriangleShape.h>
#include <Physics/Collide/Shape/HeightField/Plane/hkpPlaneShape.h>
#include <Physics/Dynamics/hkpDynamics.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Physics/Dynamics/World/hkpWorld.h>
#include <Physics/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>

// keycode - this needs to go somewhere, this seems as good a place as any
#include <Common/Base/KeyCode.cxx>
#endif

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_HashTree< GLToy_Physics_Object* > GLToy_Physics_System::s_xPhysicsObjects;
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

#ifdef _DEBUG

    if( g_pxHavokWorld )
    {
        g_pxHavokWorld->markForRead();
    }

#endif

}

void GLToy_Havok_MarkForWrite()
{

#ifdef _DEBUG

    if( g_pxHavokWorld )
    {
        g_pxHavokWorld->markForWrite();
    }

#endif

}

void GLToy_Havok_UnmarkForRead()
{

#ifdef _DEBUG

    if( g_pxHavokWorld )
    {
        g_pxHavokWorld->unmarkForRead();
    }

#endif

}

void GLToy_Havok_UnmarkForWrite()
{

#ifdef _DEBUG

    if( g_pxHavokWorld )
    {
        g_pxHavokWorld->unmarkForWrite();
    }

#endif

}

hkpWorld* GLToy_Physics_System::GetHavokWorld()
{
    return g_pxHavokWorld;
}

#endif

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

    g_pxHavokWorld->lockReadOnly();
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
    g_pxHavokWorld->unlockReadOnly();

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

bool GLToy_Physics_System::Initialise()
{

    GLToy_Console::RegisterCommand( "testbox", TestBox_Console );

#ifdef GLTOY_USE_HAVOK_PHYSICS

    // this initialisation routine is inspired by the ConsoleExampleMt_win32_9-0 example in the Havok SDK
    hkMemoryRouter* pxMemoryRouter = hkMemoryInitUtil::initDefault();
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
    xHavokWorldInfo.m_expectedMaxLinearVelocity = 15000.0f; // this seems to be the best magic number to prevent penetration.
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

    // this sucks a bit but stops Havok from complaining too much about variable frame rate
    // TODO - asynchronous updates and interpolation
    if( ls_fAccumulatedTimer > fPHYSICS_STEP_TIME )
    {

        // this seems dumb, but it has higher quality
        s_xDefaultController.Update( ls_fAccumulatedTimer * 0.5f );
        g_pxHavokWorld->stepMultithreaded( g_pxJobQueue, g_pxThreadPool, ls_fAccumulatedTimer * 0.5f );
        s_xDefaultController.Update( ls_fAccumulatedTimer * 0.5f );
        g_pxHavokWorld->stepMultithreaded( g_pxJobQueue, g_pxThreadPool, ls_fAccumulatedTimer * 0.5f );

        s_xDefaultController.LateUpdate();

        ls_fAccumulatedTimer = 0.0f;
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
        if( !xBrush.IsCollidable() )
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

GLToy_Physics_Object* GLToy_Physics_System::CreatePhysicsBox( const GLToy_Hash uHash, const GLToy_AABB &xAABB, const GLToy_Vector_3& xVelocity )
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