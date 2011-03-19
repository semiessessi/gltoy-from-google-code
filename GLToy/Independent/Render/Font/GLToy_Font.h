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

template < class T > class GLToy_HashMap;

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
    GLToy_Inline float GetWidth() const { return GetHeight() * 0.55f; }
    GLToy_Inline float GetHeight() const { return static_cast< float >( m_uSize ) / 256.0f; }

    virtual void Initialise() = 0;
    virtual void Shutdown() = 0;

    virtual void RenderString( const GLToy_String& szString, const float fX, const float fY ) const { RenderString( szString, fX, fY, GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) ); }
    virtual void RenderString( const GLToy_String& szString, const float fX, const float fY, const GLToy_Vector_4& xColour ) const = 0;

    virtual void RenderStringDown( const GLToy_String& szString, const float fX, const float fY ) const { RenderStringDown( szString, fX, fY, GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) ); }
    virtual void RenderStringDown( const GLToy_String& szString, const float fX, const float fY, const GLToy_Vector_4& xColour ) const = 0;

    virtual void RenderStringUp( const GLToy_String& szString, const float fX, const float fY ) const { RenderStringDown( szString, fX, fY, GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) ); }
    virtual void RenderStringUp( const GLToy_String& szString, const float fX, const float fY, const GLToy_Vector_4& xColour ) const = 0;

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

    static void RenderString( const GLToy_String& szString, const GLToy_String& xFontName, const float fX, const float fY, GLToy_Vector_4 xColour = 0xFFFFFFFF );

private:

    static GLToy_HashMap< GLToy_Font* > s_xFonts;

};

#endif
