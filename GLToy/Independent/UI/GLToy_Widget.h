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

#ifndef __GLTOY_WIDGET_H_
#define __GLTOY_WIDGET_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/GLToy_Updateable.h>
#include <Render/GLToy_Renderable.h>

// GLToy
#include <Core/GLToy_Hash.h>
#include <Maths/GLToy_Vector.h>
#include <UI/GLToy_WidgetTypes.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Widget
: public GLToy_Renderable
, public GLToy_Updateable
{

    friend class GLToy_Dialog;

public:

    GLToy_Widget()
    : m_xPosition()
    , m_xSize()
    , m_eType( WIDGET_NULL )
    , m_pxParent( NULL )
    {

    }

    GLToy_Widget(
        const GLToy_WidgetType eType,
        const float fX = 0.0f, const float fY = 0.0f,
        const float fWidth = 0.1f, const float fHeight = 0.1f )
    : m_xPosition( fX, fY )
    , m_xSize( fWidth, fHeight )
    , m_eType( eType )
    , m_pxParent( NULL )
    {
    }

    // TODO - I think this can be safely deleted
    GLToy_Widget( const GLToy_Widget& xWidget )
    : m_xPosition()
    , m_xSize()
    , m_eType( WIDGET_NULL )
    , m_pxParent( NULL )
    {
    }

    virtual ~GLToy_Widget() {}

    // widgets only need the 2d render function...
    // if they go on 3d stuff it should be by rendering to a texture
    virtual void Render() const {}
    
    virtual void Update() {}

    virtual void Execute() {}

    GLToy_Inline float GetX() const { return m_xPosition[ 0 ]; }
    GLToy_Inline float GetY() const { return m_xPosition[ 1 ]; }
    GLToy_Inline const GLToy_Vector_2& GetPosition() const { return m_xPosition; }
    GLToy_Inline float GetWidth() const { return m_xSize[ 0 ]; }
    GLToy_Inline float GetHeight() const { return m_xSize[ 1 ]; }
    GLToy_Inline const GLToy_Vector_2& GetSize() const { return m_xSize; }

    GLToy_Inline GLToy_WidgetType GetType() const { return m_eType; }
    
    GLToy_Inline void SetPosition( const float fX, const float fY ) { m_xPosition[ 0 ] = fX; m_xPosition[ 1 ] = fY; }
    GLToy_Inline void SetPosition( const GLToy_Vector_2& xPosition ) { m_xPosition = xPosition; }
    GLToy_Inline void SetSize( const float fWidth, const float fHeight ) { m_xSize[ 0 ] = fWidth; m_xSize[ 1 ] = fHeight; }
    GLToy_Inline void SetSize( const GLToy_Vector_2& xSize ) { m_xSize = xSize; }

    GLToy_Inline const GLToy_Dialog* GetParent() const { return m_pxParent; }

protected:

    GLToy_Inline void SetParent( const GLToy_Dialog* const pxParent ) { m_pxParent = pxParent; }

    GLToy_Vector_2 m_xPosition;
    GLToy_Vector_2 m_xSize;
    GLToy_WidgetType m_eType;

    const GLToy_Dialog* m_pxParent;

};

#endif
