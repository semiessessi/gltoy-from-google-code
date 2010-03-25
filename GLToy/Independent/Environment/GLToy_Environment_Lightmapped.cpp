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

    xStream >> m_xVertices;
    //xStream >> m_xFaces;
    xStream >> m_xLightmapData;
}

void GLToy_Environment_Lightmapped::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    GLToy_Parent::WriteToBitStream( xStream );

    GLToy_Assert( false, "Need to finish WriteToBitStream for GLToy_Environment_Lightmapped" );

    xStream << m_xVertices;
    //xStream << m_xFaces;
    xStream << m_xLightmapData;
}

void GLToy_Environment_Lightmapped::Initialise()
{
}

void GLToy_Environment_Lightmapped::Shutdown()
{
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

    if( GLToy_Environment_System::IsRenderingLightmapOnly() )
    {
        RenderLightmap();
    }
    else
    {
        // placeholder
        GLToy_ConstIterate( GLToy_Environment_LightmappedFace, xIterator, &m_xFaces )
        {
            const GLToy_Environment_LightmappedFace& xFace = xIterator.Current();

            GLToy_Texture_System::BindTexture( xFace.m_uTextureHash );

            GLToy_Render::StartSubmittingPolygon();

            GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );

            GLToy_ConstIterate( GLToy_Environment_LightmappedFaceVertex, xVertexIterator, &( xFace.m_xVertices ) )
            {
                const GLToy_Environment_LightmappedFaceVertex& xVertex = xVertexIterator.Current();
                GLToy_Render::SubmitTextureCoordinate(
                    GLToy_Vector_3( 
                        xVertex.m_fU,
                        xVertex.m_fV,
                        0.0f ) );

                GLToy_Render::SubmitVertex( m_xVertices[ xVertex.m_uVertexIndex ] );
            }

            GLToy_Render::EndSubmit();
        }

        if( GLToy_Environment_System::IsRenderingLightmap() )
        {
            GLToy_Render::EnableBlending();
            GLToy_Render::SetBlendFunction( BLEND_ZERO, BLEND_SRC_COLOR );
            
            RenderLightmap();
            
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

        //GLToy_Texture_System::BindTexture( xFace.m_uTextureHash );
        const u_int uHashSource = 1337 * xIterator.Index();
        GLToy_Texture_System::BindTexture( _GLToy_GetHash( reinterpret_cast< const char* const >( &uHashSource ), 4 ) );

        GLToy_Render::StartSubmittingPolygon();

        GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );

        GLToy_ConstIterate( GLToy_Environment_LightmappedFaceVertex, xVertexIterator, &( xFace.m_xVertices ) )
        {
            const GLToy_Environment_LightmappedFaceVertex& xVertex = xVertexIterator.Current();
            GLToy_Render::SubmitTextureCoordinate(
                GLToy_Vector_3( 
                    xVertex.m_fLightmapU,
                    xVertex.m_fLightmapV,
                    0.0f ) );

            GLToy_Render::SubmitVertex( m_xVertices[ xVertex.m_uVertexIndex ] );
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