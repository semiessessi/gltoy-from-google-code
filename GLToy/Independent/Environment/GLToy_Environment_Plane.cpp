/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Environment/GLToy_Environment_Plane.h>

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

void GLToy_Environment_Plane::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    xStream >> m_xPlane;
    xStream >> m_uTextureHash;
}

void GLToy_Environment_Plane::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    xStream << m_xPlane;
    xStream << m_uTextureHash;
}

void GLToy_Environment_Plane::Initialise()
{
    GLToy_Texture_System::CreateTexture( m_uTextureHash );
}

void GLToy_Environment_Plane::Shutdown()
{
    GLToy_Texture_System::DestroyTexture( m_uTextureHash );
}

void GLToy_Environment_Plane::Render() const
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

	GLToy_ShaderProgram* pxShader = GLToy_Shader_System::FindShader( GLToy_Hash_Constant( "Raytrace_Plane" ) );
	
	if( pxShader )
	{
		pxShader->Bind();
		pxShader->SetUniform( "xTexture", 0 );
	}

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

	GLToy_Render::UseProgram( 0 );
}

void GLToy_Environment_Plane::Update()
{
}

int GLToy_Environment_Plane::GetType() const
{
    return static_cast< int >( ENV_PLANE );
}