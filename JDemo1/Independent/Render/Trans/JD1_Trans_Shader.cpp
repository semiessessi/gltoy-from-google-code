/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
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
#include <Render/Trans/JD1_Trans_Shader.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture.h>
#include <Render/Shader/GLToy_Shader.h>
#include <Render/Shader/GLToy_Shader_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

JD1_Trans_Shader::JD1_Trans_Shader( const GLToy_Hash uShaderHash )
: m_uShader( uShaderHash )
{
    for( u_int u = 0; u < 8; ++u )
    {
        m_axTextures[ u ].First() = uGLTOY_BAD_HASH;
    }
}

void JD1_Trans_Shader::Render() const
{
    const GLToy_Vector_2 xSize( static_cast< float >( GLToy::GetWindowViewportWidth() ), static_cast< float >( GLToy::GetWindowViewportHeight() ) );
    const GLToy_Vector_2 xOneOverSize( 1.0f / xSize[ 0 ], 1.0f / xSize[ 1 ] );
    GLToy_Render::SetOrthogonalProjectionMatrix();
    GLToy_Render::PushViewMatrix();
    GLToy_Render::SetIdentityViewMatrix();
    GLToy_Render::DisableDepthWrites();
    GLToy_Render::DisableDepthTesting();
    GLToy_Render::DisableBlending();

    GLToy_Render::BindFrameBufferNoCopy();

    for( u_int u = 1; u < 8; ++u )
    {
        if( m_axTextures[ u ].First() != uGLTOY_BAD_HASH )
        {
            const GLToy_Texture* const pxTexture = GLToy_Texture_System::FindTexture( m_axTextures[ u ].First() );

            if( pxTexture )
            {
                pxTexture->Bind( u );
            }
            else
            {
                GLToy_Texture_System::BindWhite( u );
            }
        }
    }

    GLToy_ShaderProgram* const pxShader = GLToy_Shader_System::FindShader( m_uShader );
    
    if( pxShader )
    {
        // TODO: this fails to get the texture somehow
        pxShader->Bind();

        pxShader->SetUniform( "xFrameBuffer", 0 );
        pxShader->SetUniform( "xSize", xSize );
        pxShader->SetUniform( "xOneOverSize", xOneOverSize );
        pxShader->SetUniform( "fTime", GLToy_Timer::GetTime() );
        pxShader->SetUniform( "fLastFrameTime", GLToy_Timer::GetFrameTime() );

        SetUniforms();

        for( u_int u = 1; u < 8; ++u )
        {
            if( m_axTextures[ u ].First() != uGLTOY_BAD_HASH )
            {
                pxShader->SetUniform( m_axTextures[ u ].Second(), static_cast< int >( u ) );
            }
        }
    }

    GLToy_Render::StartSubmittingQuads();
    GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );
    GLToy_Render::SubmitTexturedQuad2D( GLToy_Vector_2( -0.5f * GLToy_Render::Get2DWidth(), -1.0f ), GLToy_Vector_2( GLToy_Render::Get2DWidth(), 2.0f ), 0.0f, 1.0f, 1.0f, 0.0f );
    GLToy_Render::EndSubmit();

    GLToy_Render::EnableDepthTesting();
    GLToy_Render::EnableDepthWrites();
    GLToy_Render::SetPerspectiveProjectionMatrix();
    GLToy_Render::PopViewMatrix();

    GLToy_Render::UseProgram( 0 );
}

void JD1_Trans_Shader::BindTexture( const GLToy_String& szUniformName, const GLToy_Hash uTextureHash, const u_int uTextureUnit )
{
    GLToy_Assert( uTextureUnit < 8, "Texture unit %d is not valid", uTextureUnit );
    GLToy_Assert( uTextureUnit == 0, "Texture unit 0 is reserved for the framebuffer", uTextureUnit );

    m_axTextures[ uTextureUnit ].First() = uTextureHash;
    m_axTextures[ uTextureUnit ].Second() = szUniformName;
}
