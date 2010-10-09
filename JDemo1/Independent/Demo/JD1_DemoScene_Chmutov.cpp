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
#include <Demo/JD1_DemoScene_Chmutov.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Maths/GLToy_Maths.h>
#include <Maths/GLToy_Noise.h>
#include <Maths/GLToy_OrientedSpline.h>
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_Raytrace_Fullscreen.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture.h>
#include <Render/Shader/GLToy_Shader_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const GLToy_String szFragmentShader =
"#version 120\r\n"

"varying vec3 xDirection;"
"varying vec3 xPosition;"
"uniform sampler2D xTexture;"

"vec3 xSolution;"
"vec3 xTraceSolution;"
"vec3 xNormalisedDirection;"
"vec3 xSpecularDirection;"
"vec3 xLightPosition;"
"float fGlossTexture;"
"float fHackEdgeSoften;"
"float fInitialK = -1.0f;"
"float fK;"

"float solve( vec3 xPos, vec3 xDir )"
"{"
	"const float fSize = 0.375f;"
	"xNormalisedDirection = normalize( xDir );"
	
	// Quartic coefficients for Chmutov surface
	"vec3 xSquares = xNormalisedDirection * xNormalisedDirection;"
	"vec3 xCubes = xSquares * xNormalisedDirection;"
	"vec3 xPosSquares = xPos * xPos;"
	"vec3 xCHelp = 6.0f * xPosSquares - 1.0f;"
	"vec3 xDHelp = xPos * ( 4.0f * xPosSquares - 2.0f );"

	// f = x^4 + y^4 + z^4 - x^2 - y^2 - z^2 + fSize
	"float fA = dot( xSquares, xSquares );" // i.e. x^4 + y^4 + z^4
	"float fB = 4.0f * dot( xCubes, xPosition );" // i.e. 4 * ( ox x^3 + oy y^3 + oz z^3 )
	"float fC = dot( xCHelp, xSquares );" // i.e. x^2 * ( 6 ox^2 - 1 ) + y^2 * ( 6 oy^2 - 1 ) + z^2 * ( 6 oz^2 - 1 );
	"float fD = dot( xNormalisedDirection, xDHelp );" // i.e. x * ox * ( 4 ox^2 - 2 ) + y * oy * ( 4 oy^2 - 2 ) + z * oz * ( 4 oz^2 - 2 );
	"float fE = dot( xPosSquares, xPosSquares - 1.0f ) + fSize;"
	
	"fK = max( 0.0f, fInitialK + length( xPos ) );"
	"float fL = -0.15f;"

	// Whittaker iteration
	"fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;"
	"fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;"
	"fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;"
	"fL = -0.5f;"
	"fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;"
	"fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;"
	"fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;"
	"fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;"
	"fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;"
	"fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;"
	"fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;"
	"fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;"
	"fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;"
	"fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;"
	"fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;"
	"fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;"
	"fL = -2.0f;"
	"fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;"
	"fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;"
	"fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;"
	"fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;"
	"fL = -0.5f;"
	"fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;"
	"fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;"
	"fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;"
	"fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;"
	"fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;"
	"fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;"
	
	"if( fK > 100.0f )"
	"{"
		"fK = -1.0f;"
	"}"

	"fHackEdgeSoften = min( ( ( 4.0f * fA * fK + 3.0f * fB ) * fK + 2.0f * fC ) * fK + fD, 0.0f );"
	"fHackEdgeSoften = clamp( -fHackEdgeSoften, 0.0f, 1.0f );"

	"return fK;"
"}"

"vec4 trace( vec3 xPos, vec3 xDir, bool bDiscard )"
"{"
	"float fK = solve( xPos, xDir );"

	"float fEdgeFade = fHackEdgeSoften;"

	"if( fK < 0.0f )"
	"{"
		"if( bDiscard )"
		"{"
			"discard;"
		"}"

		"return vec4( 0.0f, 0.0f, 0.0f, 0.0f );"
	"}"

	"xSolution = xNormalisedDirection * fK + xPos;"

	// f = x^4 + y^4 + z^4 - x^2 - y^2 - z^2 + fSize
	// df/dx = 4x^3 - 2x
	// df/dy = 4y^3 - 2y
	// df/sz = 4z^3 - 2z
	"vec3 xNormal = normalize( 4.0f * xSolution * xSolution * xSolution - 2.0f * xSolution );"
	// ok, lets mess with the normal, why not
	"xNormal += 0.1f * normalize( sin( 170.0f * xSolution ) );"
	"xNormal = normalize( xNormal );"

	"float fLight = 0.0f;"
	"float fSpecularity = 0.0f;"
	"xTraceSolution = xSolution;"
	"vec3 xCurrentDirection = xNormalisedDirection;"
	"float fLightDistance = length( xTraceSolution - xLightPosition );"
	"vec3 xLightDirection = xTraceSolution - xLightPosition;"
	"vec3 xNormalisedLightDirection = normalize( xLightDirection );"

	"vec4 xDiffuseTexture = vec4( ( ( cos( 12.0f * xSolution.x ) * cos( 12.0f * xSolution.y ) * cos( 12.0f * xSolution.z ) ) > 0.0f ) ? vec3( 0.7f, 0.6f, 1.0f ) : vec3( 0.45f, 0.3f, 0.85f ), 1.0f );"
	/*
	"texture2D( xTexture,"
		"vec2("
			"( 10.0f / 3.141592654f ) * atan( -xSolution.z, xSolution.x ),"
			"2.5f * xSolution.y"
			") );"*/
	"float fSpecularTexture = ( xDiffuseTexture.x > 0.5f ) ? 32.0f : 8.0f;"
	"fGlossTexture = ( fSpecularTexture > 16.0f ) ? 1.0f : 0.75f;"
	
	"float fAttenuation = 3.0f / ( 1.0f + 0.5f * dot( xLightDirection, xLightDirection ) );"
	"fLight = clamp( fAttenuation * dot( xNormal, -xNormalisedLightDirection ), 0.0f, 1.0f );"
	
	"xSpecularDirection = 2 * dot( xNormal, -xCurrentDirection ) * xNormal + xCurrentDirection;"
	"fSpecularity = fGlossTexture *  clamp( 0.25f * pow( dot( xSpecularDirection, -xNormalisedLightDirection ), fSpecularTexture ), 0.00001f, 1.0f );"

	"return fEdgeFade * ("
			"fLight * 0.8f * xDiffuseTexture"
			"+ vec4( fSpecularity, fSpecularity, fSpecularity, 1.0f )"
		");"
"}"

"void main()"
"{"
	"xLightPosition = xPosition - normalize( xDirection );"
	"vec4 xColour = trace( xPosition, xDirection, true );"
	"float fFade = fHackEdgeSoften;"
	"float fBounceScale = 1.0f;"
	"float fGloss = fBounceScale * fGlossTexture;"
	"xColour += fFade * fGloss * trace( xTraceSolution + xSpecularDirection * 0.05f, xSpecularDirection, false );"
	"gl_FragColor = xColour;"
"}"
	;

static const GLToy_String szVertexShader =
"#version 120\r\n"

"varying vec3 xDirection;"
"varying vec3 xPosition;"

"void main()"
"{"
	"xPosition = vec3( gl_Normal );"
	"xDirection = gl_MultiTexCoord0.xyz * mat3( gl_ModelViewMatrix );"
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

void JD1_DemoScene_Chmutov::Initialise()
{
    GLToy_Texture_System::CreateTexture( GLToy_Hash_Constant( "generic/grid2.png" ) );
	GLToy_Shader_System::CreateShaderFromStrings( "JD1_Raytrace_Chmutov_Hardcoded", szFragmentShader, szVertexShader );

    s_xSpline.Append( GLToy_Vector_3( 0.0f, 3.0f, 0.0f ) );
	s_xSpline.AppendOrientation( GLToy_Vector_3( 0.0f, -1.0f, 0.0f) );
    s_xSpline.Append( GLToy_Vector_3( 0.0f, 0.2f, 0.0f ) );
	s_xSpline.AppendOrientation( GLToy_Vector_3( 0.0f, -1.0f, 0.0f) );
    s_xSpline.Append( GLToy_Vector_3( 0.2f, 0.0f, 0.0f ) );
	s_xSpline.AppendOrientation( GLToy_Vector_3( -1.0f, 0.0f, 0.0f) );
    s_xSpline.Append( GLToy_Vector_3( 3.0f, 0.0f, 0.0f ) );
	s_xSpline.AppendOrientation( GLToy_Vector_3( -1.0f, 0.0f, 0.0f) );
	s_xSpline.Append( GLToy_Vector_3( 3.0f, 3.0f, 0.0f ) );
	s_xSpline.AppendOrientation( GLToy_Vector_3( -1.0f, -1.0f, 0.0f) );
	s_xSpline.Append( GLToy_Vector_3( 0.0f, 3.0f, 0.0f ) );
	s_xSpline.AppendOrientation( GLToy_Vector_3( 0.0f, -1.0f, 0.0f) );

	GLToy_Assert( s_xSpline.IsComplete(), "Incomplete oriented spline for camera!" );
}

void JD1_DemoScene_Chmutov::Shutdown()
{
}

void JD1_DemoScene_Chmutov::Render() const
{
    GLToy_Raytrace_Fullscreen xRaytrace( GLToy_Hash_Constant( "JD1_Raytrace_Chmutov_Hardcoded" ) );

    xRaytrace.BindTexture( "xTexture", GLToy_Hash_Constant( "generic/grid2.png" ) );

    xRaytrace.Render();
}

void JD1_DemoScene_Chmutov::Update()
{
    GLToy_Parent::Update();

    //const float fHeight = 0.5f * GLToy_Maths::Sin( 0.337f * GLToy_Timer::GetTime() );
    //GLToy_Vector_3 xCameraPosition = 
    //    GLToy_Vector_3(
    //        GLToy_Maths::Cos( GLToy_Timer::GetTime() * 0.701f ),
    //        0.85f * GLToy_Maths::Cos( GLToy_Timer::GetTime() * 0.377f ),
    //        GLToy_Maths::Sin( GLToy_Timer::GetTime() * 0.898f ) );
    //xCameraPosition.Normalise();
    //xCameraPosition *= ( 3.5f + fHeight );
    GLToy_Vector_3 xCameraPosition = s_xSpline.GetPoint( GLToy_Maths::Wrap( 0.05f * GLToy_Timer::GetTime() ) );
	GLToy_Matrix_3 xCameraOrientation = s_xSpline.GetOrientation( GLToy_Maths::Wrap( 0.05f * GLToy_Timer::GetTime() ) );
    GLToy_Camera::SetPosition( xCameraPosition );
	GLToy_Camera::SetOrientation( xCameraOrientation );
    //GLToy_Camera::LookAt( GLToy_Vector_3( 0.0f, 0.0f, 0.0f ) );
}

void JD1_DemoScene_Chmutov::Start()
{
    GLToy_Parent::Start();

    GLToy_Render::SetClearFrame( true );
    GLToy_Render::Clear();
}

void JD1_DemoScene_Chmutov::Stop()
{
    GLToy_Camera::Reset();
}
