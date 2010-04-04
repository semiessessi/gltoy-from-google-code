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
#include <Environment/GLToy_Environment_System.h>
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
}

void GLToy_Environment_Lightmapped::Shutdown()
{
    // clean up lightmap textures
    GLToy_ConstIterate( GLToy_Environment_LightmappedFace, xIterator, &m_xFaces )
    {
        const u_int uHashSource = 1337 * xIterator.Index() + 7;
        GLToy_Texture_System::DestroyTexture( _GLToy_GetHash( reinterpret_cast< const char* const >( &uHashSource ), 4 ) );
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

    if( GLToy_Environment_System::IsRenderingLightmapOnly() )
    {
        RenderLightmap();
    }
    else
    {
        // placeholder
        const bool bQuadRes = GLToy_Environment_System::IsBSPQuadRes();
        GLToy_ConstIterate( GLToy_Environment_LightmappedFace, xIterator, &m_xFaces )
        {
            const GLToy_Environment_LightmappedFace& xFace = xIterator.Current();

            GLToy_Texture_System::BindTexture( xFace.m_uTextureHash );

            GLToy_Render::StartSubmittingPolygon();

            GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );

            GLToy_ConstIterate( u_int, xIndexIterator, &( xFace.m_xIndices ) )
            {
                const GLToy_Environment_LightmappedFaceVertex& xVertex = m_xVertices[ xIndexIterator.Current() ];

                GLToy_Render::SubmitUV( xVertex.m_xUV * ( bQuadRes ? 4.0f : 1.0f ) );
                GLToy_Render::SubmitVertex( xVertex.m_xVertex );
            }

            GLToy_Render::EndSubmit();
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
    // placeholder
    GLToy_ConstIterate( GLToy_Environment_LightmappedFace, xIterator, &m_xFaces )
    {
        const GLToy_Environment_LightmappedFace& xFace = xIterator.Current();

        const u_int uHashSource = 1337 * xIterator.Index() + 7;
        GLToy_Texture_System::BindTexture( _GLToy_GetHash( reinterpret_cast< const char* const >( &uHashSource ), 4 ) );

        GLToy_Render::StartSubmittingPolygon();

        GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );

        GLToy_ConstIterate( u_int, xIndexIterator, &( xFace.m_xIndices ) )
        {
            const GLToy_Environment_LightmappedFaceVertex& xVertex = m_xVertices[ xIndexIterator.Current() ];

            GLToy_Render::SubmitUV( xVertex.m_xLightmapUV );
            GLToy_Render::SubmitVertex( xVertex.m_xVertex );
        }

        GLToy_Render::EndSubmit();
    }
}

void GLToy_Environment_Lightmapped::Update()
{
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