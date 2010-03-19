#ifndef __GLTOY_PARTICLE_H_
#define __GLTOY_PARTICLE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/GLToy_Updateable.h>
#include <Render/GLToy_Sprite.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

struct GLToy_ParticleProperties
{
    GLToy_Vector_3 m_xPosition;
    GLToy_Vector_3 m_xVelocity;
    float m_fSize;
    float m_fLifetime;
    GLToy_Hash m_uTextureHash;
};

class GLToy_Particle
: public GLToy_Sprite
, public GLToy_Updateable
{

    typedef GLToy_Sprite GLToy_Parent;

public:

    GLToy_Particle( const GLToy_ParticleProperties& xProperties )
    : GLToy_Parent()
    , m_xVelocity( xProperties.m_xVelocity )
    , m_fLifetime( xProperties.m_fLifetime )
    {
        SetPosition( xProperties.m_xPosition );
        SetSize( xProperties.m_fSize );
        SetTexture( xProperties.m_uTextureHash );
    }

    // TODO - really this belongs to a future physics system
    // the particle class should probably derive from a non-interacting physics object
    GLToy_Vector_3 m_xVelocity;
    float m_fLifetime;

};

#endif