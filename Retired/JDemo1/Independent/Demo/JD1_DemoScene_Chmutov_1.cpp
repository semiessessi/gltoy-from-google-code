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

#include <Core/JD1.h>

// This file's headers
#include <Demo/JD1_DemoScene_Chmutov_1.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Maths/GLToy_Noise.h>
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_Raytrace_Fullscreen.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////


JD1_DemoScene_Chmutov_1::JD1_DemoScene_Chmutov_1()
: m_xRippleTimers( GLToy_Maths::ZeroVector4 )
{
}

void JD1_DemoScene_Chmutov_1::Initialise()
{
    GLToy_Texture_System::CreateTexture( GLToy_Hash_Constant( "generic/grid2.png" ) );
}

void JD1_DemoScene_Chmutov_1::Shutdown()
{
}

void JD1_DemoScene_Chmutov_1::Render() const
{
    GLToy_Raytrace_Fullscreen xRaytrace( GLToy_Hash_Constant( "JD1_Raytrace_Chmutov_1" ) );

    xRaytrace.BindTexture( "xTexture", GLToy_Hash_Constant( "generic/grid2.png" ) );
    xRaytrace.BindUniform( "xRippleTimers", &m_xRippleTimers );

    xRaytrace.Render();
}

void JD1_DemoScene_Chmutov_1::Update()
{
    GLToy_Parent::Update();

    const float fHeight = GLToy_Maths::ClampedLerp( 3.0f, 0.0f, 0.002f * m_fTimer * m_fTimer );
    GLToy_Vector_3 xCameraPosition = GLToy_Vector_3( -0.05f, 0.2f, -1.0f );
    xCameraPosition.Normalise();    
    xCameraPosition *= fHeight;

    static float fX = 0.0f, fY = 0.0f, fZ = 0.0f;
    if( fHeight == 0.0f )
    {
        // rotate camera
        fX += 0.35f * GLToy_Timer::GetFrameTime();
        fY += 0.75f * GLToy_Timer::GetFrameTime();
    }

	m_xRippleTimers[ 0 ] = GLToy_Maths::Wrap( GLToy_Timer::GetTime() );

    GLToy_Camera::SetEuler( fX, fY, fZ );
    GLToy_Camera::SetPosition( xCameraPosition );
}

void JD1_DemoScene_Chmutov_1::Start()
{
    GLToy_Parent::Start();

    GLToy_Render::SetClearFrame( true );
    GLToy_Render::Clear();
}

void JD1_DemoScene_Chmutov_1::Stop()
{
    GLToy_Camera::Reset();
}
