/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010, 2011 Semi Essessi
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
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_RenderFunctor.h>
#include <Render/GLToy_Render_Metrics.h>
#include <Render/GLToy_Texture_System.h>

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
, m_fDelay( xProperties.m_fDelay )
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

    // TODO - fix this and allow sorting by source to reduce state change calls...
    //if( m_xParticles.GetCount() == 0 )
    //{
    //    return;
    //}

    //static const GLToy_Particle& xFirstParticle = *( m_xParticles[ 0 ] );
    //GLToy_Render_Metrics::IncrementTriangleCount( 2 * m_xParticles.GetCount() );

    //if( xFirstParticle.m_ucOrdering == ucSPRITE_ORDERED_ALWAYS )
    //{
    //    GLToy_Render::RegisterTransparent( this, ( xFirstParticle.m_xPosition - GLToy_Camera::GetPosition() ).MagnitudeSquared() );
    //    return;
    //}
    //else if( xFirstParticle.m_ucOrdering == ucSPRITE_ORDERED_NEVER )
    //{
    //    GLToy_Render::EnableBlending();
    //    GLToy_Render::EnableDepthTesting();
    //    GLToy_Render::DisableDepthWrites();

    //    RenderTransparent();

    //    GLToy_Render::EnableDepthWrites();
    //    return;
    //}

    //switch( xFirstParticle.m_ucBlendFunc )
    //{
    //    default:
    //    case ucSPRITE_BLEND_NORMAL:
    //    case ucSPRITE_BLEND_ADDITIVE_SORT:
    //    {
    //        GLToy_Render::RegisterTransparent( this, ( xFirstParticle.m_xPosition - GLToy_Camera::GetPosition() ).MagnitudeSquared() );
    //        break;
    //    }
    //    
    //    case ucSPRITE_BLEND_ADDITIVE:
    //    {
    //        GLToy_Render::SetBlendFunction( BLEND_ONE, BLEND_ONE );
    //        GLToy_Render::EnableBlending();
    //        GLToy_Render::EnableDepthTesting();
    //        GLToy_Render::DisableDepthWrites();
    //        RenderTransparent();
    //        break;
    //    }
    //}
}

void GLToy_ParticleSource::RenderTransparent() const
{
    // m_xParticles.GetCount() != 0 guaranteed by logic that sets up the call
    GLToy_Assert( m_xParticles.GetCount() != 0, "About to dereference bad data for particle source render!" );
    static const GLToy_Particle& xFirstParticle = *( m_xParticles[ 0 ] );

    switch( xFirstParticle.m_ucBlendFunc )
    {
        case ucSPRITE_BLEND_ADDITIVE:
        case ucSPRITE_BLEND_ADDITIVE_SORT:
        {
            GLToy_Render::SetBlendFunction( BLEND_SRC_ALPHA, BLEND_ONE );
            break;
        }

        default:
        case ucSPRITE_BLEND_NORMAL:
        {
            GLToy_Render::SetBlendFunction( BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA );
            break;
        }
    }

	GLToy_Render::EnableDepthTesting();
    GLToy_Render::DisableDepthWrites();

    GLToy_Texture_System::BindTexture( xFirstParticle.m_uTextureHash );

    GLToy_Render::StartSubmittingQuads();

    for( u_int u = 0; u < m_xParticles.GetCount(); ++u )
    {
        m_xParticles[ u ]->Submit();
    }

    GLToy_Render::EndSubmit();
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
    
    m_fDelay -= fFrameTime;
    if( m_fDelay > 0.0f )
    {
        return;
    }

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
                if( m_xParticleProperties.m_bInheritVelocity )
                {
                    // TODO: allow velocity to be independent of parent PFX
                    m_xParticleProperties.m_xVelocity += m_pxParent->GetVelocity();
                }
                // TODO: allow position to be independent of parent PFX
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
