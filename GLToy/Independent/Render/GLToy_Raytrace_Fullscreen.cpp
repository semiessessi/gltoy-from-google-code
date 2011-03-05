/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010, 2011 Semi Essessi
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
#include <Render/GLToy_Raytrace_Fullscreen.h>

// GLToy
#include <Maths/GLToy_Maths.h>
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture_System.h>
#include <Render/Shader/GLToy_Shader.h>
#include <Render/Shader/GLToy_Shader_System.h>
#include <String/GLToy_String.h> 

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Raytrace_Fullscreen::GLToy_Raytrace_Fullscreen( const GLToy_Hash uShaderHash, const bool bDepthBuffered )
: m_uShader( uShaderHash )
, m_bDepthBuffered( bDepthBuffered )
, m_xVector4Uniforms()
{
    for( u_int u = 0; u < 8; ++u )
    {
        m_axTextures[ u ].First().First() = uGLTOY_BAD_HASH;
        m_axTextures[ u ].First().Second() = 0xFFFFFFFF;
    }
}

void GLToy_Raytrace_Fullscreen::Render() const
{
    if( !m_bDepthBuffered )
    {
        GLToy_Render::DisableDepthWrites();
        GLToy_Render::DisableDepthTesting();
    }

    for( u_int u = 0; u < 8; ++u )
    {
        GLToy_Texture* pxTexture = ( m_axTextures[ u ].First().First() == uGLTOY_BAD_HASH ) ? NULL : GLToy_Texture_System::FindTextureInternal( m_axTextures[ u ].First().First() );

        if( pxTexture )
        {
            pxTexture->Bind( u );
        }
        else if( m_axTextures[ u ].First().Second() != 0xFFFFFFFF )
        {
            GLToy_Texture_System::BindFrameBufferTexture( m_axTextures[ u ].First().Second(), u );
        }
        else
        {
            GLToy_Texture_System::BindWhite( u );
        }
    }

    GLToy_ShaderProgram* pxShader = GLToy_Shader_System::FindShader( m_uShader );
    
    if( pxShader )
    {
        pxShader->Bind();

        for( u_int u = 0; u < 8; ++u )
        {
            if( ( m_axTextures[ u ].First().First() != uGLTOY_BAD_HASH ) || ( m_axTextures[ u ].First().Second() != 0xFFFFFFFF ) )
            {
                pxShader->SetUniform( m_axTextures[ u ].Second(), static_cast< int >( u ) );
            }
        }
    }

	GLToy_ConstIterate( Vector4Uniform, xCurrent, m_xVector4Uniforms )
		pxShader->SetUniform( xCurrent.First(), *( xCurrent.Second() ) );
    GLToy_Iterate_End;
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

    if( !m_bDepthBuffered )
    {
        GLToy_Render::EnableDepthWrites();
        GLToy_Render::EnableDepthTesting();
    }
}

void GLToy_Raytrace_Fullscreen::BindTexture( const GLToy_String& szUniformName, const GLToy_Hash uTextureHash, const u_int uTextureUnit ) const
{
    GLToy_Assert( uTextureUnit < 8, "Texture unit %d is not valid", uTextureUnit );

    m_axTextures[ uTextureUnit ].First().First() = uTextureHash;
    m_axTextures[ uTextureUnit ].First().Second() = 0xFFFFFFFF;
    m_axTextures[ uTextureUnit ].Second() = szUniformName;
}

void GLToy_Raytrace_Fullscreen::BindFrameBufferTexture( const GLToy_String& szUniformName, const u_int uTextureID, const u_int uTextureUnit ) const
{
    GLToy_Assert( uTextureUnit < 8, "Texture unit %d is not valid", uTextureUnit );

    m_axTextures[ uTextureUnit ].First().First() = uGLTOY_BAD_HASH;
    m_axTextures[ uTextureUnit ].First().Second() = uTextureID;
    m_axTextures[ uTextureUnit ].Second() = szUniformName;
}

void GLToy_Raytrace_Fullscreen::BindUniform( const GLToy_String& szUniformName, const GLToy_Vector_4* const pxVector ) const
{
	const GLToy_Hash uHash = szUniformName.GetHash();
	if( pxVector && !m_xVector4Uniforms.FindData( uHash ) )
	{
		m_xVector4Uniforms.AddNode( Vector4Uniform( szUniformName, pxVector ), uHash );
	}
	else
	{
		GLToy_Assert( pxVector != NULL, "Passing in null vector pointer!" );
		GLToy_Assert( !m_xVector4Uniforms.FindData( uHash ), "Uniform already bound!" );
	}
}