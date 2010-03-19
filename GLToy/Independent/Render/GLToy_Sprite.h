#ifndef __GLTOY_SPRITE_H_
#define __GLTOY_SPRITE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Render/GLToy_Renderable.h>

// GLToy
#include <Core/GLToy_Hash.h>
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_String;
class GLToy_Texture;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Sprite
: public GLToy_Renderable_Transparent
{

public:
    GLToy_Sprite()
    : m_xPosition( GLToy_Maths::ZeroVector3 )
    , m_fSize( 1.0f )
    , m_pxTexture( NULL )
    {
    }

    virtual ~GLToy_Sprite() {}

    virtual void Render() const;
    virtual void RenderTransparent() const;

    const GLToy_Vector_3& GetPosition() const { return m_xPosition; }
    float GetSize() const { return m_fSize; }
    const GLToy_Texture* GetTexture() const { return m_pxTexture; }
    
    void SetPosition( const GLToy_Vector_3& xPosition ) { m_xPosition = xPosition; }
    void SetSize( const float fSize ) { m_fSize = fSize; }
    void SetTexture( const GLToy_Hash uHash );
    void SetTexture( const GLToy_String& szName );

protected:

    GLToy_Vector_3  m_xPosition;
    float m_fSize;
    GLToy_Texture*  m_pxTexture;

};

#endif
