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

void GLToy_Environment_Lightmapped::Render() const
{
    GLToy_Render::EnableBackFaceCulling();
    GLToy_Render::SetCWFaceWinding();

    // placeholder
    GLToy_ConstIterate( GLToy_Environment_LightmappedFace, xIterator, &m_xFaces )
    {
        const GLToy_Environment_LightmappedFace& xFace = xIterator.Current();

        GLToy_Texture_System::BindTexture( xFace.m_uTextureHash );
        //GLToy_Texture_System::BindTexture( _GLToy_GetHash( reinterpret_cast< const char* const >( &( xIterator.Index() ) ), 4 ) );

        GLToy_Render::StartSubmittingPolygon();

        GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );

        GLToy_ConstIterate( u_int, xVertexIterator, &( xFace.m_xVertexIndices ) )
        {
            GLToy_Render::SubmitTextureCoordinate(
                GLToy_Vector_3( 
                    xFace.m_xTexCoords[ xVertexIterator.Index() ][ 0 ],
                    xFace.m_xTexCoords[ xVertexIterator.Index() ][ 1 ],
                    0.0f ) );

            GLToy_Render::SubmitVertex( m_xVertices[ xVertexIterator.Current() ] );
        }

        GLToy_Render::EndSubmit();
    }

    GLToy_Render::DisableBackFaceCulling();

    RenderTransparent();
}

void GLToy_Environment_Lightmapped::RenderTransparent() const
{
    // TODO - for now abuse the transparent render for a second pass to do lightmaps
    GLToy_Render::EnableBlending();
    GLToy_Render::SetBlendFunction( BLEND_ZERO, BLEND_SRC_COLOR );
    GLToy_Render::EnableBackFaceCulling();
    GLToy_Render::SetCWFaceWinding();

    GLToy_Render::DisableDepthWrites();

    // placeholder
    GLToy_ConstIterate( GLToy_Environment_LightmappedFace, xIterator, &m_xFaces )
    {
        const GLToy_Environment_LightmappedFace& xFace = xIterator.Current();

        //GLToy_Texture_System::BindTexture( xFace.m_uTextureHash );
        GLToy_Texture_System::BindTexture( _GLToy_GetHash( reinterpret_cast< const char* const >( &( xIterator.Index() ) ), 4 ) );

        GLToy_Render::StartSubmittingPolygon();

        GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );

        GLToy_ConstIterate( u_int, xVertexIterator, &( xFace.m_xVertexIndices ) )
        {
            GLToy_Render::SubmitTextureCoordinate(
                GLToy_Vector_3( 
                    xFace.m_xTexCoords[ xVertexIterator.Index() ][ 0 ],
                    xFace.m_xTexCoords[ xVertexIterator.Index() ][ 1 ],
                    0.0f ) );

            GLToy_Render::SubmitVertex( m_xVertices[ xVertexIterator.Current() ] );
        }

        GLToy_Render::EndSubmit();
    }

    GLToy_Render::DisableBackFaceCulling();
    GLToy_Render::EnableDepthWrites();
    GLToy_Render::DisableBlending();
}

void GLToy_Environment_Lightmapped::Update()
{
}

int GLToy_Environment_Lightmapped::GetType() const
{
    return ENV_LIGHTMAPPED;
}