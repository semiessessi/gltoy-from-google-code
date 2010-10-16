/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009, 2010 Semi Essessi
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
#include <Maths/GLToy_Maths.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Maths/Platform_GLToy_Vector.h>
#include <Test/GLToy_Test_System.h>

// C++ headers (!)
#include <math.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

const float GLToy_Maths::LargeFloat = 1.0e+35f;
const GLToy_Vector_2 GLToy_Maths::LargeVector2 = GLToy_Vector_2( 1.0e+35f, 1.0e+35f );
const GLToy_Vector_3 GLToy_Maths::LargeVector3 = GLToy_Vector_3( 1.0e+35f, 1.0e+35f, 1.0e+35f );
const float GLToy_Maths::Pi = 3.141592654f;
const GLToy_Vector_2 GLToy_Maths::ZeroVector2 = GLToy_Vector_2();
const GLToy_Vector_3 GLToy_Maths::ZeroVector3 = GLToy_Vector_3();
const GLToy_Vector_4 GLToy_Maths::ZeroVector4 = GLToy_Vector_4();
const GLToy_Matrix_3 GLToy_Maths::IdentityMatrix3 = GLToy_Matrix_3( GLToy_Vector_3( 1.0f, 0.0f, 0.0f ), GLToy_Vector_3( 0.0f, 1.0f, 0.0f ), GLToy_Vector_3( 0.0f, 0.0f, 1.0f ) );
const GLToy_Quaternion GLToy_Maths::IdentityQuaternion = GLToy_Quaternion( 1.0f, 0.0f, 0.0f, 0.0f );

/////////////////////////////////////////////////////////////////////////////////////////////
// T E S T S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Maths_AbsoluteValueTest()
{
    return ( GLToy_Maths::Abs( -1.0f ) >= 0.0f )
        && ( GLToy_Maths::Abs( 0.0f ) >= 0.0f )
        && ( GLToy_Maths::Abs( 1.0f ) >= 0.0f );
}

bool GLToy_Maths_TrigIdentityTest()
{
    return ( GLToy_Maths::Abs( GLToy_Maths::Sin( 0.0f ) ) < 0.00001f )
        && ( GLToy_Maths::Abs( GLToy_Maths::Cos( 0.0f ) - 1.0f ) < 0.00001f );
}

bool GLToy_Maths_SqrtTest()
{
    #define GLToy_Maths_SqrtTest_Test( fValue ) ( GLToy_Maths::Abs( GLToy_Maths::Sqrt( fValue ) * GLToy_Maths::Sqrt( fValue ) - fValue ) / fValue )
    float fMaxError = GLToy_Maths_SqrtTest_Test( 5.0f );
    fMaxError = GLToy_Maths::Max( fMaxError, GLToy_Maths_SqrtTest_Test( 10000.0f ) );
    fMaxError = GLToy_Maths::Max( fMaxError, GLToy_Maths_SqrtTest_Test( 1000.0f ) );
    fMaxError = GLToy_Maths::Max( fMaxError, GLToy_Maths_SqrtTest_Test( 1000000.0f ) );
    #undef GLToy_Maths_SqrtTest_Test

    return fMaxError < 0.005f;
}

bool GLToy_Maths_DotTest()
{
    #define GLToy_Maths_DotTest_Test( xValue1, xValue2 ) ( GLToy_Maths::Abs( ( xValue1 * xValue2 ) - ( xValue1[ 0 ] * xValue2[ 0 ] + xValue1[ 1 ] * xValue2[ 1 ] + xValue1[ 2 ] * xValue2[ 2 ] ) ) / ( xValue1[ 0 ] * xValue2[ 0 ] + xValue1[ 1 ] * xValue2[ 1 ] + xValue1[ 2 ] * xValue2[ 2 ] ) )
    float fMaxError = GLToy_Maths_DotTest_Test( GLToy_Vector_3( 1.0f, -1.0f, 2.0f ), GLToy_Vector_3( 11.11f, 2311.11f, -2391.121f ) );
    fMaxError = GLToy_Maths::Max( fMaxError, GLToy_Maths_DotTest_Test( GLToy_Vector_3( -1.0f, -0.5f, 0.0f ), GLToy_Vector_3( 1.11f, 0.11f, -0.121f ) ) );
    #undef GLToy_Maths_DotTest_Test

    return fMaxError < 0.0005f;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Maths::Initialise()
{
    GLToy_Test_System::RegisterTest( GLToy_Maths_AbsoluteValueTest, "Absolute value really is positive" );
    GLToy_Test_System::RegisterTest( GLToy_Maths_TrigIdentityTest, "Trig identities accurate to 5 decimal places" );
    GLToy_Test_System::RegisterTest( GLToy_Maths_SqrtTest, "Square of square root accurate to 0.5%" );
    GLToy_Test_System::RegisterTest( GLToy_Maths_DotTest, "Dot product accurate to 0.05%" );

    return Platform_Initialise();
}

void GLToy_Maths::Shutdown()
{
    Platform_Shutdown();
}

void GLToy_Maths::Update()
{
    Platform_Update();
}

bool GLToy_Maths::Platform_Initialise()
{
    return Platform_GLToy_Maths::Initialise() && Platform_GLToy_Vector::Initialise();
}

void GLToy_Maths::Platform_Shutdown()
{
}

void GLToy_Maths::Platform_Update()
{
    Platform_GLToy_Maths::Update();
}

float GLToy_Maths::ACos( const float fValue )
{
    return acos( fValue );
}

float GLToy_Maths::Abs( const float fValue )
{
    return fabs( fValue );
}

float GLToy_Maths::Pow( const float fValue, const float fPower )
{
    return powf( fValue, fPower );
}

GLToy_Vector_3 GLToy_Maths::Rotate_AxisAngle( const GLToy_Vector_3& xVector, const GLToy_Vector_3& xAxis, const float fAngle )
{
    if( fAngle == 0.0f )
    {
        return xVector;
    }

    // TODO: this can be optimised a load...
    const float fXSquared = xAxis[ 0 ] * xAxis[ 0 ];
    const float fYSquared = xAxis[ 1 ] * xAxis[ 1 ];
    const float fZSquared = xAxis[ 2 ] * xAxis[ 2 ];
    const float fXY = xAxis[ 0 ] * xAxis[ 1 ];
    const float fYZ = xAxis[ 1 ] * xAxis[ 2 ];
    const float fZX = xAxis[ 2 ] * xAxis[ 0 ];
    const float fC = Cos( fAngle );
    const float fS = Sin( fAngle );

    return xVector * GLToy_Matrix_3( 

            fXSquared + ( 1.0f - fXSquared ) * fC,     fXY * ( 1.0f - fC ) - xAxis[ 2 ] * fS,       fZX * ( 1.0f - fC ) + xAxis[ 1 ] * fS,

            fXY * ( 1.0f - fC ) + xAxis[ 2 ] * fS,     fYSquared + ( 1.0f - fYSquared ) * fC,       fYZ * ( 1.0f - fC ) - xAxis[ 0 ] * fS,

            fZX * ( 1.0f - fC ) - xAxis[ 1 ] * fS,     fYZ * ( 1.0f - fC ) + xAxis[ 0 ] * fS,       fZSquared + ( 1.0f - fZSquared ) * fC

        );
}

GLToy_Matrix_3 GLToy_Maths::Orientation_FromDirection( const GLToy_Vector_3& xDirection )
{
	GLToy_Vector_3 xFixedDirection = xDirection;
	xFixedDirection.Normalise();

	GLToy_Vector_3 xFixedRight = GLToy_Vector_3( xDirection[ 1 ], xDirection[ 2 ], xDirection[ 0 ] );
	xFixedRight.Normalise();
	xFixedRight = xFixedRight.Cross( xFixedDirection );

	GLToy_Matrix_3 xReturnValue( xFixedRight, xFixedDirection.Cross( xFixedRight ), xFixedDirection );
	xReturnValue.Orthonormalise();

	return xReturnValue;
}


GLToy_Matrix_3 GLToy_Maths::Orientation_FromDirectionAndUp( const GLToy_Vector_3& xDirection, const GLToy_Vector_3 xUp )
{
	GLToy_Vector_3 xFixedDirection = xDirection;
	xFixedDirection.Normalise();

	GLToy_Vector_3 xFixedRight = xUp;
	xFixedRight.Normalise();
	xFixedRight = xFixedRight.Cross( xFixedDirection );

	GLToy_Matrix_3 xReturnValue( xFixedRight, xFixedDirection.Cross( xFixedRight ), xFixedDirection );
	xReturnValue.Orthonormalise();

	return xReturnValue;
}

float GLToy_Maths::Ceiling( const float fValue )
{
    return ceil( fValue );
}

float GLToy_Maths::Floor( const float fValue )
{
    return floor( fValue );
}

float GLToy_Maths::Round( const float fValue )
{
    return Floor( fValue + 0.5f );
}

float GLToy_Maths::Random( const float fLower, const float fHigher )
{
    static u_int uWorkingValue = 0xA183E191;

    u_int u = uWorkingValue;
    u ^= u << 24;
    u -= ~( u << 6 );
    uWorkingValue = u * 16807 + 2147483647;

    return fLower + ( ( fHigher - fLower ) * uWorkingValue ) / static_cast< float >( 0xFFFFFFFF );
}

float GLToy_Maths::Deg2Rad( const float fValue )
{
    static const float ls_fConversionFactor = Pi / 180.0f;
    return ls_fConversionFactor * fValue;
}

float GLToy_Maths::Rad2Deg( const float fValue )
{
    static const float ls_fConversionFactor = 180.0f / Pi;
    return ls_fConversionFactor * fValue;
}
