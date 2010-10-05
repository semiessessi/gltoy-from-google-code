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

#ifndef __GLTOY_SPLINE_H_
#define __GLTOY_SPLINE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parent
#include <Core/Data Structures/GLToy_Array.h>

// GLToy
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class T >
class GLToy_Spline
: public GLToy_Array< T >
{

	typedef GLToy_Array< T > GLToy_Parent;

public:

	GLToy_Spline()
	: GLToy_Parent()
    {
    }

	GLToy_Spline( const GLToy_Spline& xSpline )
    : GLToy_Parent( xSpline )
	{
	}

	virtual ~GLToy_Spline() {}

	virtual T GetPoint( const float fParameter ) const
	{
		return SplineLerp( GLToy_Maths::Clamp( fParameter, 0.0f, 1.0f ) * GetLength() );
	}

    virtual T GetDerivative( const float fParameter ) const
	{
		return GetDerivativeFromScaledParameter( GLToy_Maths::Clamp( fParameter, 0.0f, 1.0f ) * GetLength() );
	}

	virtual float GetLength() const { return GetLengthToPoint( GetCount() - 1 ); }

protected:

	virtual T SplineLerp( const float fParameter ) const = 0;
	virtual T GetDerivativeFromScaledParameter( const float fParameter ) const = 0;
	virtual float GetSegmentLength( const u_int uFirstPointIndex ) const = 0;

	virtual float GetLengthToPoint( const u_int uPointIndex ) const
	{
		float fLength = 0.0f;
		for( u_int u = 0; u < uPointIndex; ++u )
		{
			fLength += GetSegmentLength( u );
		}

		return fLength;
	}

};

template < class T >
class GLToy_Spline_Linear
: public GLToy_Spline< T >
{

protected:

    virtual T GetDerivativeFromScaledParameter( const float fParameter ) const
    {
        u_int uIndex = 0;
		while( GetLengthToPoint( uIndex + 1 ) < fParameter )
		{
			++uIndex;
		}

        return ( ( *this )[ uIndex ] - ( *this )[ uIndex + 1 ] ) / GetSegmentLength( uIndex );
    }

	virtual float GetSegmentLength( const u_int uFirstPointIndex ) const
	{
		T xDifference = ( *this )[ uFirstPointIndex + 1 ] - ( *this )[ uFirstPointIndex ];
		return GLToy_Maths::Sqrt( xDifference * xDifference );
	}

	virtual T SplineLerp( const float fParameter ) const
	{
		u_int uIndex = 0;
		while( GetLengthToPoint( uIndex + 1 ) < fParameter )
		{
			++uIndex;
		}

		const float fSegmentLength = GetSegmentLength( uIndex );
		const float fLengthToPoint = GetLengthToPoint( uIndex );

		return GLToy_Maths::Lerp( ( *this )[ uIndex ], ( *this )[ uIndex + 1 ], ( fParameter - fLengthToPoint ) / fSegmentLength );
	}

};

template < class T >
class GLToy_Spline_Cubic
: public GLToy_Spline< T >
{

protected:

    virtual T GetDerivativeFromScaledParameter( const float fParameter ) const
    {
        u_int uIndex2 = 0;
		while( GetLengthToPoint( uIndex2 + 1 ) < fParameter )
		{
			++uIndex2;
		}

        const u_int uMax = GetCount() - 1;
		const u_int uIndex1 = ( uIndex2 == 0 ) ? 0 : uIndex2 - 1;
		const u_int uIndex3 = ( uIndex2 == uMax ) ? uMax : uIndex2 + 1;
		const u_int uIndex4 = ( uIndex3 == uMax ) ? uMax : uIndex3 + 1;

        const T& xValue1 = ( *this )[ uIndex1 ];
        const T& xValue2 = ( *this )[ uIndex2 ];
        const T& xValue3 = ( *this )[ uIndex3 ];
        const T& xValue4 = ( *this )[ uIndex4 ];

        const T xA = xValue4 - xValue3 + xValue2 - xValue1;
		const T xB = xValue1 - xValue2 - xA;
		const T xC = xValue3 - xValue1;
		const T xD = xValue2;

		return ( ( xA * 2.0f * fParameter + xB ) * fParameter + xC ) / GetSegmentLength( uIndex2 );
    }

    // the general problem is very difficult, for now take 3 segments and add them up...
	virtual float GetSegmentLength( const u_int uFirstPointIndex ) const
	{
        const u_int uIndex2 = uFirstPointIndex;
        const u_int uMax = GetCount() - 1;
		const u_int uIndex1 = ( uIndex2 == 0 ) ? 0 : uIndex2 - 1;
		const u_int uIndex3 = ( uIndex2 == uMax ) ? uMax : uIndex2 + 1;
		const u_int uIndex4 = ( uIndex3 == uMax ) ? uMax : uIndex3 + 1;

        const T xStart = ( *this )[ uIndex2 ];
        const T xEnd = ( *this )[ uIndex3 ];
        const T xMidPoint1 = GLToy_Maths::CubicInterpolate( ( *this )[ uIndex1 ], ( *this )[ uIndex2 ], ( *this )[ uIndex3 ], ( *this )[ uIndex4 ], 0.333333f );
        const T xMidPoint2 = GLToy_Maths::CubicInterpolate( ( *this )[ uIndex1 ], ( *this )[ uIndex2 ], ( *this )[ uIndex3 ], ( *this )[ uIndex4 ], 0.666666f );
		
        const T xDifference1 = xEnd - xMidPoint2;
        const T xDifference2 = xMidPoint2 - xMidPoint1;
        const T xDifference3 = xMidPoint1 - xStart;

		return GLToy_Maths::Sqrt( xDifference1 * xDifference1 ) + GLToy_Maths::Sqrt( xDifference2 * xDifference2 ) + GLToy_Maths::Sqrt( xDifference3 * xDifference3 );
	}

	virtual T SplineLerp( const float fParameter ) const
	{
		u_int uIndex2 = 0;
		while( GetLengthToPoint( uIndex2 + 1 ) < fParameter )
		{
			++uIndex2;
		}

		const u_int uMax = GetCount() - 1;
		const u_int uIndex1 = ( uIndex2 == 0 ) ? 0 : uIndex2 - 1;
		const u_int uIndex3 = ( uIndex2 == uMax ) ? uMax : uIndex2 + 1;
		const u_int uIndex4 = ( uIndex3 == uMax ) ? uMax : uIndex3 + 1;

		const float fSegmentLength = GetSegmentLength( uIndex2 );
		const float fLengthToPoint = GetLengthToPoint( uIndex2 );

		return GLToy_Maths::CubicInterpolate( ( *this )[ uIndex1 ], ( *this )[ uIndex2 ], ( *this )[ uIndex3 ], ( *this )[ uIndex4 ], ( fParameter - fLengthToPoint ) / fSegmentLength );
	}

};

template < class T >
class GLToy_Spline_CatmullRom
: public GLToy_Spline< T >
{

protected:

    virtual T GetDerivativeFromScaledParameter( const float fParameter ) const
    {
        u_int uIndex2 = 0;
		while( GetLengthToPoint( uIndex2 + 1 ) < fParameter )
		{
			++uIndex2;
		}

        const u_int uMax = GetCount() - 1;
		const u_int uIndex1 = ( uIndex2 == 0 ) ? 0 : uIndex2 - 1;
		const u_int uIndex3 = ( uIndex2 == uMax ) ? uMax : uIndex2 + 1;
		const u_int uIndex4 = ( uIndex3 == uMax ) ? uMax : uIndex3 + 1;

        const T& xValue1 = ( *this )[ uIndex1 ];
        const T& xValue2 = ( *this )[ uIndex2 ];
        const T& xValue3 = ( *this )[ uIndex3 ];
        const T& xValue4 = ( *this )[ uIndex4 ];

        const T xA = xValue4 - xValue3 + xValue2 - xValue1;
		const T xB = xValue1 - xValue2 - xA;
		const T xC = xValue3 - xValue1;
		const T xD = xValue2;

		return ( ( xA * 2.0f * fParameter + xB ) * fParameter + xC ) / GetSegmentLength( uIndex2 );
    }

	// the general problem is very difficult, for now take 3 segments and add them up...
	virtual float GetSegmentLength( const u_int uFirstPointIndex ) const
	{
		const u_int uIndex2 = uFirstPointIndex;
        const u_int uMax = GetCount() - 1;
		const u_int uIndex1 = ( uIndex2 == 0 ) ? 0 : uIndex2 - 1;
		const u_int uIndex3 = ( uIndex2 == uMax ) ? uMax : uIndex2 + 1;
		const u_int uIndex4 = ( uIndex3 == uMax ) ? uMax : uIndex3 + 1;

        const T xStart = ( *this )[ uIndex2 ];
        const T xEnd = ( *this )[ uIndex3 ];
        const T xMidPoint1 = GLToy_Maths::CatmullRomInterpolate( ( *this )[ uIndex1 ], ( *this )[ uIndex2 ], ( *this )[ uIndex3 ], ( *this )[ uIndex4 ], 0.333333f );
        const T xMidPoint2 = GLToy_Maths::CatmullRomInterpolate( ( *this )[ uIndex1 ], ( *this )[ uIndex2 ], ( *this )[ uIndex3 ], ( *this )[ uIndex4 ], 0.666666f );
		
        const T xDifference1 = xEnd - xMidPoint2;
        const T xDifference2 = xMidPoint2 - xMidPoint1;
        const T xDifference3 = xMidPoint1 - xStart;

		return GLToy_Maths::Sqrt( xDifference1 * xDifference1 ) + GLToy_Maths::Sqrt( xDifference2 * xDifference2 ) + GLToy_Maths::Sqrt( xDifference3 * xDifference3 );
	}

	virtual T SplineLerp( const float fParameter ) const
	{
		u_int uIndex2 = 0;
		while( GetLengthToPoint( uIndex2 + 1 ) < fParameter )
		{
			++uIndex2;
		}

		const u_int uMax = GetCount() - 1;
		const u_int uIndex1 = ( uIndex2 == 0 ) ? 0 : uIndex2 - 1;
		const u_int uIndex3 = ( uIndex2 == uMax ) ? uMax : uIndex2 + 1;
		const u_int uIndex4 = ( uIndex3 == uMax ) ? uMax : uIndex3 + 1;

		const float fSegmentLength = GetSegmentLength( uIndex2 );
		const float fLengthToPoint = GetLengthToPoint( uIndex2 );

		return GLToy_Maths::CatmullRomInterpolate( ( *this )[ uIndex1 ], ( *this )[ uIndex2 ], ( *this )[ uIndex3 ], ( *this )[ uIndex4 ], ( fParameter - fLengthToPoint ) / fSegmentLength );
	}

};

#endif