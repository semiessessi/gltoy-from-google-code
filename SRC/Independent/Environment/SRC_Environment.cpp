
// Thomas Young

#include "Core/GLToy.h"

#include "SRC_Environment.h"

#include "Core/GLToy_Timer.h"
#include "Maths/GLToy_Plane.h"
#include "Physics/GLToy_Physics_System.h"
#include "Render/GLToy_Render.h"
#include "Render/GLToy_Texture.h"

#include <Core/GLToy_Memory_DebugOff.h>
// Havok
#include <Physics/Platform_GLToy_Havok_Physics.h>
#ifdef GLTOY_USE_HAVOK_PHYSICS
// base
#include <Common/Base/hkBase.h>
/*#include <Common/Base/System/Error/hkDefaultError.h>
#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/Memory/System/Util/hkMemoryInitUtil.h>
#include <Common/Base/Memory/System/hkMemorySystem.h>
#include <Common/Base/Memory/Allocator/Malloc/hkMallocAllocator.h>
#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>
#include <Common/Internal/ConvexHull/hkGeometryUtility.h>*/

// physics

#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Compound/Tree/MOPP/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Compound/Tree/MOPP/hkpMoppUtility.h>
#include <Physics/Dynamics/Entity/hkpEntityListener.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>
/*
#include <Physics/Collide/Dispatch/hkpAgentRegisterUtil.h>




#include <Physics/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesShape.h>
#include <Physics/Collide/Shape/Convex/Triangle/hkpTriangleShape.h>
#include <Physics/Collide/Shape/Convex/Sphere/hkpSphereShape.h>
#include <Physics/Collide/Shape/HeightField/Plane/hkpPlaneShape.h>
#include <Physics/Dynamics/hkpDynamics.h>
#include <Physics/Dynamics/Collide/ContactListener/hkpContactListener.h>


#include <Physics/Dynamics/World/hkpWorld.h>
#include <Physics/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>
*/
#endif


// ________________________________ SRC_Map_Block __________________________________

SRC_Map_Block::SRC_Map_Block()
: m_bActive( true )
{
}

SRC_Map_Block::~SRC_Map_Block()
{
}

void SRC_Map_Block::SetHeight( float fHeight )
{
	m_xAABB.m_xBoundingBox.m_xPointMax[1] = fHeight * 64.0f;
}

void SRC_Map_Block::SetActive( bool bActive )
{
	m_bActive = bActive;
}

void SRC_Map_Block::SetPosition( GLToy_Vector_2 xPosition )
{
    xPosition -= 0.5f * GLToy_Vector_2( static_cast< float >( uSRC_ENV_BLOCKS ), static_cast< float >( uSRC_ENV_BLOCKS ) );

	GLToy_Vector_3& xMin = m_xAABB.m_xBoundingBox.m_xPointMin;
	GLToy_Vector_3& xMax = m_xAABB.m_xBoundingBox.m_xPointMax;

	xMin[0] = xPosition[0];
	xMin[1] = fSRC_ENV_VERY_LOW / 64.0f;
	xMin[2] = xPosition[1];
	
	xMax[0] = xPosition[0] + 1.0f;
	xMax[1] = 0.0f;
	xMax[2] = xPosition[1] + 1.0f;

    xMin *= 64.0f;
    xMax *= 64.0f;


}

void SRC_Map_Block::Update()
{
}

void SRC_Map_Block::Render() const
{
	if( !m_bActive )
	{
		return;
	}

	const GLToy_Vector_3& xMin = m_xAABB.m_xBoundingBox.m_xPointMin;
	const GLToy_Vector_3& xMax = m_xAABB.m_xBoundingBox.m_xPointMax;

	GLToy_Texture_System::BindTexture( "Generic/Grid2.png" );

    GLToy_Render::StartSubmittingQuads();
	{
		// Walls

		// Top bit

		GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );

		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, xMax[ 1 ] / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], xMax[ 1 ], xMax[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, xMax[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, xMax[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, xMax[ 1 ] / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], xMax[ 1 ], xMax[ 2 ] ) );
		
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, xMax[ 1 ] / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], xMax[ 1 ], xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, xMax[ 1 ] / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], xMax[ 1 ], xMin[ 2 ] ) );

		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, xMax[ 1 ] / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], xMax[ 1 ], xMax[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, xMax[ 1 ] / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], xMax[ 1 ], xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, xMax[ 2 ] ) );
		
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, xMax[ 1 ] / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], xMax[ 1 ], xMax[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, xMax[ 1 ] / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], xMax[ 1 ], xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, xMax[ 2 ] ) );

		// Fade

		GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, xMax[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_VERY_LOW / 64.0f ) );
		GLToy_Render::SubmitColour( GLToy_Vector_4( 0.0f, 0.0f, 0.0f, 1.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], fSRC_ENV_VERY_LOW, xMax[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_VERY_LOW / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], fSRC_ENV_VERY_LOW, xMax[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, xMax[ 2 ] ) );
		
		GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, xMin[ 2 ] ) );
		GLToy_Render::SubmitColour( GLToy_Vector_4( 0.0f, 0.0f, 0.0f, 1.0f ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_VERY_LOW / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], fSRC_ENV_VERY_LOW, xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_VERY_LOW / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], fSRC_ENV_VERY_LOW, xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, xMin[ 2 ] ) );

		GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, xMax[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_VERY_LOW / 64.0f ) );
		GLToy_Render::SubmitColour( GLToy_Vector_4( 0.0f, 0.0f, 0.0f, 1.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], fSRC_ENV_VERY_LOW, xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_VERY_LOW / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], fSRC_ENV_VERY_LOW, xMax[ 2 ] ) );
		
		GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, xMax[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, xMin[ 2 ] ) );
		GLToy_Render::SubmitColour( GLToy_Vector_4( 0.0f, 0.0f, 0.0f, 1.0f ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_VERY_LOW / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], fSRC_ENV_VERY_LOW, xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_VERY_LOW / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], fSRC_ENV_VERY_LOW, xMax[ 2 ] ) );
	}
    GLToy_Render::EndSubmit();

	// Top

	GLToy_Texture_System::BindTexture( "Generic/Grid3.png" );

    GLToy_Render::StartSubmittingQuads();
	{
		GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );

		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 1.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], xMax[ 1 ], xMax[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 0.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], xMax[ 1 ], xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 0.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], xMax[ 1 ], xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 1.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], xMax[ 1 ], xMax[ 2 ] ) );
		
	}
	GLToy_Render::EndSubmit();
}

// ________________________________ SRC_Environment __________________________________

SRC_Environment::SRC_Environment()
: GLToy_Environment() 
, m_pxBlocks( 0 )
{
}

SRC_Environment::~SRC_Environment()
{
}

void SRC_Environment::Initialise()
{
	CreateEnv();
}

void SRC_Environment::CreateEnv()
{
	GLToy_Assert( !m_pxBlocks, "Blocks pointer should not be set" );

	m_pxBlocks = new SRC_Map_Block*[ uSRC_ENV_BLOCKS * uSRC_ENV_BLOCKS ];
	memset( m_pxBlocks, 0, sizeof( SRC_Map_Block* ) * uSRC_ENV_BLOCKS * uSRC_ENV_BLOCKS );

	for( int iBlock = 0; iBlock < uSRC_ENV_BLOCKS * uSRC_ENV_BLOCKS; ++iBlock )
	{
		m_pxBlocks[iBlock] = new SRC_Map_Block();

		const float fX = static_cast<float>( iBlock % uSRC_ENV_BLOCKS );
		const float fY = static_cast<float>( iBlock / uSRC_ENV_BLOCKS );

		m_pxBlocks[iBlock]->SetPosition( GLToy_Vector_2( fX, fY ) );

		// TODO: Remove hack

		m_pxBlocks[iBlock]->SetHeight( GLToy_Maths::Floor( GLToy_Maths::Random( 0.0f, 1.0f ) * 3.0f ) );
		m_pxBlocks[iBlock]->SetActive( GLToy_Maths::Random( 0.0f, 1.0f ) > 0.3f );
	}

	CreatePhysics();
}

void SRC_Environment::CreatePhysics()
{
	GLToy_Assert( m_pxBlocks != 0, "Blocks pointer should be set" );

	GLToy_Physics_System::DestroyPhysicsObject( xSRC_ENV_PHYSICS_HASH );
	GLToy_Physics_System::CreatePhysicsObject( xSRC_ENV_PHYSICS_HASH );

	#ifdef GLTOY_USE_HAVOK_PHYSICS

    hkArray< hkpShape* > xShapeArray;

	const u_int uTotalBlocks = uSRC_ENV_BLOCKS * uSRC_ENV_BLOCKS;

    // create the brushes...
    xShapeArray.reserve( uTotalBlocks );

    for( u_int u = 0; u < uTotalBlocks; ++u )
    {
		const SRC_Map_Block* pxBlock = m_pxBlocks[ u ];
		
		if( !pxBlock->IsActive() )
		{
			continue;
		}
		
		const GLToy_Vector_3 xExtents = pxBlock->GetAABB()->m_xBoundingBox.GetExtents() * fHAVOK_SCALE;
		hkVector4 xHKExtents = hkVector4( xExtents[ 0 ], xExtents[ 1 ], xExtents[ 2 ] );
		hkpBoxShape* pxBox = new hkpBoxShape( xHKExtents, 0 );

        xShapeArray.pushBack( pxBox );
	}

    hkpListShape* pxShapeList = new hkpListShape( &( xShapeArray[ 0 ] ), xShapeArray.getSize() );
    // compile the mopp tree
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

	hkpWorld* pxWorld = GLToy_Physics_System::GetHavokWorld();

    pxWorld->lock();
    GLToy_Havok_MarkForWrite();
    pxWorld->addEntity( pxRigidBody );
    pxRigidBody->setQualityType( HK_COLLIDABLE_QUALITY_FIXED );
    pxRigidBody->setUserData( xSRC_ENV_PHYSICS_HASH );
    GLToy_Physics_System::CreateCollisionListener( pxRigidBody );
	GLToy_Havok_UnmarkForWrite();
	pxWorld->unlock();

    pxRigidBody->removeReference();
    pxTreeShape->removeReference();
    pxMoppCode->removeReference();

	#else

    GLToy_Assert( false, "Physics require GLTOY_USE_HAVOK_PHYSICS for now..." );

	#endif

}

SRC_Map_Block* SRC_Environment::GetBlock( int iX, int iY )
{
	if( !m_pxBlocks )
	{
		return 0;
	}

	return ( m_pxBlocks[0] + ( iY * uSRC_ENV_BLOCKS + iX % uSRC_ENV_BLOCKS ) );
}

void SRC_Environment::Update()
{
	GLToy_Parent::Update();
	
	for( int iBlock = 0; iBlock < uSRC_ENV_BLOCKS * uSRC_ENV_BLOCKS; ++iBlock )
	{
		m_pxBlocks[iBlock]->Update();
	}
}

void SRC_Environment::Render() const
{
	if( !m_pxBlocks )
	{
		return;
	}

	GLToy_Render::EnableBlending();
    
    GLToy_Render::SetBlendFunction( BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA );

	for( int iBlock = 0; iBlock < uSRC_ENV_BLOCKS * uSRC_ENV_BLOCKS; ++iBlock )
	{
		m_pxBlocks[iBlock]->Render();
	}

	GLToy_Render::EnableDepthWrites();
}
		
void SRC_Environment::Shutdown()
{
	ClearEnv();
}

void SRC_Environment::ClearEnv()
{
	if( !m_pxBlocks )
	{
		return;
	}

	for( int iBlock = 0; iBlock < uSRC_ENV_BLOCKS * uSRC_ENV_BLOCKS; ++iBlock )
	{
		delete m_pxBlocks[iBlock];
	}
	delete []m_pxBlocks;
}

u_int SRC_Environment::GetType() const
{
	return uSRC_ENVIRONMENT_TYPE;
}

void SRC_Environment::ReadFromBitStream( const GLToy_BitStream& xStream )
{
	GLToy_Parent::ReadFromBitStream( xStream );
}

void SRC_Environment::WriteToBitStream( GLToy_BitStream& xStream ) const
{
	GLToy_Parent::WriteToBitStream( xStream );
}

float SRC_Environment::Trace( const GLToy_Ray& xRay, const float fLimitingDistance ) const
{
	return 0.0f;
}

GLToy_Environment* SRC_CreateEnvironment( const u_int uType )
{
    if( uType == uSRC_ENVIRONMENT_TYPE )
    {
        return new SRC_Environment();
    }

    return NULL;
}

bool SRC_Environment::Load( const GLToy_String& xName )
{
	return false;
}