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
    static void ShowErrorDialog( const GLToy_String& szError, ... );
    static void ShowQuitDialog();

    static const GLToy_Vector_2& GetMousePosition() { return s_xMousePosition; }
    static float GetPulse();

private:

    static GLToy_Widget* CreateWidget( const GLToy_WidgetType eType, const float fX = 0.0f, const float fY = 0.0f, const float fWidth = 0.1f, const float fHeight = 0.1f );
    
    // Dialog callbacks
    static void DefaultModalCallback( void* const pData );
    static void QuitCallback( void* const pData );

    static bool s_bShowPointer;
    static GLToy_Vector_2 s_xMousePosition;

    static GLToy_Dialog* s_pxCurrentModalDialog;
    static GLToy_Array< GLToy_Dialog* > s_xDialogs;
    static GLToy_Array< GLToy_Widget* > s_xWidgets;
    
    static bool s_bModalOldPointerShow;

};

#endif
