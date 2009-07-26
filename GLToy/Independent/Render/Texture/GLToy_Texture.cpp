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
, m_uTextureID( uGLTOY_TEXTURE_BAD_ID )
{
    m_szName[0] = 0;
}

GLToy_Texture::~GLToy_Texture()
{
}

GLToy_Texture& GLToy_Texture::operator =( const GLToy_Texture& xTexture )
{
    m_bInVideoMemory = xTexture.m_bInVideoMemory;
    m_uTextureID = xTexture.m_uTextureID;

    // TODO: GLToy_String
    strcpy_s( m_szName, uGLTOY_TEXTURE_MAXNAME, xTexture.m_szName ); 

    return *this;
}
