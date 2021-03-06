/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010, 2011 Semi Essessi
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
#include <Particle/GLToy_PFX.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Core/GLToy_UpdateFunctor.h>
#include <Render/GLToy_RenderFunctor.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_PFX::IsDone() const
{
    return !IsEmitting() && m_xSources.IsEmpty();
}

void GLToy_PFX::Render() const
{
    GLToy_PointerRenderFunctor< GLToy_ParticleSource* > xFunctor;
    m_xSources.Traverse( xFunctor );
}

void GLToy_PFX::Update()
{
    GLToy_PointerUpdateFunctor< GLToy_ParticleSource* > xFunctor;
    m_xSources.Traverse( xFunctor );

    const float fFrameTime = GLToy_Timer::GetFrameTime();
    m_fLifetime -= fFrameTime;

    m_xPosition += m_xVelocity * fFrameTime;

    // remove any sources we can
    for( u_int u = 0; u < m_xSources.GetCount(); ++u )
    {
        if( m_xSources[ u ]->IsDone() )
        {
            m_xSources.RemoveAt( u );
            --u;
        }
    }
}
