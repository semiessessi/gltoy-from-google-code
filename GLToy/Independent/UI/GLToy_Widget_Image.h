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

#ifndef __GLTOY_WIDGET_IMAGE_H_
#define __GLTOY_WIDGET_IMAGE_H_

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

class GLToy_Widget_Image
: public GLToy_Widget
{

    typedef GLToy_Widget GLToy_Parent;

public:

    GLToy_Widget_Image( const GLToy_WidgetType eType, const float fX = 0.0f, const float fY = 0.0f, const float fWidth = 0.1f, const float fHeight = 0.1f )
    : GLToy_Parent( eType, fX, fY, fWidth, fHeight )
    , m_uTextureHash( uGLTOY_BAD_HASH )
    {
    }

    virtual ~GLToy_Widget_Image() {}

    virtual void Render2D() const;

    GLToy_Inline void SetTexture( const GLToy_String& szString ) { m_uTextureHash = szString.GetHash(); }
    GLToy_Inline void SetTexture( const GLToy_Hash& uHash ) { m_uTextureHash = uHash; }

protected:

    GLToy_Hash m_uTextureHash;

};

#endif
