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
#include <UI/GLToy_WidgetTypes.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Widget;

/////////////////////////////////////////////////////////////////////////////////////////////
// T Y P E D E F S
/////////////////////////////////////////////////////////////////////////////////////////////

typedef u_char GLToy_DialogStyle;

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const GLToy_DialogStyle DIALOG_STYLE_DEFAULT = 0;
static const GLToy_DialogStyle DIALOG_STYLE_CENTRE = 1;
static const GLToy_DialogStyle DIALOG_STYLE_MODAL = 2;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Dialog
: public GLToy_Renderable
, public GLToy_Updateable
{

public:

	GLToy_Dialog();
	virtual ~GLToy_Dialog() {}

	virtual void Render2D() const;
	virtual void Update();

protected:

	GLToy_DialogStyle m_ucStyle;

	GLToy_Array< GLToy_Widget* > m_xWidgets;

};

#endif