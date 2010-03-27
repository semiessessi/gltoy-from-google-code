#ifndef __GLTOY_UI_SYSTEM_H_
#define __GLTOY_UI_SYSTEM_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <UI/GLToy_WidgetTypes.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class T > class GLToy_Array;
class GLToy_Vector_2;
class GLToy_Widget;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_UI_System
{

public:

    static bool Initialise();
    static void Shutdown();

	static void Render2D();
	static void Update();

    static void ClearWidgets();
	static GLToy_Widget* CreateWidget( const GLToy_WidgetType eType, const float fX = 0.0f, const float fY = 0.0f, const float fWidth = 0.1f, const float fHeight = 0.1f );

    static void ShowPointer( const bool bShow = true );
    static bool IsPointerShown() { return s_bShowPointer; }

private:

    static bool s_bShowPointer;
    static GLToy_Vector_2 s_xMousePosition;

	static GLToy_Array< GLToy_Widget* > s_xTopWidgets;

};

#endif