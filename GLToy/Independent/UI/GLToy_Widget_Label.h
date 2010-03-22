#ifndef __GLTOY_WIDGET_LABEL_H_
#define __GLTOY_WIDGET_LABEL_H_

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

class GLToy_Widget_Label
: public GLToy_Widget
{

    typedef GLToy_Widget GLToy_Parent;

public:

    GLToy_Widget_Label( const GLToy_WidgetType eType, const float fX = 0.0f, const float fY = 0.0f, const float fWidth = 0.1f, const float fHeight = 0.1f )
    : GLToy_Parent( eType, fX, fY, fWidth, fHeight )
    , m_szString()
    , m_uFont( uGLTOY_BAD_HASH )
    {
    }

    virtual ~GLToy_Widget_Label() {}

    virtual void Render2D() const;

    GLToy_Inline void SetString( const GLToy_String& szString ) { m_szString = szString; }
    GLToy_Inline const GLToy_String& GetString() const { return m_szString; }

    GLToy_Inline void SetFont( const GLToy_String& szString ) { m_uFont = szString.GetHash(); }
    GLToy_Inline GLToy_Hash GetFont() const { return m_uFont; }

protected:

    GLToy_String m_szString;
    GLToy_Hash m_uFont;

};

#endif
