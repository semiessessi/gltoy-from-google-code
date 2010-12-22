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

#ifndef __GLTOY_PARTICLESOURCE_H_
#define __GLTOY_PARTICLESOURCE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parent
#include <Render/GLToy_Renderable.h>

// GLToy
#include <Core/Data Structures/GLToy_List.h>
#include <Particle/GLToy_Particle.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_PFX;

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

    GLToy_ParticleSourceProperties()
    : m_xPosition( GLToy_Maths::ZeroVector3 )
    , m_fReleaseRate( 2.0f )
    , m_fLifetime( 3.0f )
    , m_uParticleHash( uGLTOY_BAD_HASH )
    , m_fSpeed( 32.0f )
    , m_bRandomDirection( false )
    {
    }

    GLToy_Vector_3 m_xPosition;
    float m_fReleaseRate;
    float m_fLifetime;
    GLToy_Hash m_uParticleHash;
    float m_fSpeed;

    bool m_bRandomDirection;

};

class GLToy_ParticleSource
: public GLToy_Renderable
{

public:

    GLToy_ParticleSource( const GLToy_ParticleSourceProperties& xProperties, const GLToy_PFX* const pxParent = NULL );

    virtual void Render() const;
    void Update();

    GLToy_ForceInline bool IsEmitting() const { return m_fLifetime > 0.0f; }
    bool IsDone() const;

    // TODO : getters and setters, or okay to leave public for now?
    // maybe protected with friends specified?

    GLToy_ParticleProperties m_xParticleProperties;
    GLToy_List< GLToy_Particle* > m_xParticles;
    const GLToy_PFX* m_pxParent;

    float m_fReleaseRate;
    float m_fReleaseTimer;
    float m_fLifetime;
    float m_fSpeed;    

    bool m_bRandomDirection;

};

#endif
