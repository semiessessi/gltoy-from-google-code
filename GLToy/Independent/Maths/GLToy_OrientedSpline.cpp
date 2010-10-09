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

#include <Core/GLToy.h>

// This file's header
#include <Maths/GLToy_OrientedSpline.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Matrix_3 GLToy_OrientedSpline::LinearOrientationInterpolate( const float fParameter ) const
{
	u_int uIndex = 0;
	while( GetLengthToPoint( uIndex + 1 ) < fParameter )
	{
		++uIndex;
	}

	const float fSegmentLength = GetSegmentLength( uIndex );
	const float fLengthToPoint = GetLengthToPoint( uIndex );

	return GLToy_Maths::Lerp(
			GLToy_Quaternion( m_xOrientations[ uIndex ] ),
			GLToy_Quaternion( m_xOrientations[ uIndex + 1 ] ),
			( fParameter - fLengthToPoint ) / fSegmentLength
		).GetOrientationMatrix();
}

GLToy_Matrix_3 GLToy_OrientedSpline::CubicOrientationInterpolate( const float fParameter ) const
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

	return GLToy_Maths::CubicInterpolate(
			GLToy_Quaternion( m_xOrientations[ uIndex1 ] ),
			GLToy_Quaternion( m_xOrientations[ uIndex2 ] ),
			GLToy_Quaternion( m_xOrientations[ uIndex3 ] ),
			GLToy_Quaternion( m_xOrientations[ uIndex4 ] ),
			( fParameter - fLengthToPoint ) / fSegmentLength
		).GetOrientationMatrix();
}

GLToy_Matrix_3 GLToy_OrientedSpline::CatmullRomOrientationInterpolate( const float fParameter ) const
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

	return GLToy_Maths::CatmullRomInterpolate(
			GLToy_Quaternion( m_xOrientations[ uIndex1 ] ),
			GLToy_Quaternion( m_xOrientations[ uIndex2 ] ),
			GLToy_Quaternion( m_xOrientations[ uIndex3 ] ),
			GLToy_Quaternion( m_xOrientations[ uIndex4 ] ),
			( fParameter - fLengthToPoint ) / fSegmentLength
		).GetOrientationMatrix();
}

GLToy_Matrix_3 GLToy_OrientedSpline::GetNaturalOrientation( const float fParameter ) const
{
	return GLToy_Maths::Orientation_FromDirection( GetDerivative( fParameter ) );
}