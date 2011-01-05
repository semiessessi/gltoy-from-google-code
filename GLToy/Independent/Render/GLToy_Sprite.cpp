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
#include <Render/GLToy_Sprite.h>

// GLToy
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Sprite::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    bool bAdditive = false;

    xStream >> m_xPosition;
    xStream >> m_xColour;
    xStream >> m_fSize;
    xStream >> bAdditive;
    xStream >> m_uTextureHash;

    m_ucBlendFunc = bAdditive ? 1 : 0;

    SetTexture( m_uTextureHash );
}

void GLToy_Sprite::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    xStream << m_xPosition;
    xStream << m_xColour;
    xStream << m_fSize;
    xStream << ( m_ucBlendFunc == 1 );
    xStream << m_uTextureHash;
}

void GLToy_Sprite::Render() const
{
    if( m_ucOrdering == ucSPRITE_ORDERED_ALWAYS )
    {
        GLToy_Render::RegisterTransparent( this, ( m_xPosition - GLToy_Camera::GetPosition() ).MagnitudeSquared() );
        return;
    }
    else if( m_ucOrdering == ucSPRITE_ORDERED_NEVER )
    {
        GLToy_Render::EnableBlending();
        GLToy_Render::EnableDepthTesting();
        GLToy_Render::DisableDepthWrites();

        RenderTransparent();

        GLToy_Render::EnableDepthWrites();
        return;
    }

    switch( m_ucBlendFunc )
    {
        default:
        case ucSPRITE_BLEND_NORMAL:
        {
            GLToy_Render::RegisterTransparent( this, ( m_xPosition - GLToy_Camera::GetPosition() ).MagnitudeSquared() );
            break;
        }
        
        case ucSPRITE_BLEND_ADDITIVE:
        {
            GLToy_Render::SetBlendFunction( BLEND_ONE, BLEND_ONE );
            GLToy_Render::EnableBlending();
            GLToy_Render::EnableDepthTesting();
            GLToy_Render::DisableDepthWrites();
            RenderTransparent();
            break;
        }
    }
}

void GLToy_Sprite::RenderTransparent() const
{
    switch( m_ucBlendFunc )
    {
        case ucSPRITE_BLEND_ADDITIVE:
        {
            GLToy_Render::SetBlendFunction( BLEND_ONE, BLEND_ONE );
            break;
        }

        default:
        case ucSPRITE_BLEND_NORMAL:
        {
            GLToy_Render::SetBlendFunction( BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA );
            break;
        }
    }

    GLToy_Texture_System::BindTexture( m_uTextureHash );

    // rotate xUp and xRight around the camera direction to allow for rotating sprites

    // TODO: my AVS artist instincts tell me to cache the trig calls for the rotations into some array
    // because we can't see the difference between 512 distinct rotations and floating point precision
    // and trig calls are expensive compared to memory lookup - here we are repeating a load of them :/

    const GLToy_Vector_3 xUp = GLToy_Maths::Rotate_AxisAngle( GLToy_Camera::GetUp(), GLToy_Camera::GetDirection(), m_fAngle );
    const GLToy_Vector_3 xRight = GLToy_Maths::Rotate_AxisAngle( GLToy_Camera::GetRight(), GLToy_Camera::GetDirection(), m_fAngle );

    GLToy_Render::StartSubmittingQuads();

    GLToy_Render::SubmitColour( GLToy_Vector_4( m_xColour, m_fMultiplier ) );

    GLToy_Render::SubmitUV( GLToy_Vector_3( 1.0f, 0.0f, 0.0f ) );
    GLToy_Render::SubmitVertex( m_xPosition + ( xRight + xUp ) * m_fSize );
    GLToy_Render::SubmitUV( GLToy_Vector_3( 1.0f, 1.0f, 0.0f ) );
    GLToy_Render::SubmitVertex( m_xPosition + ( xRight - xUp ) * m_fSize );
    GLToy_Render::SubmitUV( GLToy_Vector_3( 0.0f, 1.0f, 0.0f ) );
    GLToy_Render::SubmitVertex( m_xPosition - ( xRight + xUp ) * m_fSize );
    GLToy_Render::SubmitUV( GLToy_Vector_3( 0.0f, 0.0f, 0.0f ) );
    GLToy_Render::SubmitVertex( m_xPosition - ( xRight - xUp ) * m_fSize );

    GLToy_Render::EndSubmit();
}

void GLToy_Sprite::SetTexture( const GLToy_Hash uHash )
{
    m_uTextureHash = uHash;
    GLToy_Texture_System::CreateTexture( m_uTextureHash );
}

void GLToy_Sprite::SetTexture( const GLToy_String& szName )
{
    SetTexture( szName.GetHash() );
}
