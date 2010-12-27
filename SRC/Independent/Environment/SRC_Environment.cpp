/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi, Thomas Young
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

#include "Core/GLToy.h"

#include "SRC_Environment.h"

#include "Core/GLToy_Timer.h"
#include "Core/Data Structures/GLToy_List.h"
#include "Core/State/GLToy_State_System.h"
#include "Core/GLToy_UpdateFunctor.h"
#include "Input/GLToy_Input_System.h"
#include "Maths/GLToy_Plane.h"
#include "Maths/GLToy_Ray.h"
#include "Physics/GLToy_Physics_System.h"
#include "Physics/GLToy_Physics_Object.h"
#include "Render/GLToy_Camera.h"
#include "Render/GLToy_Render.h"
#include "Render/GLToy_RenderFunctor.h"
#include "Render/GLToy_Texture.h"
#include "UI/GLToy_UI_System.h"

#include <Core/GLToy_Memory_DebugOff.h>
// Havok
#include <Physics/Platform_GLToy_Havok_Physics.h>
#ifdef GLTOY_USE_HAVOK_PHYSICS
// base
#include <Common/Base/hkBase.h>
#include <Common/Internal/ConvexHull/hkGeometryUtility.h>
/*#include <Common/Base/System/Error/hkDefaultError.h>
#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/Memory/System/Util/hkMemoryInitUtil.h>
#include <Common/Base/Memory/System/hkMemorySystem.h>
#include <Common/Base/Memory/Allocator/Malloc/hkMallocAllocator.h>
#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>
*/

// physics

#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesShape.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Compound/Tree/MOPP/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Compound/Tree/MOPP/hkpMoppUtility.h>
#include <Physics/Dynamics/Entity/hkpEntityListener.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>
/*
#include <Physics/Collide/Dispatch/hkpAgentRegisterUtil.h>





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
, m_bEditor_Highlighted( false )
, m_bEditor_Selected( false )
{
}

SRC_Map_Block::~SRC_Map_Block()
{
}

void SRC_Map_Block::SetHeight( float fHeight )
{
	m_xBoundingBox.m_xPointMax[1] = fHeight * 64.0f;
	if( m_xBoundingBox.m_xPointMax[1] < fSRC_ENV_MIN_BLOCK_HEIGHT )
	{
		m_xBoundingBox.m_xPointMax[1] = fSRC_ENV_MIN_BLOCK_HEIGHT;
	}
}

float SRC_Map_Block::GetHeight() const
{
	return m_xBoundingBox.m_xPointMax[1] / 64.0f;
}

void SRC_Map_Block::SetActive( bool bActive )
{
	m_bActive = bActive;
}

void SRC_Map_Block::SetPosition( GLToy_Vector_2 xPosition )
{
    xPosition -= 0.5f * GLToy_Vector_2( static_cast< float >( uSRC_ENV_BLOCKS ), static_cast< float >( uSRC_ENV_BLOCKS ) );

	GLToy_Vector_3& xMin = m_xBoundingBox.m_xPointMin;
	GLToy_Vector_3& xMax = m_xBoundingBox.m_xPointMax;

	xMin[0] = xPosition[0];
	xMin[1] = fSRC_ENV_VERY_LOW;
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
	const bool bInEditor = 	GLToy_State_System::GetState() == GLToy_Hash_Constant( "Editor" );
	
	if( !m_bActive && !bInEditor )
	{
		return;
	}
	
	const GLToy_Vector_3& xMin = m_xBoundingBox.m_xPointMin;
	const GLToy_Vector_3& xMax = m_xBoundingBox.m_xPointMax;

	GLToy_Texture_System::BindTexture( "SRC/ConcreteTile1.png" );

    GLToy_Render::StartSubmittingQuads();
	{
		// Walls

		// Top bit

		if( bInEditor && !m_bActive )
		{
			GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 0.3f ) );
		}
		else
		{
			GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );
		}

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

		const float fAlpha = ( bInEditor && !m_bActive ) ? 0.3f : 1.0f;

		GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, fAlpha ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, xMax[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_VERY_LOW / 64.0f ) );
		GLToy_Render::SubmitColour( GLToy_Vector_4( 0.0f, 0.0f, 0.0f, fAlpha ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], fSRC_ENV_VERY_LOW, xMax[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_VERY_LOW / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], fSRC_ENV_VERY_LOW, xMax[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, fAlpha ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, xMax[ 2 ] ) );
		
		GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, fAlpha ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, xMin[ 2 ] ) );
		GLToy_Render::SubmitColour( GLToy_Vector_4( 0.0f, 0.0f, 0.0f, fAlpha ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_VERY_LOW / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], fSRC_ENV_VERY_LOW, xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_VERY_LOW / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], fSRC_ENV_VERY_LOW, xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, fAlpha ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, xMin[ 2 ] ) );

		GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, fAlpha ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, xMax[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_VERY_LOW / 64.0f ) );
		GLToy_Render::SubmitColour( GLToy_Vector_4( 0.0f, 0.0f, 0.0f, fAlpha ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], fSRC_ENV_VERY_LOW, xMin[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_VERY_LOW / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], fSRC_ENV_VERY_LOW, xMax[ 2 ] ) );
		
		GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, fAlpha ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, xMax[ 2 ] ) );
		GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, fSRC_ENV_MIN_BLOCK_HEIGHT / 64.0f ) );
		GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], fSRC_ENV_MIN_BLOCK_HEIGHT, xMin[ 2 ] ) );
		GLToy_Render::SubmitColour( GLToy_Vector_4( 0.0f, 0.0f, 0.0f, fAlpha ) );
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
		if( bInEditor )
		{
			GLToy_Render::SubmitColour( GLToy_Vector_4( 8.0f, m_bEditor_Highlighted ? 0.5f : 1.0f, m_bEditor_Selected ? 0.5f : 1.0f, m_bActive ? 1.0f : 0.3f ) );
		}
		else
		{
			GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );
		}

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

void SRC_Map_Block::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    xStream >> m_bActive;
    float fHeight;
    xStream >> fHeight;
    SetHeight( fHeight );
}

void SRC_Map_Block::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    xStream << m_bActive;
    xStream << GetHeight();
}

void SRC_Map_Block::Editor_SetHighlighted( bool bHighlight )
{
	m_bEditor_Highlighted = bHighlight;
}

void SRC_Map_Block::Editor_SetSelected( bool bSelected )
{
	m_bEditor_Selected = bSelected;
}

bool SRC_Map_Block::Editor_IsHighlighted() const
{
	return m_bEditor_Highlighted;
}

bool SRC_Map_Block::Editor_IsSelected() const
{
	return m_bEditor_Selected;
}

// ________________________________ SRC_Environment __________________________________

SRC_Environment::SRC_Environment()
: GLToy_Environment() 
, m_xBlocks()
{
    m_xBlocks.Resize( uSRC_ENV_BLOCKS * uSRC_ENV_BLOCKS );
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
	GLToy_Iterate( SRC_Map_Block, xBlock, m_xBlocks )
		const float fX = static_cast<float>( xIterator.Index() % uSRC_ENV_BLOCKS );
		const float fY = static_cast<float>( xIterator.Index() / uSRC_ENV_BLOCKS );

		xBlock.SetPosition( GLToy_Vector_2( fX, fY ) );
		xBlock.SetActive( false );
    GLToy_Iterate_End;

	CreatePhysics();
}

void SRC_Environment::CreatePhysics()
{
	GLToy_Physics_System::DestroyPhysicsObject( xSRC_ENV_PHYSICS_HASH );
	GLToy_Physics_Object* pxObject = GLToy_Physics_System::CreatePhysicsObject( xSRC_ENV_PHYSICS_HASH );

#ifdef GLTOY_USE_HAVOK_PHYSICS

    // create the brushes...
	const u_int uTotalBlocks = uSRC_ENV_BLOCKS * uSRC_ENV_BLOCKS;
    hkArray< hkpShape* > xShapeArray;
    xShapeArray.reserve( uTotalBlocks );

    GLToy_ConstIterate( SRC_Map_Block, xBlock, m_xBlocks )
		if( !xBlock.IsActive() )
		{
			continue;
		}
		
        hkArray< hkVector4 > xPlanes;
        hkArray< hkVector4 > xVertices;
        xPlanes.reserve( 6 );
        for( u_int v = 0; v < 6; ++v )
        {
            const GLToy_Plane xPlane = xBlock.GetBB().GetPlane( v );
            xPlanes.pushBack( hkVector4(
                ( xPlane.GetNormal()[ 0 ] ),
                ( xPlane.GetNormal()[ 1 ] ),
                ( xPlane.GetNormal()[ 2 ] ),
                ( xPlane.GetDistance() * fHAVOK_SCALE ) ) );
        }

		
        hkGeometryUtility::createVerticesFromPlaneEquations( xPlanes, xVertices );

        if( xVertices.getSize() == 0 )
        {
            continue;
        }

        hkStridedVertices xStridedVertices;
        xStridedVertices.m_numVertices = xVertices.getSize();
        xStridedVertices.m_striding = sizeof( hkVector4 );
        xStridedVertices.m_vertices = &( xVertices[ 0 ]( 0 ) );

        xShapeArray.pushBack( new hkpConvexVerticesShape( xStridedVertices, xPlanes ) );
	GLToy_Iterate_End;

	if( xShapeArray.getSize() == 0 )
	{
		return;
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
	pxObject->SetHavokRigidBodyPointer( pxRigidBody );

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

SRC_Map_Block& SRC_Environment::GetBlock( int iX, int iY )
{
	return m_xBlocks[ iY * uSRC_ENV_BLOCKS + iX % uSRC_ENV_BLOCKS ];
}

void SRC_Environment::Update()
{
	GLToy_Parent::Update();
	
	m_xBlocks.Traverse( GLToy_UpdateFunctor< SRC_Map_Block >() );

	// Editor stuff

	if( !GLToy_State_System::GetState() == GLToy_Hash_Constant( "Editor" ) )
	{
		// We are not in the editor, so we are done
		return;
	}
	
	// Deal with highlighting

	GLToy_List<SRC_Map_Block*> xHighlighted;
	
	GLToy_Vector_2 xPoint = GLToy_UI_System::GetMousePosition();
	GLToy_Ray xRay = GLToy_Camera::ScreenSpaceToRay( xPoint );
		
	GLToy_Iterate( SRC_Map_Block, xBlock, m_xBlocks )
		xBlock.Editor_SetHighlighted( false );
		// Force selection to occur on the top face of the BB
		GLToy_AABB xAABB = xBlock.GetBB();
		xAABB.m_xPointMin[1] = xAABB.m_xPointMax[1] - 10.0f;
		if( xRay.IntersectsWithAABB( xAABB ) )
		{
			// Add to the list BB's which intersected on the top face, we will pick the nearest below
			xHighlighted.Append( &xBlock );
		}
    GLToy_Iterate_End;

	SRC_Map_Block* pxHighlighted = 0;

	if( !xHighlighted.IsEmpty() )
	{
		pxHighlighted = xHighlighted.Head();
		xHighlighted.RemoveHead();

		while( !xHighlighted.IsEmpty() )
		{
			SRC_Map_Block* pxOther = xHighlighted.Head();
			xHighlighted.RemoveHead();

			const float fToCurrent = pxHighlighted->GetDistanceToPoint( GLToy_Camera::GetPosition() );
			const float fToOther = pxOther->GetDistanceToPoint( GLToy_Camera::GetPosition() );

			if( fToOther < fToCurrent )
			{
				pxHighlighted = pxOther;
			}
		}

		pxHighlighted->Editor_SetHighlighted( true );
	}

	if( pxHighlighted )
	{
		if( GLToy_Input_System::GetDebouncedMouseLeft() )
		{
			pxHighlighted->Editor_SetSelected( !pxHighlighted->Editor_IsSelected() );
			if( !GLToy_Input_System::IsKeyDown( GLToy_Input_System::GetCtrlKey() ) )
			{
				// Deselect existing blocks
				GLToy_Iterate( SRC_Map_Block, xBlock, m_xBlocks )
					if( &xBlock != pxHighlighted )
					{
						xBlock.Editor_SetSelected( false );
					}
                GLToy_Iterate_End;
			}
		}
	}

	// Deal with selected blocks

	bool bChange = false;

	GLToy_Iterate( SRC_Map_Block, xBlock, m_xBlocks )
		if( xBlock.Editor_IsSelected() )
		{
			if( GLToy_Input_System::GetMouseWheelScroll() == GLTOY_MOUSE_SCROLL_POSITIVE )
			{
				bChange = true;
				xBlock.SetHeight( GLToy_Maths::Floor( xBlock.GetHeight() - 1.0f ) );
			}
			else if( GLToy_Input_System::GetMouseWheelScroll() == GLTOY_MOUSE_SCROLL_NEGATIVE )
			{
				bChange = true;
				xBlock.SetHeight( GLToy_Maths::Floor( xBlock.GetHeight() + 1.0f ) );
			}
		
			if( GLToy_Input_System::GetDebouncedMouseMiddle() )
			{
				bChange = true;
				xBlock.SetActive( !xBlock.IsActive() );
			}
		}
    GLToy_Iterate_End;

	if( bChange )
	{
		CreatePhysics();
	}
}

void SRC_Environment::Render() const
{
	GLToy_Render::EnableBlending();
    
    GLToy_Render::SetBlendFunction( BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA );

    m_xBlocks.Traverse( GLToy_RenderFunctor< SRC_Map_Block >() );

    GLToy_Render::DisableBlending(); // preserve render state
}
		
void SRC_Environment::Shutdown()
{
	ClearEnv();
}

void SRC_Environment::ClearEnv()
{
    m_xBlocks.Clear();
    m_xBlocks.Resize( uSRC_ENV_BLOCKS * uSRC_ENV_BLOCKS );
}

u_int SRC_Environment::GetType() const
{
	return uSRC_ENVIRONMENT_TYPE;
}

void SRC_Environment::ReadFromBitStream( const GLToy_BitStream& xStream )
{
	GLToy_Parent::ReadFromBitStream( xStream );

    xStream >> m_xBlocks;
}

void SRC_Environment::WriteToBitStream( GLToy_BitStream& xStream ) const
{
	GLToy_Parent::WriteToBitStream( xStream );

    xStream << m_xBlocks;
}

float SRC_Environment::Trace( const GLToy_Ray& xRay, const float fLimitingDistance ) const
{
    float fMin = ( fLimitingDistance > 0.0f ) ? fLimitingDistance : GLToy_Maths::LargeFloat;
	GLToy_ConstIterate( SRC_Map_Block, xBlock, m_xBlocks )
        float fParameter;
		if( xRay.IntersectsWith( xBlock, &fParameter ) )
        {
            fMin = GLToy_Maths::Min( fParameter, fMin );
        }
	GLToy_Iterate_End;

	return ( fMin == GLToy_Maths::LargeFloat ) ? -1.0f : fMin;
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