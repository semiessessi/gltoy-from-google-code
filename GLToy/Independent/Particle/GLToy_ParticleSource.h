#ifndef __GLTOY_PARTICLESOURCE_H_
#define __GLTOY_PARTICLESOURCE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/GLToy_Updateable.h>
#include <Render/GLToy_Renderable.h>

// GLToy
#include <Particle/GLToy_Particle.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const u_int uMAX_PARTICLES_PER_SOURCE = 64;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

// just some initial simple properties to get things working
struct GLToy_ParticleSourceProperties
{

    GLToy_Vector_3 m_xPosition;
    float m_fReleaseRate;

};

class GLToy_ParticleSource
: public GLToy_Renderable
, public GLToy_Updateable
{

public:

    GLToy_ParticleSource( const GLToy_ParticleSourceProperties& xProperties )
    : m_fReleaseRate( 1.0f )
    , m_xParticleProperties()
    {
    }

    virtual void Render() const;
    virtual void Update();

    GLToy_ParticleProperties m_xParticleProperties;
    float m_fReleaseRate;

};

class GLToy_ParticleSource_System
{

public:

    static bool Initialise();
    static void Shutdown();

};

#endif