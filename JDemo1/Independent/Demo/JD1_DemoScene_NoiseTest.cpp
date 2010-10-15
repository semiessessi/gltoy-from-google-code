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
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const GLToy_String szFragmentShader =
"varying vec3 xDirection;"
"varying vec3 xPosition;"
"uniform sampler2D xTexture;"

"void main()"
"{"
    // x^2 + y^2 + z^2 - r^2 = 0
    // t^2(dx^2 + dy^2 + dz^2) + 2t(oxdx + oydy + ozdz) + (ox^2 + oy^2 + oz^2 - r^2) = 0
	// at^2 + bt + c = 0
	// t = -b - sqrt(b^2 - 4ac) / 2a // (always take the nearest, so discard the '+' solution)

	"vec3 xNormalisedDirection = normalize( xDirection );"
	
	// SE - 15/10/2010 - this always seems too convenient to not have a geometric interpetation I am missing somehow
	"vec3 xQ = 2.0 * vec3( dot( xNormalisedDirection, xNormalisedDirection ), dot( xPosition, xNormalisedDirection ), dot( xPosition, xPosition ) - 1.0 );"
    
	"float fDiscriminant = xQ.y * xQ.y - xQ.x * xQ.z;"
	"if( fDiscriminant < 0.0 )"
    "{"
        "discard;"
    "}"

    "float fT = ( -xQ.y - sqrt( fDiscriminant ) ) / xQ.x;"
    
	"if( fT < 0.0 )"
    "{"
        "discard;"
    "}"

    "vec3 xSolution = xNormalisedDirection * fT + xPosition;"
    
    "gl_FragDepth = 1.0;" //gl_DepthRange.diff / xSolution.z;
    "float fNoise = noise3d( xSolution * 8192.0 );"
	"vec4 xColour = vec4( fNoise, fNoise, fNoise, 1.0 ) * -dot( normalize( xSolution ), xNormalisedDirection );"
	"gl_FragColor = xColour;"
"}"
;

static const GLToy_String szVertexShader =
"varying vec3 xDirection;"
"varying vec3 xPosition;"

"void main()"
"{"
	"xPosition = vec3( gl_Normal );"
	// xDirection = gl_MultiTexCoord0.xyz * mat3( gl_ModelViewMatrix );
    //"xDirection = gl_MultiTexCoord0.xyz * mat3( gl_ModelViewMatrix[ 0 ], gl_ModelViewMatrix[ 1 ], gl_ModelViewMatrix[ 2 ] );"
    "xDirection = gl_MultiTexCoord0.xyz * mat3( gl_ModelViewMatrix[ 0 ][ 0 ], gl_ModelViewMatrix[ 0 ][ 1 ], gl_ModelViewMatrix[ 0 ][ 2 ], gl_ModelViewMatrix[ 1 ][ 0 ], gl_ModelViewMatrix[ 1 ][ 1 ], gl_ModelViewMatrix[ 1 ][ 2 ], gl_ModelViewMatrix[ 2 ][ 0 ], gl_ModelViewMatrix[ 2 ][ 1 ], gl_ModelViewMatrix[ 2 ][ 2 ] );"
	"gl_Position = gl_Vertex;"
"}"
;

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

static GLToy_OrientedSpline_CatmullRomCatmullRom s_xSpline;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void JD1_DemoScene_NoiseTest::Initialise()
{
	GLToy_Shader_System::CreateShaderFromStrings( "JD1_Raytrace_NoiseTest_Hardcoded", szFragmentShader, szVertexShader );

	GLToy_ShaderProgram* const pxShader = GLToy_Shader_System::FindShader( GLToy_Hash_Constant( "JD1_Raytrace_NoiseTest_Hardcoded" ) );
	if( pxShader )
	{
		pxShader->SetUseNoise( true );
	}

	s_xSpline.Append( GLToy_Vector_3( 0.0f, 2.5f, 0.0f ) );
	s_xSpline.AppendOrientation( GLToy_Vector_3( 0.0f, -1.0f, 0.0f) );
	s_xSpline.Append( GLToy_Vector_3( 2.0f, 2.0f, 0.0f ) );
	s_xSpline.AppendOrientation( GLToy_Vector_3( -1.0f, -1.0f, 0.0f) );
	s_xSpline.Append( GLToy_Vector_3( 2.5f, 0.0f, 0.0f ) );
	s_xSpline.AppendOrientation( GLToy_Vector_3( -1.0f, 0.0f, 0.0f) );
	s_xSpline.Append( GLToy_Vector_3( 0.0f, -2.5f, 0.0f ) );
	s_xSpline.AppendOrientation( GLToy_Vector_3( 0.0f, 1.0f, 0.0f) );
	s_xSpline.Append( GLToy_Vector_3( 2.0f, -2.0f, 0.0f ) );
	s_xSpline.AppendOrientation( GLToy_Vector_3( -1.0f, 1.0f, 0.0f) );
	s_xSpline.Append( GLToy_Vector_3( -2.0f, -2.0f, 0.0f ) );
	s_xSpline.AppendOrientation( GLToy_Vector_3( 1.0f, 1.0f, 0.0f) );
	s_xSpline.Append( GLToy_Vector_3( -2.5f, 0.0f, 0.0f ) );
	s_xSpline.AppendOrientation( GLToy_Vector_3( 1.0f, 0.0f, 0.0f) );
	s_xSpline.Append( GLToy_Vector_3( 0.0f, 2.5f, 0.0f ) );
	s_xSpline.AppendOrientation( GLToy_Vector_3( 0.0f, -1.0f, 0.0f) );

	GLToy_Assert( s_xSpline.IsComplete(), "Incomplete oriented spline for camera!" );
}

void JD1_DemoScene_NoiseTest::Shutdown()
{
}

void JD1_DemoScene_NoiseTest::Render() const
{
    GLToy_Raytrace_Fullscreen xRaytrace( GLToy_Hash_Constant( "JD1_Raytrace_NoiseTest_Hardcoded" ) );

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
