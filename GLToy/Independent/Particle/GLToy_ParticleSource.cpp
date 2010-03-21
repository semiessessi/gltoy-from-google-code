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
    m_xParticles.Traverse( GLToy_RenderFunctor< GLToy_Particle >() );
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
        if( m_xParticles[ u ].IsDone() )
        {
            m_xParticles.RemoveAt( u );
            --u;
        }
    }
}