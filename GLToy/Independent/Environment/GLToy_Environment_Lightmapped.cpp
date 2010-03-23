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

void GLToy_Environment_Lightmapped::Initialise()
{
}

void GLToy_Environment_Lightmapped::Shutdown()
{
}

void GLToy_Environment_Lightmapped::Render() const
{
    // placeholder
    GLToy_ConstIterate( GLToy_Environment_LightmappedFace, xIterator, &m_xFaces )
    {
        const GLToy_Environment_LightmappedFace& xFace = xIterator.Current();

        GLToy_Texture_System::BindTexture( xFace.m_uTextureHash );

        GLToy_Render::StartSubmittingPolygon();

        GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );

        GLToy_ConstIterate( u_int, xVertexIterator, &( xFace.m_xVertexIndices ) )
        {
            GLToy_Render::SubmitTextureCoordinate( xFace.m_xTexCoords[ xVertexIterator.Index() ] );
            GLToy_Render::SubmitVertex( m_xVertices[ xVertexIterator.Current() ] );
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