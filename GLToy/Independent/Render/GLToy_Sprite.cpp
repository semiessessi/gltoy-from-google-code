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

    GLToy_Render::SubmitTextureCoordinate( GLToy_Vector_3( 1.0f, 1.0f, 0.0f ) );
    GLToy_Render::SubmitVertex( m_xPosition + ( xRight + xUp ) * m_fSize );
    GLToy_Render::SubmitTextureCoordinate( GLToy_Vector_3( 1.0f, 0.0f, 0.0f ) );
    GLToy_Render::SubmitVertex( m_xPosition + ( xRight - xUp ) * m_fSize );
    GLToy_Render::SubmitTextureCoordinate( GLToy_Vector_3( 0.0f, 0.0f, 0.0f ) );
    GLToy_Render::SubmitVertex( m_xPosition - ( xRight + xUp ) * m_fSize );
    GLToy_Render::SubmitTextureCoordinate( GLToy_Vector_3( 0.0f, 1.0f, 0.0f ) );
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