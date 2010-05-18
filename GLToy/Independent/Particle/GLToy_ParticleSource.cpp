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

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Particle/GLToy_ParticleSource.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Core/GLToy_UpdateFunctor.h>
#include <Maths/GLToy_Maths.h>
#include <Particle/GLToy_PFX.h>
#include <Particle/GLToy_PFX_System.h>
#include <Render/GLToy_RenderFunctor.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_ParticleSource::GLToy_ParticleSource( const GLToy_ParticleSourceProperties& xProperties, const GLToy_PFX* const pxParent )
: m_xParticleProperties()
, m_fReleaseRate( 1.0f / xProperties.m_fReleaseRate )
, m_fReleaseTimer( 0.0f )
, m_fLifetime( xProperties.m_fLifetime )
, m_xParticles()
, m_pxParent( pxParent )
{
    const GLToy_ParticleProperties* const pxParticleProperties = GLToy_PFX_System::GetParticleProperties( xProperties.m_uParticleHash );
    if( pxParticleProperties )
    {
        m_xParticleProperties = *pxParticleProperties;
    }
}

bool GLToy_ParticleSource::IsDone() const
{
    return !IsEmitting() && ( m_xParticles.IsEmpty() );
}

void GLToy_ParticleSource::Render() const
{
    m_xParticles.Traverse( GLToy_PointerRenderFunctor< GLToy_Particle* >() );
}

void GLToy_ParticleSource::Update()
{
    GLToy_Assert( m_pxParent != NULL, "Particle source without parent!" );

#ifdef GLTOY_DEBUG
    if( !m_pxParent )
    {
        return;
    }
#endif

    const float fFrameTime = GLToy_Timer::GetFrameTime();
    m_fLifetime -= fFrameTime;

    if( IsEmitting() )
    {
        // release a particle if we can
        m_fReleaseTimer += fFrameTime;
        if( ( m_fReleaseTimer > m_fReleaseRate ) && ( m_xParticles.GetCount() < uMAX_PARTICLES_PER_SOURCE ) )
        {
            const u_int uCount = static_cast< u_int >( GLToy_Maths::Floor( m_fReleaseTimer / m_fReleaseRate ) );
            for( u_int u = 0; u < uCount; ++u )
            {
                m_xParticles.Append( new GLToy_Particle( m_xParticleProperties, m_pxParent->GetPosition() + m_xParticleProperties.m_xVelocity * m_fReleaseRate * static_cast< float >( u ) ) );
            }

            m_fReleaseTimer -= static_cast< float >( uCount ) * m_fReleaseRate;
        }
    }

    // update particles
    m_xParticles.Traverse( GLToy_PointerUpdateFunctor< GLToy_Particle* >() );

    // remove any particles we can
    for( u_int u = 0; u < m_xParticles.GetCount(); ++u )
    {
        if( m_xParticles[ u ]->IsDone() )
        {
            m_xParticles.RemoveAt( u );
            --u;
        }
    }
}
