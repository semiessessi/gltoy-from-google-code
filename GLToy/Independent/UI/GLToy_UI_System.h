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

#ifndef __GLTOY_UI_SYSTEM_H_
#define __GLTOY_UI_SYSTEM_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <UI/GLToy_DialogStyles.h>
#include <UI/GLToy_WidgetTypes.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class T > class GLToy_Array;
class GLToy_Dialog;
class GLToy_String;
class GLToy_Vector_2;
class GLToy_Widget;
class GLToy_Widget_Image;
class GLToy_Widget_ImageButton;
class GLToy_Widget_Label;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_UI_System
{

    friend class GLToy_Dialog;

public:

    static bool Initialise();
    static void Shutdown();

	static void Render2D();
	static void Update();

    static void ClearWidgets();

    static void ShowPointer( const bool bShow = true );
    static bool IsPointerShown() { return s_bShowPointer; }

    static GLToy_Widget_Label* CreateLabel( const GLToy_String& szLabel, const float fX, const float fY );
    static GLToy_Widget_Image* CreateImage( const GLToy_String& szTexture, const float fX, const float fY, const float fWidth, const float fHeight );
    static GLToy_Widget_ImageButton* CreateImageButton(
		const GLToy_String& szTexture,
		const GLToy_String& szLabel,
		void ( * const pfnCallback )( void* const pData ),
		const float fX, const float fY,
		const float fWidth, const float fHeight );

    static GLToy_Dialog* CreateDialog( const GLToy_DialogStyle ucStyle, const float fX = 0.0f, const float fY = 0.0f, const float fWidth = 0.1f, const float fHeight = 0.1f );
    static void DestroyCurrentModalDialog();
    static void ShowQuitDialog();

	static const GLToy_Vector_2& GetMousePosition() { return s_xMousePosition; }
	static float GetPulse();

private:

    static GLToy_Widget* CreateWidget( const GLToy_WidgetType eType, const float fX = 0.0f, const float fY = 0.0f, const float fWidth = 0.1f, const float fHeight = 0.1f );

    static bool s_bShowPointer;
    static GLToy_Vector_2 s_xMousePosition;

    static GLToy_Dialog* s_pxCurrentModalDialog;
	static GLToy_Array< GLToy_Dialog* > s_xDialogs;
	static GLToy_Array< GLToy_Widget* > s_xWidgets;

};

#endif