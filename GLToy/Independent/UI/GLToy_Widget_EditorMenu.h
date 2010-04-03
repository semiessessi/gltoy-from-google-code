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

#ifndef __GLTOY_WIDGET_EDITORMENU_H_
#define __GLTOY_WIDGET_EDITORMENU_H_

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

class GLToy_Widget_EditorMenu
: public GLToy_Widget
{

    typedef GLToy_Widget GLToy_Parent;

public:

    GLToy_Widget_EditorMenu( const GLToy_WidgetType eType, const float fX = 0.0f, const float fY = 0.0f, const float fWidth = 0.1f, const float fHeight = 0.1f )
    : GLToy_Parent( eType, fX, fY, fWidth, fHeight )
    {
    }

    virtual ~GLToy_Widget_EditorMenu() {}

    virtual void Render2D() const;
    virtual void Update();

};

#endif
