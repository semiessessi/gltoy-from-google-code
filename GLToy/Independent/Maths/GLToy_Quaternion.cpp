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
#include <Maths/GLToy_Quaternion.h>

// GLToy
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Quaternion::GLToy_Quaternion( const GLToy_Matrix_3& xOrientation )
{
	const float fTrace = xOrientation[ 0 ][ 0 ] + xOrientation[ 1 ][ 1 ] + xOrientation[ 2 ][ 2 ] + 1.0f;

	if( fTrace > 0.0f )
	{
		const float fScale = 0.5f * GLToy_Maths::InvSqrt( fTrace );
		m_fComponents[ 0 ] = 0.5f * fTrace;
		m_fComponents[ 1 ] = fScale * ( xOrientation[ 2 ][ 1 ] - xOrientation[ 1 ][ 2 ] );
		m_fComponents[ 2 ] = fScale * ( xOrientation[ 0 ][ 2 ] - xOrientation[ 2 ][ 0 ] );
		m_fComponents[ 3 ] = fScale * ( xOrientation[ 1 ][ 0 ] - xOrientation[ 0 ][ 1 ] );
	}
	else if( ( xOrientation[ 0 ][ 0 ] > xOrientation[ 1 ][ 1 ] ) && ( xOrientation[ 0 ][ 0 ] > xOrientation[ 2 ][ 2 ] ) )
	{
		const float fSortOfTrace = 1.0f + xOrientation[ 0 ][ 0 ] - xOrientation[ 1 ][ 1 ] - xOrientation[ 2 ][ 2 ];
		const float fScale = 0.5f * GLToy_Maths::InvSqrt( fSortOfTrace );
		m_fComponents[ 0 ] = fScale * ( xOrientation[ 2 ][ 1 ] + xOrientation[ 1 ][ 2 ] );
		m_fComponents[ 1 ] = 0.5f * fSortOfTrace;
		m_fComponents[ 2 ] = fScale * ( xOrientation[ 1 ][ 0 ] + xOrientation[ 0 ][ 1 ] );
		m_fComponents[ 3 ] = fScale * ( xOrientation[ 0 ][ 2 ] + xOrientation[ 2 ][ 0 ] );
	}
	else if( xOrientation[ 1 ][ 1 ] > xOrientation[ 2 ][ 2 ] )
	{
		const float fSortOfTrace = 1.0f + xOrientation[ 1 ][ 1 ] - xOrientation[ 2 ][ 2 ] - xOrientation[ 0 ][ 0 ];
		const float fScale = 0.5f * GLToy_Maths::InvSqrt( fSortOfTrace );
		m_fComponents[ 0 ] = fScale * ( xOrientation[ 0 ][ 2 ] + xOrientation[ 2 ][ 0 ] );
		m_fComponents[ 1 ] = fScale * ( xOrientation[ 1 ][ 0 ] + xOrientation[ 0 ][ 1 ] );
		m_fComponents[ 2 ] = 0.5f * fSortOfTrace;
		m_fComponents[ 3 ] = fScale * ( xOrientation[ 2 ][ 1 ] + xOrientation[ 1 ][ 2 ] );
	}
	else
	{
		const float fSortOfTrace = 1.0f + xOrientation[ 2 ][ 2 ] - xOrientation[ 0 ][ 0 ] - xOrientation[ 1 ][ 1 ];
		const float fScale = 0.5f * GLToy_Maths::InvSqrt( fSortOfTrace );
		m_fComponents[ 0 ] = fScale * ( xOrientation[ 1 ][ 0 ] + xOrientation[ 0 ][ 1 ] );
		m_fComponents[ 1 ] = fScale * ( xOrientation[ 2 ][ 1 ] + xOrientation[ 1 ][ 2 ] );
		m_fComponents[ 2 ] = fScale * ( xOrientation[ 0 ][ 2 ] + xOrientation[ 2 ][ 0 ] );
		m_fComponents[ 3 ] = 0.5f * fSortOfTrace;
	}
}

GLToy_Quaternion GLToy_Quaternion::operator +( const GLToy_Quaternion& xQuaternion ) const
{
    return GLToy_Quaternion(
		m_fComponents[ 0 ] + xQuaternion[ 0 ],
		m_fComponents[ 1 ] + xQuaternion[ 1 ],
		m_fComponents[ 2 ] + xQuaternion[ 2 ],
		m_fComponents[ 3 ] + xQuaternion[ 3 ]
	);
}

GLToy_Quaternion GLToy_Quaternion::operator -( const GLToy_Quaternion& xQuaternion ) const
{
    return GLToy_Quaternion(
		m_fComponents[ 0 ] - xQuaternion[ 0 ],
		m_fComponents[ 1 ] - xQuaternion[ 1 ],
		m_fComponents[ 2 ] - xQuaternion[ 2 ],
		m_fComponents[ 3 ] - xQuaternion[ 3 ]
	);
}

GLToy_Quaternion GLToy_Quaternion::operator *( const float fFloat ) const
{
    return GLToy_Quaternion(
		fFloat * m_fComponents[ 0 ],
		fFloat * m_fComponents[ 1 ],
		fFloat * m_fComponents[ 2 ],
		fFloat * m_fComponents[ 3 ]
	);
}

GLToy_Matrix_3 GLToy_Quaternion::GetOrientationMatrix()
{
    const float& w = m_fComponents[ 0 ];
    const float& x = m_fComponents[ 1 ];
    const float& y = m_fComponents[ 2 ];
    const float& z = m_fComponents[ 3 ];

    GLToy_Matrix_3 xReturnValue(

        w*w + x*x - y*y - z*z,      2*x*y - 2*z*w,              2*x*z + 2*w*y,
        2*x*y + 2*w*z,              w*w - x*x + y*y - z*z,      2*y*z - 2*w*x,
        2*x*z - 2*w*y,              2*y*z + 2*w*x,              w*w - x*x - y*y + z*z

        );

    return xReturnValue;
}

float GLToy_Quaternion::Magnitude() const
{
	return GLToy_Maths::Sqrt( MagnitudeSquared() );
}