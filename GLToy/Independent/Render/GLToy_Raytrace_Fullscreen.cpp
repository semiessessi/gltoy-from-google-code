/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010 Semi Essessi
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
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture.h>
#include <Render/Shader/GLToy_Shader.h>
#include <Render/Shader/GLToy_Shader_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Raytrace_Fullscreen::GLToy_Raytrace_Fullscreen( const GLToy_Hash uShaderHash )
: m_uShader( uShaderHash )
{
    for( u_int u = 0; u < 8; ++u )
    {
        m_axTextures[ u ].First() = uGLTOY_BAD_HASH;
    }
}

void GLToy_Raytrace_Fullscreen::Render() const
{
    for( u_int u = 0; u < 8; ++u )
    {
        if( m_axTextures[ u ].First() != uGLTOY_BAD_HASH )
        {
            GLToy_Texture* pxTexture = GLToy_Texture_System::FindTexture( m_axTextures[ u ].First() );

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

    GLToy_ShaderProgram* pxShader = GLToy_Shader_System::FindShader( m_uShader );
    
    if( pxShader )
    {
        pxShader->Bind();

        for( u_int u = 0; u < 8; ++u )
        {
            if( m_axTextures[ u ].First() != uGLTOY_BAD_HASH )
            {
                pxShader->SetUniform( m_axTextures[ u ].Second(), u );
            }
        }
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

void GLToy_Raytrace_Fullscreen::BindTexture( const GLToy_String& szUniformName, const GLToy_Hash uTextureHash, const u_int uTextureUnit )
{
    GLToy_Assert( uTextureUnit < 8, "Texture unit %d is not valid", uTextureUnit );

    m_axTextures[ uTextureUnit ].First() = uTextureHash;
    m_axTextures[ uTextureUnit ].Second() = szUniformName;
}
