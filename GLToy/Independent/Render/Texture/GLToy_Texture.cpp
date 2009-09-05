/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Render/Texture/GLToy_Texture.h>

// C/C++ headers
#include <string.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Texture::GLToy_Texture()
: m_bInVideoMemory( false )
, m_uTextureHash( uGLTOY_BAD_HASH )
, m_uTextureID( 0xFFFFFFFF )
{
}

GLToy_Texture::GLToy_Texture( const GLToy_Texture& xTexture )
: m_bInVideoMemory( xTexture.m_bInVideoMemory )
, m_uTextureHash( xTexture.m_uTextureHash )
, m_uTextureID( xTexture.m_uTextureID )
{
}

GLToy_Texture::~GLToy_Texture()
{
}

GLToy_Texture& GLToy_Texture::operator =( const GLToy_Texture& xTexture )
{
    m_bInVideoMemory = xTexture.m_bInVideoMemory;
    m_uTextureHash = xTexture.m_uTextureHash;
    m_uTextureID = xTexture.m_uTextureID;

    return *this;
}
