/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of GLToy.
//
// GLToy is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// GLToy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with GLToy.
// If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GLTOY_PARTICLESOURCE_H_
#define __GLTOY_PARTICLESOURCE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/GLToy_Updateable.h>
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
    {
    }

    GLToy_Vector_3 m_xPosition;
    float m_fReleaseRate;
    float m_fLifetime;
    GLToy_Hash m_uParticleHash;

};

class GLToy_ParticleSource
: public GLToy_Renderable
, public GLToy_Updateable
{

public:

    GLToy_ParticleSource( const GLToy_ParticleSourceProperties& xProperties, const GLToy_PFX* const pxParent = NULL );

    virtual void Render() const;
    virtual void Update();

    GLToy_Inline bool IsEmitting() const { return m_fLifetime > 0.0f; }
    bool IsDone() const;

	// TODO : getters and setters, or okay to leave public for now?
	// maybe protected with friends specified?

    GLToy_ParticleProperties m_xParticleProperties;
    float m_fReleaseRate;
    float m_fReleaseTimer;
    float m_fLifetime;

    GLToy_List< GLToy_Particle* > m_xParticles;

	const GLToy_PFX* m_pxParent;

};

#endif