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
    , m_uFont( GLToy_Hash_Constant( "FrontEnd" ) )
    , m_pfnCallback( NULL )
    , m_uFlags( 0 )
    {
    }

    virtual ~GLToy_Widget_ImageButton() {}

    virtual void Render2D() const;
    virtual void Update();

    GLToy_Inline void SetTexture( const GLToy_String& szString ) { m_uTextureHash = szString.GetHash(); }
    GLToy_Inline void SetTexture( const GLToy_Hash& uHash ) { m_uTextureHash = uHash; }

    GLToy_Inline void SetLabelString( const GLToy_String& szString ) { m_szString = szString; }
    GLToy_Inline const GLToy_String& GetLabelString() const { return m_szString; }

    GLToy_Inline void SetFont( const GLToy_String& szString ) { m_uFont = szString.GetHash(); }
    GLToy_Inline GLToy_Hash GetFont() const { return m_uFont; }

    GLToy_Inline void SetCallback( void ( * const pfnCallback )( void* const pData ) ) { m_pfnCallback = pfnCallback; }

protected:

    GLToy_Hash m_uTextureHash;
    GLToy_String m_szString;
    GLToy_Hash m_uFont;
    void ( * m_pfnCallback )( void* const pData );

    union
    {
        struct
        {
            u_int m_bClickFlag : 1;
            u_int m_bHighlightFlag : 1;
        };

        u_int m_uFlags;
    };
};

#endif
