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

    GLToy_Inline GLToy_Hash GetHash() const { return m_szName.GetHash(); }

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

    void LoadTGA();

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
    static GLToy_Texture* LookUpTextureNoExt( const GLToy_String& szName );
    static void CreateTexture( const GLToy_Hash uHash );
    static void CreateTexture( const GLToy_String& szName );
    static void DestroyTexture( const GLToy_Hash uHash );
    static void DestroyTexture( const GLToy_String& szName );
    static void BindTexture( const GLToy_Hash uHash, const u_int uTextureUnit = 0 );
    static void BindTexture( const GLToy_String& szName, const u_int uTextureUnit = 0 );
    static void Reset();

    static void BindBlack( const u_int uTextureUnit = 0 );
    static void BindWhite( const u_int uTextureUnit = 0 );

    static void CreateFrameBufferTexture( u_int& uID, const int iWidth, const int iHeight ) { Platform_CreateFrameBufferTexture( uID, iWidth, iHeight ); }
    static void DestroyFrameBufferTexture( u_int& uID ) { Platform_DestroyFrameBufferTexture( uID ); }
    static void BindFrameBufferTexture( const u_int uID ) { Platform_BindFrameBufferTexture( uID ); }

    // TODO - find a better solution for lightmaps = creating from the hash directly leaves the name blank, which might be flakey
    static void CreateTextureFromRGBAData( const GLToy_Hash uHash, u_int* const puData, const u_int uWidth, const u_int uHeight );
    static void CreateTextureFromRGBData( const GLToy_Hash uHash, u_char* const pucData, const u_int uWidth, const u_int uHeight );

    static void SaveTextureBMP( const GLToy_String& szName, u_int* const puData, const u_int uWidth, const u_int uHeight ) { Platform_SaveTextureBMP( szName, puData, uWidth, uHeight ); }
    static void SaveTextureTGA( const GLToy_String& szName, u_int* const puData, const u_int uWidth, const u_int uHeight ) { Platform_SaveTextureTGA( szName, puData, uWidth, uHeight ); }

private:

    static bool Platform_Initialise();
    static void Platform_Shutdown();

    static void Platform_SaveTextureBMP( const GLToy_String& szName, u_int* const puData, const u_int uWidth, const u_int uHeight );
    static void Platform_SaveTextureTGA( const GLToy_String& szName, u_int* const puData, const u_int uWidth, const u_int uHeight );

    static void Platform_CreateFrameBufferTexture( u_int& uID, const int iWidth, const int iHeight );
    static void Platform_DestroyFrameBufferTexture( u_int& uID );
    static void Platform_BindFrameBufferTexture( const u_int uID );

    static GLToy_HashTree< GLToy_Texture > s_xTextures;

    static GLToy_Texture* s_pxBlackTexture;
    static GLToy_Texture* s_pxWhiteTexture;

};

#endif
