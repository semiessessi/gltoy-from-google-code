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

#include <Core/JD1.h>

// This file's header
#include <Render/SuperScope/JD1_SuperScope.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Maths/GLToy_Maths.h>
#include <Maths/GLToy_Noise.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void JD1_SuperScope::Render() const
{
    GLToy_Vector_2 xCurrentPoint;
    GLToy_Vector_3 xCurrentColour = GLToy_Vector_3( 1.0f, 1.0f, 1.0f );
    float fParameter;
    float fFakeOsc;
    float fSize;
    bool bSkip = false;

    GLToy_Render::SetOrthogonalProjectionMatrix();
    GLToy_Render::PushViewMatrix();
    GLToy_Render::SetIdentityViewMatrix();

    GLToy_Texture_System::BindWhite();

    GLToy_Render::StartSubmittingLineStrip();
    
    for( u_int u = 0; u < m_uPointCount; ++u )
    {
        fParameter = static_cast< float >( u ) / static_cast< float >( m_uPointCount - 1 );
        fFakeOsc = GLToy_Noise::FractalCosine1D( fParameter * 20.0f + 5.0f * GLToy_Timer::GetTime(), 30.0f );

        PerPoint( fParameter, fFakeOsc, xCurrentPoint[ 0 ], xCurrentPoint[ 1 ], xCurrentColour[ 0 ], xCurrentColour[ 1 ], xCurrentColour[ 2 ], bSkip, fSize );

        if( !bSkip )
        {
            xCurrentPoint[ 1 ] = -xCurrentPoint[ 1 ];
            GLToy_Render::SubmitColour( xCurrentColour );
            GLToy_Render::SubmitVertex( xCurrentPoint );
        }
    }

    GLToy_Render::EndSubmit();

    GLToy_Render::SetPerspectiveProjectionMatrix();
    GLToy_Render::PopViewMatrix();
}

void JD1_SuperScope::Update()
{
    PerFrame();

    static float fTimer = 0.0f;
    fTimer += GLToy_Timer::GetFrameTime();

    if( fTimer > 0.6f )
    {
        OnBeat();
        fTimer = 0.0f;
    }

    m_fWidth = static_cast< float >( GLToy::GetWindowViewportWidth() );
    m_fHeight = static_cast< float >( GLToy::GetWindowViewportHeight() );
}
