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
, m_xParticles()
, m_pxParent( pxParent )
, m_fReleaseRate( 1.0f / xProperties.m_fReleaseRate )
, m_fReleaseTimer( 0.0f )
, m_fLifetime( xProperties.m_fLifetime )
, m_fSpeed( xProperties.m_fSpeed )
, m_bRandomDirection( xProperties.m_bRandomDirection )
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
    GLToy_PointerRenderFunctor< GLToy_Particle* > xFunctor;
    m_xParticles.Traverse( xFunctor );
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
                m_xParticleProperties.m_xVelocity = m_bRandomDirection ? ( GLToy_Maths::RandomDirection() * m_fSpeed ) : m_xParticleProperties.m_xVelocity;
                m_xParticles.Append( new GLToy_Particle( m_xParticleProperties, m_pxParent->GetPosition() + m_xParticleProperties.m_xVelocity * m_fReleaseRate * static_cast< float >( u ) ) );
            }

            m_fReleaseTimer -= static_cast< float >( uCount ) * m_fReleaseRate;
        }
    }

    // update particles
    GLToy_PointerUpdateFunctor< GLToy_Particle* > xFunctor;
    m_xParticles.Traverse( xFunctor );

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
