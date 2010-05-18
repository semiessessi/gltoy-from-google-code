/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010 Semi Essessi
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
    static void BindTexture( const GLToy_Hash uHash );
    static void BindTexture( const GLToy_String& szName );
    static void Reset();

    static void BindWhite();

    // TODO - find a better solution for lightmaps = creating from the hash directly leaves the name blank, which might be flakey
    static void CreateTextureFromRGBAData( const GLToy_Hash uHash, u_int* const puData, const u_int uWidth, const u_int uHeight );
    static void CreateTextureFromRGBData( const GLToy_Hash uHash, u_char* const pucData, const u_int uWidth, const u_int uHeight );

private:

    static bool Platform_Initialise();
    static void Platform_Shutdown();

    static GLToy_HashTree< GLToy_Texture > s_xTextures;

    static GLToy_Texture* s_pxWhiteTexture;

};

#endif
