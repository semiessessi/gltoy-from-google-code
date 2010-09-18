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
#include <Demo/JD1_DemoScene_Chmutov_2.h>

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

void JD1_DemoScene_Chmutov_2::Initialise()
{
    GLToy_Texture_System::CreateTexture( GLToy_Hash_Constant( "generic/grid2.png" ) );
}

void JD1_DemoScene_Chmutov_2::Shutdown()
{
}

void JD1_DemoScene_Chmutov_2::Render() const
{
    GLToy_Raytrace_Fullscreen xRaytrace( GLToy_Hash_Constant( "JD1_Raytrace_Chmutov_2" ) );

    xRaytrace.BindTexture( "xTexture", GLToy_Hash_Constant( "generic/grid2.png" ) );

    xRaytrace.Render();
}

void JD1_DemoScene_Chmutov_2::Update()
{
    GLToy_Parent::Update();

    const float fHeight = 0.5f * GLToy_Maths::Sin( 0.337f * GLToy_Timer::GetTime() );
    GLToy_Vector_3 xCameraPosition = 
        GLToy_Vector_3(
            GLToy_Maths::Cos( GLToy_Timer::GetTime() * 0.701f ),
            0.85f * GLToy_Maths::Cos( GLToy_Timer::GetTime() * 0.377f ),
            GLToy_Maths::Sin( GLToy_Timer::GetTime() * 0.898f ) );
    xCameraPosition.Normalise();
    xCameraPosition *= ( 3.0f - GLToy_Maths::Abs( fHeight ) );
    GLToy_Camera::SetPosition( xCameraPosition );
    GLToy_Camera::LookAt( GLToy_Vector_3( 0.0f, 0.0f, 0.0f ) );
}

void JD1_DemoScene_Chmutov_2::Start()
{
    GLToy_Parent::Start();

    GLToy_Render::SetClearFrame( true );
    GLToy_Render::Clear();
}

void JD1_DemoScene_Chmutov_2::Stop()
{
    GLToy_Camera::Reset();
}
