/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/RTToy.h>

// This file's header
#include <Environment/RTToy_Environment_Plane.h>

// GLToy
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture.h>

// RTToy
#include <Render/Shader/RTToy_Shader_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void RTToy_Environment_Plane::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    xStream >> m_xPlane;
    xStream >> m_uTextureHash;
}

void RTToy_Environment_Plane::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    xStream << m_xPlane;
    xStream << m_uTextureHash;
}

void RTToy_Environment_Plane::Initialise()
{
    GLToy_Texture_System::CreateTexture( m_uTextureHash );
}

void RTToy_Environment_Plane::Shutdown()
{
    GLToy_Texture_System::DestroyTexture( m_uTextureHash );
}

void RTToy_Environment_Plane::Render() const
{
    GLToy_Texture* pxTexture = GLToy_Texture_System::FindTexture( m_uTextureHash );

    if( pxTexture )
    {
        pxTexture->Bind();
    }
    else
    {
        GLToy_Texture_System::BindWhite();
    }

    RTToy_Shader_System::BindShaderProgram( GLToy_Hash_Constant( "Raytrace_Plane" ) );

    const float fAspectRatio = static_cast< float >( GLToy::GetWindowViewportWidth() ) / static_cast< float >( GLToy::GetWindowViewportHeight() );

    GLToy_Render::StartSubmittingQuads();

    GLToy_Render::SubmitNormal( GLToy_Camera::GetPosition() );

    GLToy_Render::SubmitTextureCoordinate( GLToy_Vector_3( -1.0f * fAspectRatio, -1.0f, -1.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( -1.0f, -1.0f, 0.0f ) );

    GLToy_Render::SubmitTextureCoordinate( GLToy_Vector_3( 1.0f * fAspectRatio, -1.0f, -1.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( 1.0f, -1.0f, 0.0f ) );

    GLToy_Render::SubmitTextureCoordinate( GLToy_Vector_3( 1.0f * fAspectRatio, 1.0f, -1.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( 1.0f, 1.0f, 0.0f ) );

    GLToy_Render::SubmitTextureCoordinate( GLToy_Vector_3( -1.0f * fAspectRatio, 1.0f, -1.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( -1.0f, 1.0f, 0.0f ) );

    GLToy_Render::EndSubmit();
}

void RTToy_Environment_Plane::Update()
{
}