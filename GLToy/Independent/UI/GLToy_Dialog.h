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

#ifndef __GLTOY_DIALOG_H_
#define __GLTOY_DIALOG_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/GLToy_Updateable.h>
#include <Render/GLToy_Renderable.h>

// GLToy
#include <Core/Data Structures/GLToy_Array.h>
#include <Maths/GLToy_Vector.h>
#include <UI/GLToy_DialogStyles.h>
#include <UI/GLToy_WidgetTypes.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_String;
class GLToy_Widget;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Dialog
: public GLToy_Renderable
, public GLToy_Updateable
{

public:

	GLToy_Dialog(
        const GLToy_DialogStyle ucStyle = DIALOG_STYLE_DEFAULT,
        const float fX = 0.0f, const float fY = 0.0f,
        const float fWidth = 0.1f, const float fHeight = 0.1f );

	virtual ~GLToy_Dialog();

    // we should never want to render these in 3D...
    virtual void Render() const {}
	virtual void Render2D() const;
	virtual void Update();

	GLToy_Inline float GetX() const { return m_xPosition[ 0 ]; }
	GLToy_Inline float GetY() const { return m_xPosition[ 1 ]; }
    GLToy_Inline const GLToy_Vector_2& GetPosition() const { return m_xPosition; }
	GLToy_Inline float GetWidth() const { return m_xSize[ 0 ]; }
	GLToy_Inline float GetHeight() const { return m_xSize[ 1 ]; }
    GLToy_Inline const GLToy_Vector_2& GetSize() const { return m_xSize; }

    GLToy_Inline GLToy_DialogStyle GetStyle() const { return m_ucStyle; }
    
    GLToy_Inline void SetPosition( const float fX, const float fY ) { m_xPosition[ 0 ] = fX; m_xPosition[ 1 ] = fY; }
    GLToy_Inline void SetPosition( const GLToy_Vector_2& xPosition ) { m_xPosition = xPosition; }
    GLToy_Inline void SetSize( const float fWidth, const float fHeight ) { m_xSize[ 0 ] = fWidth; m_xSize[ 1 ] = fHeight; }
    GLToy_Inline void SetSize( const GLToy_Vector_2& xSize ) { m_xSize = xSize; }

    void AddText( const GLToy_String& szString );
    void AddWidget( GLToy_Widget* const pxWidget );

protected:

	GLToy_DialogStyle m_ucStyle;
    GLToy_Vector_2 m_xPosition;
    GLToy_Vector_2 m_xSize;

	GLToy_Array< GLToy_Widget* > m_xWidgets;

};

#endif