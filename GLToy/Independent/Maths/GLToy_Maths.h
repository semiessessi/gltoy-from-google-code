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

#ifndef __GLTOY_MATHS_H_
#define __GLTOY_MATHS_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Maths/GLToy_Matrix.h>
#include <Maths/GLToy_Plane.h>
#include <Maths/GLToy_Ray.h>
#include <Maths/GLToy_Quaternion.h>
#include <Maths/GLToy_Vector.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Maths
{

public:
    
    static const float LargeFloat;
    static const GLToy_Vector_2 LargeVector2;
    static const GLToy_Vector_3 LargeVector3;
    static const float Pi;
    static const GLToy_Vector_2 ZeroVector2;
    static const GLToy_Vector_3 ZeroVector3;
    static const GLToy_Vector_4 ZeroVector4;
    static const GLToy_Matrix_3 IdentityMatrix3;
    static const GLToy_Quaternion IdentityQuaternion;

    static bool Initialise();
    static void Shutdown();

    static void Update();

    static float Ceiling( const float fValue );
    static float Floor( const float fValue );
    static float Round( const float fValue );

    static float Random( const float fLower = 0.0f, const float fHigher = 1.0f );

    static float Deg2Rad( const float fDegrees );
    static float Rad2Deg( const float fDegrees );

    static GLToy_Inline float Clamp( const float fValue, const float fMin = 0.0f, const float fMax = 1.0f ) { return Min( fMax, Max( fValue, fMin ) ); }
    static GLToy_Inline float Max( const float fValue1, const float fValue2 ) { return ( fValue1 > fValue2 ) ? fValue1 : fValue2; }
    static GLToy_Inline float Min( const float fValue1, const float fValue2 ) { return ( fValue1 < fValue2 ) ? fValue1 : fValue2; }
    static GLToy_Inline int Max( const int iValue1, const int iValue2 ) { return ( iValue1 > iValue2 ) ? iValue1 : iValue2; }
	static GLToy_Inline int Min( const int iValue1, const int iValue2 ) { return ( iValue1 < iValue2 ) ? iValue1 : iValue2; }

	static GLToy_Inline float Wrap( const float fValue, const float fMin = 0.0f, const float fMax = 1.0f )
	{
		const float fRange = fMax - fMin;
		float fReturnValue = ( fValue - fMin ) / fRange;
		// lazy - avoiding library floating point mod
		const float fIntegerPart = Floor( fReturnValue );
		fReturnValue -= fIntegerPart;
		fReturnValue *= fRange;
		return fReturnValue;
	}

	template < class T >
	static GLToy_Inline T Lerp( const T& xValue1, const T& xValue2, const float fAmount )
	{
		return xValue1 + ( xValue2 - xValue1 ) * fAmount;
	}

	template < class T >
    static GLToy_Inline T ClampedLerp( const T& xValue1, const T& xValue2, const float fAmount )
	{
		return Lerp( xValue1, xValue2, Clamp( fAmount, 0.0f, 1.0f ) );
	}

	template < class T >
    static GLToy_Inline T CosineInterpolate( const T& xValue1, const T& xValue2, const float fAmount )
	{
		return xValue1 + ( xValue2 - xValue1 ) * 0.5f * ( 1.0f - Cos( Pi * fAmount ) );
	}
	
	template < class T >
	static GLToy_Inline T CubicInterpolate( const T& xValue1, const T& xValue2, const T& xValue3, const T& xValue4, const float fAmount )
	{
		const T xA = xValue4 - xValue3 + xValue2 - xValue1;
		const T xB = xValue1 - xValue2 - xA;
		const T xC = xValue3 - xValue1;
		const T xD = xValue2;

		return ( ( xA * fAmount + xB ) * fAmount + xC ) * fAmount + xD;
	}

	template < class T >
	static GLToy_Inline T CatmullRomInterpolate( const T& xValue1, const T& xValue2, const T& xValue3, const T& xValue4, const float fAmount )
	{
		const T xA = 0.5f * xValue4 - 1.5f * xValue3 + 1.5f * xValue2 - 0.5f * xValue1;
		const T xB = xValue1 - 2.5f * xValue2 + 2.0f * xValue3 - 0.5f* xValue4;
		const T xC = 0.5f * ( xValue3 - xValue1 );
		const T xD = xValue2;

		return ( ( xA * fAmount + xB ) * fAmount + xC ) * fAmount + xD;
	}

    static float Sin( const float fValue );
    static float Cos( const float fValue );

    static float ACos( const float fValue );

    static float Abs( const float fValue );
	static float InvSqrt( const float fValue ) { return 1.0f / Sqrt( fValue ); } // shocking I know...
    static float Sqrt( const float fValue );
    static float Pow( const float fValue, const float fPower );

    static GLToy_Vector_3 Rotate_AxisAngle( const GLToy_Vector_3& xVector, const GLToy_Vector_3& xAxis, const float fAngle );

private:
    
    static bool Platform_Initialise();
    static void Platform_Shutdown();

    static void Platform_Update();

};

#endif
