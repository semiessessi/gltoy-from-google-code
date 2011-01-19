/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009-2011 Semi Essessi
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
#include <Maths/GLToy_AlignedVector.h>
#include <Maths/GLToy_Complex.h>
#include <Maths/GLToy_Matrix.h>
#include <Maths/GLToy_Plane.h>
#include <Maths/GLToy_Ray.h>
#include <Maths/GLToy_Quaternion.h>
#include <Maths/GLToy_Vector.h>
#include <Maths/Platform_GLToy_Maths.h>

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
	static const GLToy_AlignedVector_2 ZeroAlignedVector2;
    static const GLToy_AlignedVector_3 ZeroAlignedVector3;
    static const GLToy_AlignedVector_4 ZeroAlignedVector4;
    static const GLToy_Matrix_2 IdentityMatrix2;
    static const GLToy_Matrix_3 IdentityMatrix3;
    static const GLToy_Matrix_4 IdentityMatrix4;
    static const GLToy_Quaternion IdentityQuaternion;

    static bool Initialise();
    static void Shutdown();

    static void Update();

    static float Ceiling( const float fValue );
    static float Floor( const float fValue );
    static float Round( const float fValue );

    static float Random( const float fLower = 0.0f, const float fHigher = 1.0f );
    static GLToy_Vector_3 RandomDirection();

    static float Deg2Rad( const float fDegrees );
    static float Rad2Deg( const float fDegrees );

    template < class T > static GLToy_ForceInline T Max( const T xValue1, const T xValue2 ) { return ( xValue1 > xValue2 ) ? xValue1 : xValue2; }
	template < class T > static GLToy_ForceInline T Min( const T xValue1, const T xValue2 ) { return ( xValue1 < xValue2 ) ? xValue1 : xValue2; }
    template < class T > static GLToy_ForceInline T Clamp( const T xValue, const T xMin = 0.0f, const T xMax = 1.0f ) { return Min( xMax, Max( xValue, xMin ) ); }

	static GLToy_Inline float Wrap( const float fValue, const float fMin = 0.0f, const float fMax = 1.0f )
	{
        GLToy_Assert( fMax > fMin, "Minimum is greater than maximum!" );

		const float fRange = fMax - fMin;
		float fReturnValue = ( fValue - fMin ) / fRange;
		// lazy - avoiding library floating point mod
		const float fIntegerPart = Floor( fReturnValue );
		fReturnValue -= fIntegerPart;
		fReturnValue *= fRange;
		return fReturnValue + fMin;
	}

    static GLToy_Inline u_int Wrap( const u_int uValue, const u_int uMin = 0, const u_int uMax = 1 )
    {
        GLToy_Assert( uMax > uMin, "Minimum is greater than maximum!" );
        return uMin + ( ( uValue - uMin ) % ( uMax - uMin ) );
    }

	template < class T >
	static GLToy_Inline T Lerp( const T& xValue1, const T& xValue2, const float fAmount )
	{
		return xValue1 + ( xValue2 - xValue1 ) * fAmount;
	}

	template < class T >
    static GLToy_ForceInline T ClampedLerp( const T& xValue1, const T& xValue2, const float fAmount )
	{
		return Lerp( xValue1, xValue2, Clamp( fAmount, 0.0f, 1.0f ) );
	}

	template < class T >
    static GLToy_ForceInline T CosineInterpolate( const T& xValue1, const T& xValue2, const float fAmount )
	{
		return Lerp( xValue1, xValue2, 0.5f * ( 1.0f - Cos( Pi * fAmount ) ) );
	}

	template < class T >
    static GLToy_Inline T QuinticNoiseInterpolate( const T& xValue1, const T& xValue2, const float fAmount )
	{
		return Lerp( xValue1, xValue2, ( ( ( fAmount * 6.0f - 15.0f ) * fAmount + 10.0f ) * fAmount * fAmount * fAmount ) );
	}

	template < class T >
    static GLToy_ForceInline T ClampedCosineInterpolate( const T& xValue1, const T& xValue2, const float fAmount )
	{
		return CosineInterpolate( xValue1, xValue2, Clamp( fAmount, 0.0f, 1.0f ) );
	}

	template < class T >
    static GLToy_ForceInline T ClampedQuinticNoiseInterpolate( const T& xValue1, const T& xValue2, const float fAmount )
	{
		return QuinticNoiseInterpolate( xValue1, xValue2, Clamp( fAmount, 0.0f, 1.0f ) );
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
		const T xB = xValue1 - 2.5f * xValue2 + 2.0f * xValue3 - 0.5f * xValue4;
		const T xC = 0.5f * ( xValue3 - xValue1 );
		const T xD = xValue2;

		return ( ( xA * fAmount + xB ) * fAmount + xC ) * fAmount + xD;
	}

    static GLToy_ForceInline float Sin( const float fValue ) { return Platform_GLToy_Maths::Sin( fValue ); }
    static GLToy_ForceInline float Cos( const float fValue ) { return Platform_GLToy_Maths::Cos( fValue ); }
    static GLToy_ForceInline float Tan( const float fValue ) { return Platform_GLToy_Maths::Tan( fValue ); }
    static GLToy_ForceInline float Cot( const float fValue ) { return Platform_GLToy_Maths::Tan( fValue + 0.785398163f ); }
    static GLToy_ForceInline float Sec( const float fValue ) { return 1.0f / Platform_GLToy_Maths::Cos( fValue ); }
    static GLToy_ForceInline float Csc( const float fValue ) { return 1.0f / Platform_GLToy_Maths::Sin( fValue ); }

    static float ASin( const float fValue );
    static float ACos( const float fValue );
    static float ATan( const float fValue );
    static float ATan( const float fY, const float fX );

    static float Abs( const float fValue );
	static GLToy_ForceInline float InvSqrt( const float fValue ) { return 1.0f / Sqrt( fValue ); } // shocking I know...
    static GLToy_ForceInline float Sqrt( const float fValue ) { return Platform_GLToy_Maths::Sqrt( fValue ); }
    static float Pow( const float fValue, const float fPower );

    template < class T > static GLToy_Inline T Reflect( const T& xVector, const T& xNormal ) { return xVector - 2.0f * ( xVector * xNormal ) * xNormal; }
    static GLToy_Vector_2 Rotate_2D( const GLToy_Vector_2& xVector, const float fAngle );
    static GLToy_Vector_2 Rotate_2D_FromCosSin( const GLToy_Vector_2& xVector, const float fCos, const float fSin );
    static GLToy_Vector_3 Rotate_AxisAngle( const GLToy_Vector_3& xVector, const GLToy_Vector_3& xAxis, const float fAngle );
	static GLToy_Matrix_3 Orientation_FromDirection( const GLToy_Vector_3& xDirection );
    static GLToy_Matrix_3 Orientation_FromDirectionAndUp( const GLToy_Vector_3& xDirection, const GLToy_Vector_3 xUp = GLToy_Vector_3( 0.0f, 1.0f, 0.0f ) );

    static GLToy_Inline float SolveLinear( const float fA, const float fB ) { return ( fA == 0.0f ) ? LargeFloat : ( -fB / fA ); }
    static GLToy_Vector_2 SolveQuadratic( const float fA, const float fB, const float fC );
    // TODO: ...
    //static GLToy_Vector_3 SolveCubic( const float fA, const float fB, const float fC, const float fD );
    //static GLToy_Vector_3 SolveQuartic( const float fA, const float fB, const float fC, const float fD, const float fE );

    static GLToy_Inline GLToy_Vector_2 StereographicProjection( const GLToy_Vector_3& xNormalised ) { return GLToy_Vector_2( xNormalised.x, xNormalised.y ) / ( 1.0f - xNormalised.z ); }
    static GLToy_Inline GLToy_Vector_3 InverseStereographicProjection( const GLToy_Vector_2& xProjected ) { float fSumOfSquares = xProjected * xProjected; return GLToy_Vector_3( 2.0f * xProjected.x, 2.0f * xProjected.y, fSumOfSquares - 1.0f ) / ( 1.0f + fSumOfSquares ); }
	//vec3 ReconstructNormal( vec2 xCompressed )
	//{
	//	float fIntermediaryValue = xCompressed.y * 2.0 - 1.0;
	//	vec3 xReturnValue = vec3( xCompressed.x - 0.5, abs( fIntermediaryValue ) - 0.5, sign( fIntermediaryValue ) );
	//	return xReturnValue * vec3( 2.0, 2.0, 1.0 - sqrt( dot( xReturnValue.xy, xReturnValue.xy ) ) );
	//}

	//vec2 CompressNormal( vec3 xNormalised )
	//{
	//	vec2 xReturnValue = 0.5 * xNormalised.xy + 0.5;
	//	xReturnValue.y = 0.5 * sign( xNormalised.z ) * xReturnValue.y + 0.5;
	//	return xReturnValue;
	//}
	//static GLToy_Inline GLToy_Vector_2 CompressNormal( const GLToy_Vector_3& xNormalised ) { return GLToy_Vector_2( 0.0f, 0.0f ); }
    //static GLToy_Inline GLToy_Vector_3 ReconstructNormal( const GLToy_Vector_2& xCompressed ) { return GLToy_Vector_3( 0.0f, 0.0f, 0.0f ); }


private:
    
    static bool Platform_Initialise();
    static void Platform_Shutdown();

    static void Platform_Update();

};

#endif
