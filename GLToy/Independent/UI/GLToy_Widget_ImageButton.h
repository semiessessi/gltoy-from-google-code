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

#ifndef __GLTOY_WIDGET_IMAGEBUTTON_H_
#define __GLTOY_WIDGET_IMAGEBUTTON_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <UI/GLToy_Widget.h>

// GLToy
#include <Core/GLToy_Hash.h>
#include <String/GLToy_String.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Widget_ImageButton
: public GLToy_Widget
{

    typedef GLToy_Widget GLToy_Parent;

public:

    GLToy_Widget_ImageButton( const GLToy_WidgetType eType, const float fX = 0.0f, const float fY = 0.0f, const float fWidth = 0.1f, const float fHeight = 0.1f )
    : GLToy_Parent( eType, fX, fY, fWidth, fHeight )
    , m_uTextureHash( uGLTOY_BAD_HASH )
    , m_szString()
    , m_uFont( uGLTOY_BAD_HASH )
    {
    }

    virtual ~GLToy_Widget_ImageButton() {}

    virtual void Render2D() const;

    GLToy_Inline void SetTexture( const GLToy_String& szString ) { m_uTextureHash = szString.GetHash(); }
    GLToy_Inline void SetTexture( const GLToy_Hash& uHash ) { m_uTextureHash = uHash; }

    GLToy_Inline void SetLabelString( const GLToy_String& szString ) { m_szString = szString; }
    GLToy_Inline const GLToy_String& GetLabelString() const { return m_szString; }

    GLToy_Inline void SetFont( const GLToy_String& szString ) { m_uFont = szString.GetHash(); }
    GLToy_Inline GLToy_Hash GetFont() const { return m_uFont; }
protected:

    GLToy_Hash m_uTextureHash;
    GLToy_String m_szString;
    GLToy_Hash m_uFont;

    void ( *m_pfnCallback )( void* const pData );
};

#endif
