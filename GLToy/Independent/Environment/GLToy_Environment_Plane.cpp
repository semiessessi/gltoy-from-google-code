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
#include <Environment/GLToy_Environment_Plane.h>

// GLToy
#include <Environment/GLToy_Environment_System.h>
#include <Maths/GLToy_Maths.h>
#include <Physics/GLToy_Physics_System.h>
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
    GLToy_Physics_System::Reset();
    m_pxPhysicsObject = GLToy_Physics_System::CreatePhysicsPlane( GLToy_Hash_Constant( "Environment" ), GLToy_Plane( GLToy_Vector_3( 0.0f, 1.0f, 0.0f ), 0.0f ) );
}

void GLToy_Environment_Plane::Shutdown()
{
    GLToy_Texture_System::DestroyTexture( m_uTextureHash );
    GLToy_Physics_System::Reset();
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

    GLToy_Render::SubmitUV( GLToy_Vector_3( -1.0f * fAspectRatio, -1.0f, -1.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( -1.0f, -1.0f, 0.0f ) );

    GLToy_Render::SubmitUV( GLToy_Vector_3( 1.0f * fAspectRatio, -1.0f, -1.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( 1.0f, -1.0f, 0.0f ) );

    GLToy_Render::SubmitUV( GLToy_Vector_3( 1.0f * fAspectRatio, 1.0f, -1.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( 1.0f, 1.0f, 0.0f ) );

    GLToy_Render::SubmitUV( GLToy_Vector_3( -1.0f * fAspectRatio, 1.0f, -1.0f ) );
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

float GLToy_Environment_Plane::Trace( const GLToy_Ray& xRay, const float fLimitingDistance ) const
{
    float fDistance = 0.0f;
    bool bHit = xRay.IntersectsWithPlane( m_xPlane, &fDistance );
    return bHit ? ( ( fLimitingDistance > 0.0f ) ? GLToy_Maths::Min( fDistance, fLimitingDistance ) : fDistance ) : -1.0f;
}