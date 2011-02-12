/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010, 2011 Semi Essessi
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
#include <Environment/GLToy_Environment_Lightmapped.h>

// GLToy
#include <Core/Console/GLToy_Console.h>
#include <Environment/GLToy_Environment_System.h>
#include <Material/GLToy_Material_System.h>
#include <Physics/GLToy_Physics_System.h>
#include <Render/Font/GLToy_Font.h>
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_Light_System.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Render_Metrics.h>
#include <Render/GLToy_Texture_System.h>
#include <Render/GLToy_VertexBuffer.h>
#include <Render/Shader/GLToy_Shader.h>
#include <Render/Shader/GLToy_Shader_System.h>
#include <Visibility/GLToy_Visibility_System.h>

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
    // register PVS with visiblity system
    GLToy_Visibility_System::ClearPVS();
    GLToy_ConstIterate( GLToy_Environment_LightmappedCluster, xCluster, m_xClusters )
        GLToy_Visibility_System::RegisterCluster( xCluster.m_xPVS, xCluster.m_xIndices );
    GLToy_Iterate_End;

    // initialise physics
    m_pxPhysicsObject = GLToy_Physics_System::CreatePhysicsEnvironment( GLToy_Hash_Constant( "Environment" ), *this );

    // create a vertex buffer
    GLToy_Vertex_Deferred* pxVertices = new GLToy_Vertex_Deferred[ m_xVertices.GetCount() ];

    GLToy_ConstIterate( GLToy_Environment_LightmappedFaceVertex, xVertex, m_xVertices )
        pxVertices[ xIterator.Index() ].m_xPosition = xVertex.m_xPosition;
        pxVertices[ xIterator.Index() ].m_xUV = GLToy_Vector_4( xVertex.m_xUV * /*( bQuadRes ?*/ 4.0f/* : 1.0f )*/, 0.0f, 0.0f );
        pxVertices[ xIterator.Index() ].m_xBasisVectors.m_xEncodedNormal = GLToy_Maths::CompressNormal( xVertex.m_xNormal );
        pxVertices[ xIterator.Index() ].m_xBasisVectors.m_xEncodedTangent = GLToy_Maths::CompressNormal( xVertex.m_xTangent );
    GLToy_Iterate_End;

    m_pxVertexBuffer = GLToy_VertexBuffer_Deferred::Create( m_xVertices.GetCount(), pxVertices );

    delete[] pxVertices;

    // now create an index buffer and fill the faces with appropriate data for using it...
    GLToy_Array< u_short > xIndices;
    GLToy_Iterate( GLToy_Environment_LightmappedFace, xFace, m_xFaces )
        
        xFace.m_xVertexBufferData.m_usCount = xFace.m_xIndices.GetCount();
        xFace.m_xVertexBufferData.m_usOffset = xIndices.GetCount();

        u_short usStart = 0xFFFF;
        u_short usEnd = 0;

        GLToy_ConstIterate( u_int, uIndex, xFace.m_xIndices )
            xIndices.Append( uIndex );
            
            if( uIndex > usEnd )
            {
                usEnd = uIndex;
            }

            if( uIndex < usStart )
            {
                usStart = uIndex;
            }

        GLToy_Iterate_End;

        xFace.m_xVertexBufferData.m_usMinIndex = usStart;
        xFace.m_xVertexBufferData.m_usMaxIndex = usEnd;

    GLToy_Iterate_End;

    m_pxIndexBuffer = GLToy_IndexBuffer::Create( xIndices.GetCount(), xIndices.GetDataPointer() );

    // optimise for material batching and frustum culling
    for( u_int u = 0; u < m_xLeaves.GetCount(); ++u )
    {
        GLToy_EnvironmentLeaf_Lightmapped& xLeaf = *static_cast< GLToy_EnvironmentLeaf_Lightmapped* >( &m_xLeaves[ u ] );

        if( xLeaf.m_xIndices.GetCount() == 0 )
        {
            continue;
        }

        xLeaf.m_xBoundingSphere.SetToPoint( m_xVertices[ m_xFaces[ xLeaf.m_xIndices[ 0 ] ].m_xIndices[ 0 ] ].m_xPosition );
        GLToy_ConstIterate( u_int, uFaceIndex, xLeaf.m_xIndices )

            const GLToy_Environment_LightmappedFace& xFace = xLeaf.GetFace( xIterator.Index() );

            bool bFound = false;
            for( u_int v = 0; v < xLeaf.m_xBatches.GetCount(); ++v )
            {
                if( xLeaf.m_xBatches[ v ].m_uHash == xFace.m_uTextureHash )
                {
                    bFound = true;
                    xLeaf.m_xBatches[ v ].m_xFaces.Append( uFaceIndex );
                    break;
                }
            }

            if( !bFound )
            {
                xLeaf.m_xBatches.Append( GLToy_EnvironmentLeaf_Lightmapped::MaterialBatch() );
                xLeaf.m_xBatches.End().m_uHash = xFace.m_uTextureHash;
                xLeaf.m_xBatches.End().m_xFaces.Append( uFaceIndex );
            }

            GLToy_ConstIterate( u_int, uIndex, xFace.m_xIndices )
                xLeaf.m_xBoundingSphere.GrowByPoint( m_xVertices[ xFace.m_xIndices[ 0 ] ].m_xPosition );
            GLToy_Iterate_End;

        GLToy_Iterate_End;
    }
}

void GLToy_Environment_Lightmapped::Shutdown()
{
    // clean up index & vertex buffer
    delete m_pxVertexBuffer;
    delete m_pxIndexBuffer;

    // clean up lightmap textures
    GLToy_ConstIterate( GLToy_Environment_LightmappedFace, xFace, m_xFaces )
        if( xFace.m_uLightmapHash != GLToy_Hash_Constant( "White" ) )
        {
            GLToy_Texture_System::DestroyTexture( xFace.m_uLightmapHash );
        }
    GLToy_Iterate_End;
}

// TODO - many optimisations
// * frustum cull PVS
void GLToy_Environment_Lightmapped::Render() const
{
    if( GLToy_Render::HasDeferredBuffer() )
    {
        if( !GLToy_Environment_System::IsRenderingLightmapOnly() )
        {
            GLToy_Render::RegisterDeferred( this );
            GLToy_Light_System::RegisterLightSource( this );
        }
    }
    else
    {
        GLToy_Render::EnableBackFaceCulling();
        GLToy_Render::SetCWFaceWinding();
    }

    GLToy_Render::DisableBlending();
    GLToy_Render::EnableDepthTesting();
    GLToy_Render::EnableDepthWrites();
    GLToy_Render::UseProgram( 0 );

    if( GLToy_Environment_System::IsDebugRenderNormals() )
    {
        GLToy_Texture_System::BindWhite();
        GLToy_EnvironmentLeaf_Lightmapped* pxLeaf = static_cast< GLToy_EnvironmentLeaf_Lightmapped* >( GetLeafData( GLToy_Camera::GetPosition() ) );
        if( !IsEmpty() && pxLeaf && pxLeaf->m_uCluster != 0xFFFF )
        {
            GLToy_ConstIterate( u_int, uClusterIndex, m_xClusters[ pxLeaf->m_uCluster ].m_xPVS )
                GLToy_Assert( uClusterIndex < m_xClusters.GetCount(), "Cluster index is too large!" );
                
                GLToy_ConstIterate( u_int, uCluster, m_xClusters[ uClusterIndex ].m_xIndices )
                    GetLeaf< GLToy_EnvironmentLeaf_Lightmapped >( uCluster )->RenderDebugNormals();
                GLToy_Iterate_End;
            GLToy_Iterate_End;
        }
    }

    if( GLToy_Render::HasDeferredBuffer() )
    {
        return;
    }

    GLToy_ConstIterate( GLToy_Environment_LightmappedFace, xFace, m_xFaces )
        xFace.m_uRenderFlags = 0;
    GLToy_Iterate_End;

    if( GLToy_Environment_System::IsRenderingLightmapOnly() )
    {
        RenderLightmap();
    }
    else
    {
        GLToy_EnvironmentLeaf_Lightmapped* pxLeaf = static_cast< GLToy_EnvironmentLeaf_Lightmapped* >( GetLeafData( GLToy_Camera::GetPosition() ) );
        if( !IsEmpty() && pxLeaf && pxLeaf->m_uCluster != 0xFFFF )
        {
            GLToy_ConstIterate( u_int, uClusterIndex, m_xClusters[ pxLeaf->m_uCluster ].m_xPVS )
                GLToy_Assert( uClusterIndex < m_xClusters.GetCount(), "Cluster index is too large!" );
                
                GLToy_ConstIterate( u_int, uCluster, m_xClusters[ uClusterIndex ].m_xIndices )
                    m_xLeaves[ uCluster ].Render();
                GLToy_Iterate_End;
            GLToy_Iterate_End;
        }
        else
        {
            // fallback - render with no bsp tree or visibilty culling
            const bool bQuadRes = GLToy_Environment_System::IsBSPQuadRes();
            GLToy_ConstIterate( GLToy_Environment_LightmappedFace, xFace, m_xFaces )                
                if( !xFace.m_bVisible || xFace.m_bRendered )
                {
                    continue;
                }

                xFace.m_bRendered = true;

                GLToy_Texture_System::BindTexture( xFace.m_uTextureHash );

                GLToy_Render::StartSubmittingPolygon();

                GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );

                GLToy_ConstIterate( u_int, uIndex, xFace.m_xIndices )
                    const GLToy_Environment_LightmappedFaceVertex& xVertex = m_xVertices[ uIndex ];

                    GLToy_Render::SubmitUV( xVertex.m_xUV * ( bQuadRes ? 4.0f : 1.0f ) );
                    GLToy_Render::SubmitVertex( xVertex.m_xPosition );
                GLToy_Iterate_End;

                GLToy_Render::EndSubmit();
            GLToy_Iterate_End;
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

void GLToy_Environment_Lightmapped::RenderDeferred() const
{
    if( GLToy_Environment_System::IsRenderingLightmapOnly() )
    {
        return;
    }

    GLToy_Render::EnableBackFaceCulling();
    GLToy_Render::SetCWFaceWinding();
    GLToy_Render::DisableBlending();
    GLToy_Render::EnableDepthTesting();
    GLToy_Render::EnableDepthWrites();

    GLToy_ConstIterate( GLToy_Environment_LightmappedFace, xFace, m_xFaces )
        xFace.m_uRenderFlags = 0;
    GLToy_Iterate_End;

    GLToy_Render::EnableVertexBuffers();
    GLToy_Render::EnableIndexBuffers();

    m_pxVertexBuffer->Bind();
    m_pxIndexBuffer->Bind();

    // TODO: Finish GLToy_Visiblity_System and do something a lot better than this
    // which can be used by lights, sounds and entities as well
    GLToy_EnvironmentLeaf_Lightmapped* pxLeaf = static_cast< GLToy_EnvironmentLeaf_Lightmapped* >( GetLeafData( GLToy_Camera::GetPosition() ) );
    if( !IsEmpty() && pxLeaf && pxLeaf->m_uCluster != 0xFFFF )
    {
        GLToy_ConstIterate( u_int, uClusterIndex, m_xClusters[ pxLeaf->m_uCluster ].m_xPVS )
            GLToy_Assert( uClusterIndex < m_xClusters.GetCount(), "Cluster index is too large!" );
                
            GLToy_ConstIterate( u_int, uLeaf, m_xClusters[ uClusterIndex ].m_xIndices )
                
                if( GetLeaf< GLToy_EnvironmentLeaf_Lightmapped >( uLeaf )->m_xIndices.GetCount() == 0 )
                {
                    continue;
                }

                // TODO: some frustum culling   
                //if( m_xLeaves[ uCluster ].m_xBoundingSphere.IsOutside( GLToy_Camera::GetPosition() ) )
                //{
                //    const GLToy_Vector_2 xTestPoint =
                //        GLToy_Camera::WorldSpaceToScreenSpace(
                //            m_xLeaves[ uCluster ].m_xBoundingSphere.GetPosition()
                //            + GLToy_Camera::GetDirection() * m_xLeaves[ uCluster ].m_xBoundingSphere.GetRadius()
                //        );

                //    if( xTestPoint == GLToy_Maths::LargeVector2 )
                //    {
                //        continue;
                //    }
                //}

                GetLeaf< GLToy_EnvironmentLeaf_Lightmapped >( uLeaf )->RenderDeferred();

            GLToy_Iterate_End;
        GLToy_Iterate_End;
    }

    GLToy_Render::DisableVertexBuffers();
    GLToy_Render::DisableIndexBuffers();

    GLToy_Render::DisableBackFaceCulling();
}

void GLToy_Environment_Lightmapped::RenderLightmap() const
{
    GLToy_EnvironmentLeaf_Lightmapped* pxLeaf = static_cast< GLToy_EnvironmentLeaf_Lightmapped* >( GetLeafData( GLToy_Camera::GetPosition() ) );
    if( !IsEmpty() && pxLeaf && pxLeaf->m_uCluster != 0xFFFF )
    {
        GLToy_ConstIterate( u_int, uClusterIndex, m_xClusters[ pxLeaf->m_uCluster ].m_xPVS )
            GLToy_Assert( uClusterIndex < m_xClusters.GetCount(), "Cluster index is too large!" );
            
            GLToy_ConstIterate( u_int, uLeafIndex, m_xClusters[ uClusterIndex ].m_xIndices )
                GetLeaf< GLToy_EnvironmentLeaf_Lightmapped >( uLeafIndex )->RenderLightmap();
            GLToy_Iterate_End;
        GLToy_Iterate_End;
    }
    else
    {
        // fallback - render with no bsp tree or visibilty culling
        GLToy_ConstIterate( GLToy_Environment_LightmappedFace, xFace, m_xFaces )
            if( !xFace.m_bVisible || xFace.m_bRenderedLightmap )
            {
                continue;
            }

            xFace.m_bRenderedLightmap = true;

            GLToy_Texture_System::BindTexture( xFace.m_uLightmapHash );

            GLToy_Render::StartSubmittingPolygon();

            GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );

            GLToy_ConstIterate( u_int, uIndex, xFace.m_xIndices )
                const GLToy_Environment_LightmappedFaceVertex& xVertex = m_xVertices[ uIndex ];

                GLToy_Render::SubmitUV( xVertex.m_xLightmapUV );
                GLToy_Render::SubmitVertex( xVertex.m_xPosition );
            GLToy_Iterate_End;

            GLToy_Render::EndSubmit();
        GLToy_Iterate_End;
    }
}

void GLToy_Environment_Lightmapped::RenderLighting() const
{
    if( !GLToy_Environment_System::IsRenderingLightmap() )
    {
        return;
    }

    GLToy_Render::EnableBackFaceCulling();
    GLToy_Render::SetCWFaceWinding();

    GLToy_EnvironmentLeaf_Lightmapped* pxLeaf = static_cast< GLToy_EnvironmentLeaf_Lightmapped* >( GetLeafData( GLToy_Camera::GetPosition() ) );
    if( !IsEmpty() && pxLeaf && pxLeaf->m_uCluster != 0xFFFF )
    {
        GLToy_ShaderProgram* const pxShader = GLToy_Shader_System::FindShader( GLToy_Hash_Constant( "Lightmap_Simple" ) );
        GLToy_Assert( pxShader != NULL, "Lightmap shader not found!" );
        if( !pxShader )
        {
            return;
        }

        pxShader->Bind();
        pxShader->SetUniform( "xLightmapSampler", 0 );
        pxShader->SetUniform( "xDiffuseSampler", 1 );
        pxShader->SetUniform( "xNormalSampler", 2 );

        GLToy_Render::BindDiffuseTexture( 1 );
        GLToy_Render::BindNormalTexture( 2 );

        const GLToy_Vector_2 xSize( static_cast< float >( GLToy::GetWindowViewportWidth() ), static_cast< float >( GLToy::GetWindowViewportHeight() ) );
        const GLToy_Vector_2 xOneOverSize( 1.0f / xSize[ 0 ], 1.0f / xSize[ 1 ] );

        pxShader->SetUniform( "xSize", xSize );
        pxShader->SetUniform( "xOneOverSize", xOneOverSize );

        GLToy_ConstIterate( u_int, uClusterIndex, m_xClusters[ pxLeaf->m_uCluster ].m_xPVS )
            GLToy_Assert( uClusterIndex < m_xClusters.GetCount(), "Cluster index is too large!" );
            
            GLToy_ConstIterate( u_int, uLeafIndex, m_xClusters[ uClusterIndex ].m_xIndices )
                m_xLeaves[ uLeafIndex ].RenderLighting();
            GLToy_Iterate_End;
        GLToy_Iterate_End;

        GLToy_Render::UseProgram( 0 );
    }

    GLToy_Render::DisableBackFaceCulling();
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
        GLToy_ConstIterate( u_int, uClusterIndex, m_xClusters[ pxLeaf->m_uCluster ].m_xPVS )
            GLToy_Assert( uClusterIndex < m_xClusters.GetCount(), "Cluster index is too large!" );
            
            GLToy_ConstIterate( u_int, uLeafIndex, m_xClusters[ uClusterIndex ].m_xIndices )
                GetLeaf< GLToy_EnvironmentLeaf_Lightmapped >( uLeafIndex )->RenderDebugFaceInfo();
            GLToy_Iterate_End;
        GLToy_Iterate_End;
    }
    else
    {
        // fallback - render with no bsp tree or visibilty culling
        GLToy_ConstIterate( GLToy_Environment_LightmappedFace, xFace, m_xFaces )
            if( !xFace.m_bVisible )
            {
                continue;
            }

            GLToy_Vector_3 xAverage = GLToy_Maths::ZeroVector3;
            GLToy_ConstIterate( u_int, uIndex, xFace.m_xIndices )
                xAverage += m_xVertices[ uIndex ].m_xPosition;
            GLToy_Iterate_End;
            xAverage /= static_cast< float >( xFace.m_xIndices.GetCount() );

            const GLToy_Vector_2 xPoint = GLToy_Camera::WorldSpaceToScreenSpace( xAverage );
            if( GLToy_Camera::IsPointOnScreen( xPoint ) )
            {
                GLToy_String xFaceString;
                xFaceString.SetToFormatString( "Face %d", xIterator.Index() );
                GLToy_Console::GetFont()->RenderString( xFaceString, xPoint[ 0 ], xPoint[ 1 ] );
            }
        GLToy_Iterate_End;
    }
}

u_int GLToy_Environment_Lightmapped::GetType() const
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
    GLToy_ConstIterate( GLToy_Environment_LightmappedFaceVertex, xVertex, m_xVertices )
        if( xVertex == xVertex )
        {
            return xIterator.Index();
        }
    GLToy_Iterate_End;

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
    GLToy_ConstIterate( u_int, uFaceIndex, m_xIndices )
        GLToy_Assert( uFaceIndex < m_pxParent->m_xFaces.GetCount(), "Bad face index: %d (max: %d)", uFaceIndex, m_pxParent->m_xFaces.GetCount() - 1 );

        const GLToy_Environment_LightmappedFace& xFace = m_pxParent->m_xFaces[ uFaceIndex ];

        if( !xFace.m_bVisible || xFace.m_bRendered )
        {
            continue;
        }

        xFace.m_bRendered = true;

        GLToy_Texture_System::BindTexture( xFace.m_uTextureHash );

        GLToy_Render::StartSubmittingPolygon();

        GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );

        GLToy_ConstIterate( u_int, uIndex, xFace.m_xIndices )
            const GLToy_Environment_LightmappedFaceVertex& xVertex = m_pxParent->m_xVertices[ uIndex ];

            GLToy_Render::SubmitUV( xVertex.m_xUV * ( bQuadRes ? 4.0f : 1.0f ) );
            GLToy_Render::SubmitVertex( xVertex.m_xPosition );
        GLToy_Iterate_End;

        GLToy_Render::EndSubmit();
    GLToy_Iterate_End;
}

void GLToy_EnvironmentLeaf_Lightmapped::RenderDeferred() const
{
    if( !m_pxParent )
    {
        return;
    }

    const bool bQuadRes = GLToy_Environment_System::IsBSPQuadRes();

    GLToy_ConstIterate( MaterialBatch, xBatch, m_xBatches )
        
        GLToy_Material_System::BindMaterial( xBatch.m_uHash );

        GLToy_ConstIterate( u_short, uFaceIndex, xBatch.m_xFaces )
            GLToy_Assert( uFaceIndex < m_pxParent->m_xFaces.GetCount(), "Bad face index: %d (max: %d)", uFaceIndex, m_pxParent->m_xFaces.GetCount() - 1 );

            const GLToy_Environment_LightmappedFace& xFace = m_pxParent->m_xFaces[ uFaceIndex ];

            if( !xFace.m_bVisible || xFace.m_bRendered )
            {
                continue;
            }

            xFace.m_bRendered = true;

            m_pxParent->m_pxIndexBuffer->SubmitPolygon(
                    xFace.m_xVertexBufferData.m_usMinIndex,
                    xFace.m_xVertexBufferData.m_usMaxIndex,
                    xFace.m_xVertexBufferData.m_usCount,
                    xFace.m_xVertexBufferData.m_usOffset
                );
        GLToy_Iterate_End;
    GLToy_Iterate_End;
    //GLToy_ConstIterate( u_int, uFaceIndex, m_xIndices )
    //    GLToy_Assert( uFaceIndex < m_pxParent->m_xFaces.GetCount(), "Bad face index: %d (max: %d)", uFaceIndex, m_pxParent->m_xFaces.GetCount() - 1 );

    //    const GLToy_Environment_LightmappedFace& xFace = m_pxParent->m_xFaces[ uFaceIndex ];

    //    if( !xFace.m_bVisible || xFace.m_bRendered )
    //    {
    //        continue;
    //    }

    //    xFace.m_bRendered = true;

    //    GLToy_Material_System::BindMaterial( xFace.m_uTextureHash );

    //    m_pxParent->m_pxIndexBuffer->SubmitPolygon(
    //            xFace.m_xVertexBufferData.m_usMinIndex,
    //            xFace.m_xVertexBufferData.m_usMaxIndex,
    //            xFace.m_xVertexBufferData.m_usCount,
    //            xFace.m_xVertexBufferData.m_usOffset
    //        );
    //GLToy_Iterate_End;
}

void GLToy_EnvironmentLeaf_Lightmapped::RenderLightmap() const
{
    if( !m_pxParent )
    {
        return;
    }

    GLToy_ConstIterate( u_int, uFaceIndex, m_xIndices )
        const GLToy_Environment_LightmappedFace& xFace = m_pxParent->m_xFaces[ uFaceIndex ];

        if( !xFace.m_bVisible || xFace.m_bRenderedLightmap )
        {
            continue;
        }

        xFace.m_bRenderedLightmap = true;

        GLToy_Texture_System::BindTexture( xFace.m_uLightmapHash );

        GLToy_Render::StartSubmittingPolygon();

        GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );

        GLToy_ConstIterate( u_int, uIndex, xFace.m_xIndices )
            const GLToy_Environment_LightmappedFaceVertex& xVertex = m_pxParent->m_xVertices[ uIndex ];

            GLToy_Render::SubmitUV( xVertex.m_xLightmapUV );
            GLToy_Render::SubmitVertex( xVertex.m_xPosition );
        GLToy_Iterate_End;

        GLToy_Render::EndSubmit();
    GLToy_Iterate_End;
}

void GLToy_EnvironmentLeaf_Lightmapped::RenderLighting() const
{
    if( !m_pxParent )
    {
        return;
    }

    GLToy_ConstIterate( u_int, uFaceIndex, m_xIndices )
        const GLToy_Environment_LightmappedFace& xFace = m_pxParent->m_xFaces[ uFaceIndex ];

        if( !xFace.m_bVisible || xFace.m_bRenderedLightmap )
        {
            continue;
        }

        xFace.m_bRenderedLightmap = true;

        GLToy_Texture_System::BindTexture( xFace.m_uLightmapHash );

        GLToy_Render::StartSubmittingPolygon();

        GLToy_ConstIterate( u_int, uIndex, xFace.m_xIndices )
            const GLToy_Environment_LightmappedFaceVertex& xVertex = m_pxParent->m_xVertices[ uIndex ];

            GLToy_Render::SubmitUV( xVertex.m_xLightmapUV );
            GLToy_Render::SubmitVertex( xVertex.m_xPosition );
        GLToy_Iterate_End;

        GLToy_Render::EndSubmit();
    GLToy_Iterate_End;
}

void GLToy_EnvironmentLeaf_Lightmapped::RenderDebugFaceInfo() const
{
    if( !m_pxParent )
    {
        return;
    }

    GLToy_ConstIterate( u_int, uFaceIndex, m_xIndices )
        const GLToy_Environment_LightmappedFace& xFace = m_pxParent->m_xFaces[ uFaceIndex ];

        if( !xFace.m_bVisible )
        {
            continue;
        }

        GLToy_Vector_3 xAverage = GLToy_Maths::ZeroVector3;
        GLToy_ConstIterate( u_int, uIndex, xFace.m_xIndices )
            xAverage += m_pxParent->m_xVertices[ uIndex ].m_xPosition;
        GLToy_Iterate_End;
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
    GLToy_Iterate_End;
}

void GLToy_EnvironmentLeaf_Lightmapped::RenderDebugNormals() const
{
    GLToy_ConstIterate( u_int, uFaceIndex, m_xIndices )
        const GLToy_Environment_LightmappedFace& xFace = m_pxParent->m_xFaces[ uFaceIndex ];

        if( !xFace.m_bVisible )
        {
            continue;
        }

        // render basis vectors...
        GLToy_ConstIterate( u_int, uFaceIndex, m_xIndices )
            GLToy_ConstIterate( u_int, uIndex, xFace.m_xIndices )
                    const GLToy_Environment_LightmappedFaceVertex& xVertex = m_pxParent->m_xVertices[ uIndex ];
                    GLToy_Render::SubmitColour( GLToy_Vector_3( 1.0f, 1.0f, 1.0f ) );
                    GLToy_Render::StartSubmittingLines();
                    GLToy_Render::SubmitVertex( xVertex.m_xPosition );
                    GLToy_Render::SubmitVertex( xVertex.m_xPosition + 20.0f * xVertex.m_xNormal );
                    GLToy_Render::EndSubmit();
                    GLToy_Render::SubmitColour( GLToy_Vector_3( 1.0f, 0.0f, 0.0f ) );
                    GLToy_Render::StartSubmittingLines();
                    GLToy_Render::SubmitVertex( xVertex.m_xPosition );
                    GLToy_Render::SubmitVertex( xVertex.m_xPosition + 20.0f * xVertex.m_xTangent );
                    GLToy_Render::EndSubmit();
                    GLToy_Render::SubmitColour( GLToy_Vector_3( 0.0f, 0.0f, 1.0f ) );
                    GLToy_Render::StartSubmittingLines();
                    GLToy_Render::SubmitVertex( xVertex.m_xPosition );
                    GLToy_Render::SubmitVertex( xVertex.m_xPosition + 20.0f * xVertex.m_xNormal.Cross( xVertex.m_xTangent ) );
                    GLToy_Render::EndSubmit();
            GLToy_Iterate_End;
        GLToy_Iterate_End;
    GLToy_Iterate_End;
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
