#ifndef __GLTOY_TEXTURE_H_
#define __GLTOY_TEXTURE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Texture
{

    friend class GLToy_TextureManager;

public:
    
    GLToy_Texture();
    virtual ~GLToy_Texture();

    GLToy_Texture& operator =( const GLToy_Texture& xTexture );

    GLToy_Hash GetID() const { return m_uTextureID; }

protected:

    bool m_bInVideoMemory;
    GLToy_Hash m_uTextureID;

};

#endif