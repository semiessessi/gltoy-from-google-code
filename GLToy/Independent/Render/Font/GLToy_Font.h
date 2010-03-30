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

#ifndef __GLTOY_FONT_H_
#define __GLTOY_FONT_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Core/GLToy_Hash.h>
#include <Maths/GLToy_Vector.h>
#include <String/GLToy_String.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class T > class GLToy_HashTree;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Font
{

public:

    GLToy_Font( const GLToy_String& szName, const u_int uSize = 16 )
    : m_szName( szName )
    , m_uSize( uSize )
    {
    }

    virtual ~GLToy_Font() {}

    GLToy_Inline GLToy_Hash GetHash() const { return m_szName.GetHash(); }
    GLToy_Inline const GLToy_String& GetName() const { return m_szName; }
    GLToy_Inline u_int GetSize() const { return m_uSize; }
    GLToy_Inline float GetHeight() const { return static_cast< float >( m_uSize ) / 256.0f; }

    virtual void Initialise() = 0;
    virtual void Shutdown() = 0;

    virtual void RenderString( const GLToy_String& szString, const float fX, const float fY ) const { RenderString( szString, fX, fY, GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) ); }
    virtual void RenderString( const GLToy_String& szString, const float fX, const float fY, const GLToy_Vector_4& xColour ) const = 0;

protected:

    GLToy_String m_szName;
    u_int m_uSize;

};

class GLToy_Font_System
{

public:

    static bool Initialise();
    static void Shutdown();

    static GLToy_Font* FindFont( const GLToy_Hash uHash );
    static GLToy_Font* LookUpFont( const GLToy_String& xName );

    static void InitialiseFont( const GLToy_String& xName );
    static void ShutdownFont( const GLToy_String& xName );

    static void RenderString( const GLToy_String& szString, const GLToy_String& xFontName, const float fX, const float fY );

private:

    static GLToy_HashTree< GLToy_Font* > s_xFonts;

};

#endif