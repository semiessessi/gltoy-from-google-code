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
#include <Render/GLToy_RenderFunctor.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_ParticleSource::Render() const
{
    m_xParticles.Traverse( GLToy_PointerRenderFunctor< GLToy_Particle* >() );
}

void GLToy_ParticleSource::Update()
{
    // release a particle if we can
    m_fReleaseTimer += GLToy_Timer::GetFrameTime();
    if( ( m_fReleaseTimer > m_fReleaseRate ) && ( m_xParticles.GetCount() < uMAX_PARTICLES_PER_SOURCE ) )
    {
        const u_int uCount = static_cast< u_int >( GLToy_Maths::Floor( m_fReleaseTimer / m_fReleaseRate ) );
        for( u_int u = 0; u < uCount; ++u )
        {
            m_xParticles.Append( new GLToy_Particle( m_xParticleProperties ) );
        }

        m_fReleaseTimer -= static_cast< float >( uCount ) * m_fReleaseRate;
    }

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