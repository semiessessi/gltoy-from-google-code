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
#include <Render/GLToy_Sprite.h>

// GLToy
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Sprite::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    bool bAdditive = false;

    xStream >> m_xPosition;
    xStream >> m_fSize;
    xStream >> bAdditive;
    xStream >> m_uTextureHash;

    m_ucBlendFunc = bAdditive ? 1 : 0;

    SetTexture( m_uTextureHash );
}

void GLToy_Sprite::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    xStream << m_xPosition;
    xStream << m_fSize;
    xStream << ( m_ucBlendFunc == 1 );
    xStream << m_uTextureHash;
}

void GLToy_Sprite::Render() const
{
    GLToy_Render::RegisterTransparent( this, ( m_xPosition - GLToy_Camera::GetPosition() ).MagnitudeSquared() );
}

void GLToy_Sprite::RenderTransparent() const
{
    switch( m_ucBlendFunc )
    {
        default:
        case ucSPRITE_BLEND_NORMAL:
        {
            GLToy_Render::SetBlendFunction( BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA );
            break;
        }
        
        case ucSPRITE_BLEND_ADDITIVE:
        {
            GLToy_Render::SetBlendFunction( BLEND_ONE, BLEND_ONE );
            break;
        }
    }

    GLToy_Texture_System::BindTexture( m_uTextureHash );

    const GLToy_Vector_3 xUp = GLToy_Camera::GetUp();
    const GLToy_Vector_3 xRight = GLToy_Camera::GetRight();

    GLToy_Render::StartSubmittingQuads();

    GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );

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