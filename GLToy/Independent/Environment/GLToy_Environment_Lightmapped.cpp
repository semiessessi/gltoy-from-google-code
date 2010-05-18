/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
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
#include <Environment/GLToy_Environment_Lightmapped.h>

// GLToy
#include <Core/Console/GLToy_Console.h>
#include <Environment/GLToy_Environment_System.h>
#include <Physics/GLToy_Physics_System.h>
#include <Render/Font/GLToy_Font.h>
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture.h>
#include <Render/Shader/GLToy_Shader.h>
#include <Render/Shader/GLToy_Shader_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Environment_Lightmapped::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    GLToy_Parent::ReadFromBitStream( xStream );

    GLToy_Assert( false, "Need to finish ReadFromBitStream for GLToy_Environment_Lightmapped" );
}

void GLToy_Environment_Lightmapped::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    GLToy_Parent::WriteToBitStream( xStream );

    GLToy_Assert( false, "Need to finish WriteToBitStream for GLToy_Environment_Lightmapped" );
}

void GLToy_Environment_Lightmapped::Initialise()
{
    m_pxPhysicsObject = GLToy_Physics_System::CreatePhysicsEnvironment( GLToy_Hash_Constant( "Environment" ), *this );
}

void GLToy_Environment_Lightmapped::Shutdown()
{
    // clean up lightmap textures
    GLToy_ConstIterate( GLToy_Environment_LightmappedFace, xIterator, &m_xFaces )
    {
        if( xIterator.Current().m_uLightmapHash != GLToy_Hash_Constant( "White" ) )
        {
            GLToy_Texture_System::DestroyTexture( xIterator.Current().m_uLightmapHash );
        }
    }
}

// TODO - many optimisations
// * use PVS to cull
// * frustrum cull PVS
// * vertex/index buffers
// * single pass lightmapping with shader - although it will need its own pass anyway when there is a deferred renderer
void GLToy_Environment_Lightmapped::Render() const
{
    GLToy_Render::EnableBackFaceCulling();
    GLToy_Render::SetCWFaceWinding();
    GLToy_Render::DisableBlending();

    GLToy_ConstIterate( GLToy_Environment_LightmappedFace, xIterator, &m_xFaces )
    {
        const GLToy_Environment_LightmappedFace& xFace = xIterator.Current();
        xFace.m_uRenderFlags = 0;
    }

    if( GLToy_Environment_System::IsRenderingLightmapOnly() )
    {
        RenderLightmap();
    }
    else
    {
        GLToy_EnvironmentLeaf_Lightmapped* pxLeaf = static_cast< GLToy_EnvironmentLeaf_Lightmapped* >( GetLeafData( GLToy_Camera::GetPosition() ) );
        if( !IsEmpty() && pxLeaf && pxLeaf->m_uCluster != 0xFFFF )
        {
            GLToy_ConstIterate( u_int, xPVSIterator, &( m_xClusters[ pxLeaf->m_uCluster ].m_xPVS ) )
            {
                GLToy_Assert( xPVSIterator.Current() < m_xClusters.GetCount(), "Cluster index is too large!" );
                
                GLToy_ConstIterate( u_int, xClusterIterator, &( m_xClusters[ xPVSIterator.Current() ].m_xIndices ) )
                {
                    m_xLeaves[ xClusterIterator.Current() ].Render();
                }
            }
        }
        else
        {
            // fallback - render with no bsp tree or visibilty culling
            const bool bQuadRes = GLToy_Environment_System::IsBSPQuadRes();
            GLToy_ConstIterate( GLToy_Environment_LightmappedFace, xIterator, &m_xFaces )
            {
                const GLToy_Environment_LightmappedFace& xFace = xIterator.Current();
                
                if( !xFace.m_bVisible || xFace.m_bRendered )
                {
                    continue;
                }

                xFace.m_bRendered = true;

                GLToy_Texture_System::BindTexture( xFace.m_uTextureHash );

                GLToy_Render::StartSubmittingPolygon();

                GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );

                GLToy_ConstIterate( u_int, xIndexIterator, &( xFace.m_xIndices ) )
                {
                    const GLToy_Environment_LightmappedFaceVertex& xVertex = m_xVertices[ xIndexIterator.Current() ];

                    GLToy_Render::SubmitUV( xVertex.m_xUV * ( bQuadRes ? 4.0f : 1.0f ) );
                    GLToy_Render::SubmitVertex( xVertex.m_xPosition );
                }

                GLToy_Render::EndSubmit();
            }
        }

        if( GLToy_Environment_System::IsRenderingLightmap() )
        {
            GLToy_Render::EnableBlending();
            GLToy_Render::DisableDepthWrites();

            GLToy_Render::SetBlendFunction( BLEND_ZERO, BLEND_SRC_COLOR );
            
            RenderLightmap();
            
            GLToy_Render::EnableDepthWrites();
            GLToy_Render::DisableBlending();
        }
    }

    GLToy_Render::DisableBackFaceCulling();
}

void GLToy_Environment_Lightmapped::RenderLightmap() const
{
    GLToy_EnvironmentLeaf_Lightmapped* pxLeaf = static_cast< GLToy_EnvironmentLeaf_Lightmapped* >( GetLeafData( GLToy_Camera::GetPosition() ) );
    if( !IsEmpty() && pxLeaf && pxLeaf->m_uCluster != 0xFFFF )
    {
        GLToy_ConstIterate( u_int, xPVSIterator, &( m_xClusters[ pxLeaf->m_uCluster ].m_xPVS ) )
        {
            GLToy_Assert( xPVSIterator.Current() < m_xClusters.GetCount(), "Cluster index is too large!" );
            
            GLToy_ConstIterate( u_int, xClusterIterator, &( m_xClusters[ xPVSIterator.Current() ].m_xIndices ) )
            {
                m_xLeaves[ xClusterIterator.Current() ].RenderLightmap();
            }
        }
    }
    else
    {
        // fallback - render with no bsp tree or visibilty culling
        GLToy_ConstIterate( GLToy_Environment_LightmappedFace, xIterator, &m_xFaces )
        {
            const GLToy_Environment_LightmappedFace& xFace = xIterator.Current();

            if( !xFace.m_bVisible || xFace.m_bRenderedLightmap )
            {
                continue;
            }

            xFace.m_bRenderedLightmap = true;

            GLToy_Texture_System::BindTexture( xFace.m_uLightmapHash );

            GLToy_Render::StartSubmittingPolygon();

            GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );

            GLToy_ConstIterate( u_int, xIndexIterator, &( xFace.m_xIndices ) )
            {
                const GLToy_Environment_LightmappedFaceVertex& xVertex = m_xVertices[ xIndexIterator.Current() ];

                GLToy_Render::SubmitUV( xVertex.m_xLightmapUV );
                GLToy_Render::SubmitVertex( xVertex.m_xPosition );
            }

            GLToy_Render::EndSubmit();
        }
    }
}

void GLToy_Environment_Lightmapped::Update()
{
}

void GLToy_Environment_Lightmapped::Render2D() const
{
    GLToy_Render::DisableDepthTesting();
    GLToy_EnvironmentLeaf_Lightmapped* pxLeaf = static_cast< GLToy_EnvironmentLeaf_Lightmapped* >( GetLeafData( GLToy_Camera::GetPosition() ) );
    if( !IsEmpty() && pxLeaf && pxLeaf->m_uCluster != 0xFFFF )
    {
        GLToy_ConstIterate( u_int, xPVSIterator, &( m_xClusters[ pxLeaf->m_uCluster ].m_xPVS ) )
        {
            GLToy_Assert( xPVSIterator.Current() < m_xClusters.GetCount(), "Cluster index is too large!" );
            
            GLToy_ConstIterate( u_int, xClusterIterator, &( m_xClusters[ xPVSIterator.Current() ].m_xIndices ) )
            {
                m_xLeaves[ xClusterIterator.Current() ].RenderDebugFaceInfo();
            }
        }
    }
    else
    {
        // fallback - render with no bsp tree or visibilty culling
        GLToy_ConstIterate( GLToy_Environment_LightmappedFace, xIterator, &m_xFaces )
        {
            const GLToy_Environment_LightmappedFace& xFace = xIterator.Current();

            if( !xFace.m_bVisible )
            {
                continue;
            }

            GLToy_Vector_3 xAverage = GLToy_Maths::ZeroVector3;
            GLToy_ConstIterate( u_int, xIndexIterator, &( xFace.m_xIndices ) )
            {
                xAverage += m_xVertices[ xIndexIterator.Current() ].m_xPosition;
            }
            xAverage /= static_cast< float >( xFace.m_xIndices.GetCount() );

            const GLToy_Vector_2 xPoint = GLToy_Camera::WorldSpaceToScreenSpace( xAverage );
            if( GLToy_Camera::IsPointOnScreen( xPoint ) )
            {
                GLToy_String xFaceString;
                xFaceString.SetToFormatString( "Face %d", xIterator.Index() );
                GLToy_Console::GetFont()->RenderString( xFaceString, xPoint[ 0 ], xPoint[ 1 ] );
            }
        }
    }
}

int GLToy_Environment_Lightmapped::GetType() const
{
    return ENV_LIGHTMAPPED;
}

// TODO - implement trace
float GLToy_Environment_Lightmapped::Trace( const GLToy_Ray& xRay, const float fLimitingDistance ) const
{
    return -1.0f;
}

u_int GLToy_Environment_Lightmapped::GetVertexIndex( const GLToy_Environment_LightmappedFaceVertex& xVertex )
{
    GLToy_ConstIterate( GLToy_Environment_LightmappedFaceVertex, xIterator, &m_xVertices )
    {
        if( xVertex == xIterator.Current() )
        {
            return xIterator.Index();
        }
    }

    m_xVertices.Append( xVertex );

    return m_xVertices.GetCount() - 1;
}

const GLToy_Vector_3& GLToy_Environment_Lightmapped::GetFaceVertexPosition( const u_int uFace, const u_int uVertex ) const
{
    return m_xVertices[ m_xFaces[ uFace ].m_xIndices[ uVertex ] ].m_xPosition;
}

bool GLToy_Environment_Lightmapped::ValidateBSPTree() const
{
    class ValidateFunctor
    : public GLToy_ConstFunctor< GLToy_BSPNode< GLToy_EnvironmentLeaf > >
    {

        public:

            ValidateFunctor()
            : m_bValid( true )
            {
            }

            virtual void operator ()( const GLToy_BSPNode< GLToy_EnvironmentLeaf >* const pxData )
            {
                // is it already been shown invalid?
                if( !m_bValid )
                {
                    return;
                }

                // is this a leaf?
                if( pxData->GetData() )
                {
                    return;
                }

                // check if the children are leaves, if they are check their points are on the correct side of the plane
                const GLToy_Plane& xPlane = pxData->GetPlane();

                const GLToy_BSPNode< GLToy_EnvironmentLeaf >* const pxPositive = pxData->GetPositiveNode();
                if( pxPositive->GetData() )
                {
                    const GLToy_EnvironmentLeaf_Lightmapped* const pxLeaf = static_cast< const GLToy_EnvironmentLeaf_Lightmapped* const >( pxPositive->GetData() );
                    for( u_int u = 0; u < pxLeaf->GetFaceCount(); ++u )
                    {
                        for( u_int v = 0; v < pxLeaf->GetFace( u ).GetVertexCount(); ++v )
                        {
                            const GLToy_Vector_3& xTestPoint = pxLeaf->GetFaceVertex( u, v ).m_xPosition;
                            const float fDistance = xPlane.SignedDistance( xTestPoint );
                            if( fDistance < 0.0f )
                            {
                                m_bValid = false;
                                return;
                            }
                        }
                    }
                }

                const GLToy_BSPNode< GLToy_EnvironmentLeaf >* const pxNegative = pxData->GetNegativeNode();
                if( pxNegative->GetData() )
                {
                    const GLToy_EnvironmentLeaf_Lightmapped* const pxLeaf = static_cast< const GLToy_EnvironmentLeaf_Lightmapped* const >( pxNegative->GetData() );
                    for( u_int u = 0; u < pxLeaf->GetFaceCount(); ++u )
                    {
                        for( u_int v = 0; v < pxLeaf->GetFace( u ).GetVertexCount(); ++v )
                        {
                            const GLToy_Vector_3& xTestPoint = pxLeaf->GetFaceVertex( u, v ).m_xPosition;
                            const float fDistance = xPlane.SignedDistance( xTestPoint );
                            if( fDistance > 0.0f )
                            {
                                m_bValid = false;
                                return;
                            }
                        }
                    }
                }
            }

            bool m_bValid;
    };

    ValidateFunctor xFunctor;
    TraverseNodes( xFunctor );

    return xFunctor.m_bValid;
}

void GLToy_EnvironmentLeaf_Lightmapped::Render() const
{
    if( !m_pxParent )
    {
        return;
    }

    const bool bQuadRes = GLToy_Environment_System::IsBSPQuadRes();
    GLToy_ConstIterate( u_int, xIterator, &m_xIndices )
    {
        GLToy_Assert( xIterator.Current() < m_pxParent->m_xFaces.GetCount(), "Bad face index: %d (max: %d)", xIterator.Current(), m_pxParent->m_xFaces.GetCount() - 1 );

        const GLToy_Environment_LightmappedFace& xFace = m_pxParent->m_xFaces[ xIterator.Current() ];

        if( !xFace.m_bVisible || xFace.m_bRendered )
        {
            continue;
        }

        xFace.m_bRendered = true;

        GLToy_Texture_System::BindTexture( xFace.m_uTextureHash );

        GLToy_Render::StartSubmittingPolygon();

        GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );

        GLToy_ConstIterate( u_int, xIndexIterator, &( xFace.m_xIndices ) )
        {
            const GLToy_Environment_LightmappedFaceVertex& xVertex = m_pxParent->m_xVertices[ xIndexIterator.Current() ];

            GLToy_Render::SubmitUV( xVertex.m_xUV * ( bQuadRes ? 4.0f : 1.0f ) );
            GLToy_Render::SubmitVertex( xVertex.m_xPosition );
        }

        GLToy_Render::EndSubmit();
    }
}

void GLToy_EnvironmentLeaf_Lightmapped::RenderLightmap() const
{
    if( !m_pxParent )
    {
        return;
    }

    GLToy_ConstIterate( u_int, xIterator, &m_xIndices )
    {
        const GLToy_Environment_LightmappedFace& xFace = m_pxParent->m_xFaces[ xIterator.Current() ];

        if( !xFace.m_bVisible || xFace.m_bRenderedLightmap )
        {
            continue;
        }

        xFace.m_bRenderedLightmap = true;

        GLToy_Texture_System::BindTexture( xFace.m_uLightmapHash );

        GLToy_Render::StartSubmittingPolygon();

        GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );

        GLToy_ConstIterate( u_int, xIndexIterator, &( xFace.m_xIndices ) )
        {
            const GLToy_Environment_LightmappedFaceVertex& xVertex = m_pxParent->m_xVertices[ xIndexIterator.Current() ];

            GLToy_Render::SubmitUV( xVertex.m_xLightmapUV );
            GLToy_Render::SubmitVertex( xVertex.m_xPosition );
        }

        GLToy_Render::EndSubmit();
    }
}

void GLToy_EnvironmentLeaf_Lightmapped::RenderDebugFaceInfo() const
{
    if( !m_pxParent )
    {
        return;
    }

    GLToy_ConstIterate( u_int, xIterator, &m_xIndices )
    {
        const GLToy_Environment_LightmappedFace& xFace = m_pxParent->m_xFaces[ xIterator.Current() ];

        if( !xFace.m_bVisible )
        {
            continue;
        }

        GLToy_Vector_3 xAverage = GLToy_Maths::ZeroVector3;
        GLToy_ConstIterate( u_int, xIndexIterator, &( xFace.m_xIndices ) )
        {
            xAverage += m_pxParent->m_xVertices[ xIndexIterator.Current() ].m_xPosition;
        }
        xAverage /= static_cast< float >( xFace.m_xIndices.GetCount() );

        const GLToy_Vector_2 xPoint = GLToy_Camera::WorldSpaceToScreenSpace( xAverage );
        if( GLToy_Camera::IsPointOnScreen( xPoint ) )
        {
            GLToy_String xFaceString;
            GLToy_String xClusterString;
            xFaceString.SetToFormatString( "Face %d", xIterator.Index() );
            xClusterString.SetToFormatString( "Cluster %d", this->m_uCluster );
            GLToy_Console::GetFont()->RenderString( xFaceString, xPoint[ 0 ], xPoint[ 1 ] );
            GLToy_Console::GetFont()->RenderString( xClusterString, xPoint[ 0 ], xPoint[ 1 ] + GLToy_Console::GetFont()->GetHeight() );
        }
    }
}

const GLToy_Environment_LightmappedFace& GLToy_EnvironmentLeaf_Lightmapped::GetFace( const u_int uFace ) const
{
    GLToy_Assert( m_pxParent != NULL, "Trying to get a face from a leaf with no associated environment!" );
    GLToy_Assert( uFace < GetFaceCount(), "This leaf has %d faces, but we are asking for face %d.", GetFaceCount(), uFace );

    return m_pxParent->m_xFaces[ m_xIndices[ uFace ] ];
}

const GLToy_Environment_LightmappedFaceVertex& GLToy_EnvironmentLeaf_Lightmapped::GetFaceVertex( const u_int uFace, const u_int uVertex ) const
{
    const GLToy_Environment_LightmappedFace& xFace = GetFace( uFace );
    
    GLToy_Assert( uVertex < xFace.GetVertexCount(), "This face has %d vertices, but we are asking for vertex %d", xFace.GetVertexCount(), uVertex );

    return m_pxParent->m_xVertices[ xFace.m_xIndices[ uVertex ] ];
}
