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
    void Expand( const float fWidth, const float fHeight );

    void AddText( const GLToy_String& szString );
    void AddWidget( GLToy_Widget* const pxWidget );
    void AddOKButton( void ( * const pfnCallback )( void* const ), void* const pData = NULL );
    void AddYesNoButtons(
        void ( * const pfnYesCallback )( void* const ),
        void ( * const pfnNoCallback )( void* const ),
        void* const pYesData = NULL, void* const pNoData = NULL );

    void SizeToText( const GLToy_String& szString );

    void Destroy() { m_bDelete = true; }

protected:

    GLToy_DialogStyle m_ucStyle;
    GLToy_Vector_2 m_xPosition;
    GLToy_Vector_2 m_xSize;

    GLToy_Array< GLToy_Widget* > m_xWidgets;

    bool m_bDelete;

};

#endif
