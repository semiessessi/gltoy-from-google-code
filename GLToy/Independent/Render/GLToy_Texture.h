#ifndef __GLTOY_TEXTURE_H_
#define __GLTOY_TEXTURE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/Data Structures/GLToy_Array.h>

// GLToy
#include <String/GLToy_String.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class T > class GLToy_HashTree;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Texture
: protected GLToy_SerialisableArray< u_int >
{

    friend class GLToy_Texture_System;

    typedef GLToy_SerialisableArray< u_int > GLToy_DataParent;

public:

    bool IsDataLoaded() const { return GetCount() > 0; }
    bool IsReadyForUse() const { return m_iID != -1; }

    u_int GetHeight() const { return m_uHeight; }
    u_int GetWidth() const { return m_uWidth; }

    GLToy_Inline u_int GetPixel( const u_int uX, const u_int uY ) const
    {
        return operator []( uX + uY * m_uWidth );
    }

    void Create();
    void Destroy();
    void Bind( const u_int uTextureUnit = 0 ) const;

protected:

    GLToy_Texture( const GLToy_String& szName )
    : GLToy_DataParent()
    , m_uWidth( 0 )
    , m_uHeight( 0 )
    , m_szName( szName )
    , m_iID( -1 )
    {
    }

    GLToy_Texture( const GLToy_String& szName, const u_int uRGBA )
    : GLToy_DataParent()
    , m_uWidth( 1 )
    , m_uHeight( 1 )
    , m_szName( szName )
    , m_iID( -1 )
    {
        Append( uRGBA );
    }


    void LoadFromFile();
    void Unload();

    void Platform_Create();
    void Platform_Destroy();
    void Platform_LoadFromFile();
    void Platform_Bind( const u_int uTextureUnit ) const;

    u_int m_uWidth;
    u_int m_uHeight;

    GLToy_String m_szName;
    union
    {
        int m_iID;
        u_int m_uUID;
    };

};

class GLToy_Texture_System
{

public:

    static bool Initialise();
    static void Shutdown();

    static GLToy_Texture* FindTexture( const GLToy_Hash uHash );
    static GLToy_Texture* LookUpTexture( const GLToy_String& szName );
    static void CreateTexture( const GLToy_Hash uHash );
    static void CreateTexture( const GLToy_String& szName );
    static void DestroyTexture( const GLToy_Hash uHash );
    static void DestroyTexture( const GLToy_String& szName );
    static void BindTexture( const GLToy_String& szName );
    static void Reset();

    static void BindWhite();

private:

    static bool Platform_Initialise();
    static void Platform_Shutdown();

    static GLToy_HashTree< GLToy_Texture > s_xTextures;

    static GLToy_Texture* s_pxWhiteTexture;

};

#endif