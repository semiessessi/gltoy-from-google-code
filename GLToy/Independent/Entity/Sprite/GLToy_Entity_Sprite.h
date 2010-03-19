#ifndef __GLTOY_ENTITY_SPRITE_H_
#define __GLTOY_ENTITY_SPRITE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Entity/GLToy_Entity.h>

// GLToy
#include <Render/GLToy_Sprite.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Entity_Sprite
: public GLToy_Entity_Sphere
{

    typedef GLToy_Entity_Sphere GLToy_Parent;

public:

    GLToy_Entity_Sprite( const GLToy_Hash uHash, const GLToy_EntityType eType )
    : GLToy_Parent( uHash, eType )
    , m_xSprite()
    {
    }

    virtual void ReadFromBitStream( const GLToy_BitStream& xStream );
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const;

    virtual void Render() const;

protected:

    GLToy_Sprite m_xSprite;

};

#endif