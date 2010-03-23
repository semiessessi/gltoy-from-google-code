#ifndef __GLTOY_WIDGET_H_
#define __GLTOY_WIDGET_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/GLToy_Updateable.h>
#include <Render/GLToy_Renderable.h>

// GLToy
#include <Core/Data Structures/GLToy_HashTree.h>
#include <Core/GLToy_Hash.h>
#include <UI/GLToy_WidgetTypes.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Widget
: public GLToy_Renderable
, public GLToy_Updateable
{

public:

    GLToy_Widget()
    : m_fX( 0.0f )
	, m_fY( 0.0f )
	, m_fWidth( 0.0f )
	, m_fHeight( 0.0f )
	, m_eType( WIDGET_NULL )
    {

    }

	GLToy_Widget( const GLToy_WidgetType eType, const float fX = 0.0f, const float fY = 0.0f, const float fWidth = 0.1f, const float fHeight = 0.1f )
    : m_fX( fX )
	, m_fY( fY )
	, m_fWidth( fWidth )
	, m_fHeight( fHeight )
	, m_eType( eType )
    {
    }

    // TODO - I think this can be safely deleted
    GLToy_Widget( const GLToy_Widget& xWidget )
    : m_fX( 0.0f )
	, m_fY( 0.0f )
	, m_fWidth( 0.0f )
	, m_fHeight( 0.0f )
	, m_eType( WIDGET_NULL )
    {
    }

    virtual ~GLToy_Widget() {}

	// widgets only need the 2d render function...
	// if they go on 3d stuff it should be by rendering to a texture
	virtual void Render() const {}
	
	virtual void Update() {}

	virtual void Execute() {}

	GLToy_Inline float GetX() const { return m_fX; }
	GLToy_Inline float GetY() const { return m_fY; }
	GLToy_Inline float GetWidth() const { return m_fWidth; }
	GLToy_Inline float GetHeight() const { return m_fHeight; }
    
    GLToy_Inline void SetPosition( const float fX, const float fY ) { m_fX = fX; m_fY = fY; }
    GLToy_Inline void SetSize( const float fWidth, const float fHeight ) { m_fWidth = fWidth; m_fHeight = fHeight; }

protected:

    float m_fX;
	float m_fY;
	float m_fWidth;
	float m_fHeight;
	GLToy_WidgetType m_eType;

	GLToy_HashTree< GLToy_Widget* > m_xChildren;

};

#endif
