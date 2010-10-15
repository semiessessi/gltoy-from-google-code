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
#include <Demo/JD1_DemoScene_NoiseTest.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Maths/GLToy_Maths.h>
#include <Maths/GLToy_OrientedSpline.h>
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_Raytrace_Fullscreen.h>
#include <Render/GLToy_Render.h>
#include <Render/Shader/GLToy_Shader.h>
#include <Render/Shader/GLToy_Shader_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

static GLToy_OrientedSpline_CatmullRomCatmullRom s_xSpline;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void JD1_DemoScene_NoiseTest::Initialise()
{
	GLToy_ShaderProgram* const pxShader = GLToy_Shader_System::FindShader( GLToy_Hash_Constant( "JD1_Raytrace_NoiseTest" ) );
	if( pxShader )
	{
		pxShader->SetUseNoise( true );
	}

	s_xSpline.Append( GLToy_Vector_3( 0.0f, 2.5f, 0.0f ) );
	s_xSpline.AppendOrientation( GLToy_Vector_3( 0.0f, -1.0f, 0.0f) );
	s_xSpline.Append( GLToy_Vector_3( 2.5f, 0.0f, 0.0f ) );
	s_xSpline.AppendOrientation( GLToy_Vector_3( -1.0f, 0.0f, 0.0f) );
	s_xSpline.Append( GLToy_Vector_3( 0.0f, -2.5f, 0.0f ) );
	s_xSpline.AppendOrientation( GLToy_Vector_3( 0.0f, 1.0f, 0.0f) );
	s_xSpline.Append( GLToy_Vector_3( -2.5f, 0.0f, 0.0f ) );
	s_xSpline.AppendOrientation( GLToy_Vector_3( 1.0f, 0.0f, 0.0f) );

	GLToy_Assert( s_xSpline.IsComplete(), "Incomplete oriented spline for camera!" );
}

void JD1_DemoScene_NoiseTest::Shutdown()
{
}

void JD1_DemoScene_NoiseTest::Render() const
{
    GLToy_Raytrace_Fullscreen xRaytrace( GLToy_Hash_Constant( "JD1_Raytrace_NoiseTest" ) );

    xRaytrace.Render();
}

void JD1_DemoScene_NoiseTest::Update()
{
    GLToy_Parent::Update();

	const float fTimer = GLToy_Maths::Wrap( GLToy_Timer::GetTime() * 0.03f );
    GLToy_Camera::SetPosition( s_xSpline.GetPoint( fTimer ) );
	GLToy_Camera::SetOrientation( s_xSpline.GetOrientation( fTimer ) );
}

void JD1_DemoScene_NoiseTest::Start()
{
    GLToy_Parent::Start();

    GLToy_Render::SetClearFrame( true );
    GLToy_Render::Clear();
}

void JD1_DemoScene_NoiseTest::Stop()
{
    GLToy_Camera::Reset();
}
