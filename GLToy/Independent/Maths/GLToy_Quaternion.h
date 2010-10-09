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

#ifndef __GLTOY_QUATERNION_H_
#define __GLTOY_QUATERNION_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Maths/GLToy_Matrix.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Quaternion
{

public:

    GLToy_Quaternion()
    {
        m_fComponents[ 0 ] = 0.0f;
        m_fComponents[ 1 ] = 0.0f;
        m_fComponents[ 2 ] = 0.0f;
        m_fComponents[ 3 ] = 0.0f;
    }

    GLToy_Quaternion( const float fW, const float fX, const float fY, const float fZ )
    {
        m_fComponents[ 0 ] = fW;
        m_fComponents[ 1 ] = fX;
        m_fComponents[ 2 ] = fY;
        m_fComponents[ 3 ] = fZ;
    }

	GLToy_Quaternion( const GLToy_Matrix_3& xOrientation );

    virtual ~GLToy_Quaternion() {}

	float& operator[] ( int i ) { return m_fComponents[ i ]; }
    const float& operator[] ( int i ) const { return m_fComponents[ i ]; }

	GLToy_Quaternion& operator =( const GLToy_Quaternion& xQuaternion );

	GLToy_Quaternion operator +( const GLToy_Quaternion& xQuaternion ) const;
    GLToy_Quaternion operator -( const GLToy_Quaternion& xQuaternion ) const;
    GLToy_Quaternion operator *( const float fFloat ) const;
	friend GLToy_Quaternion operator *( const float fFloat, const GLToy_Quaternion& xQuaternion ) { return xQuaternion * fFloat; }
    GLToy_Quaternion operator /( const float fFloat ) const { return operator *( 1.0f / fFloat ); }

	GLToy_Quaternion& operator *=( const float fFloat ) { return ( *this = *this * fFloat ); }

	GLToy_Quaternion Conjugate() const { return GLToy_Quaternion( m_fComponents[ 0 ], -m_fComponents[ 1 ], -m_fComponents[ 2 ], -m_fComponents[ 3 ] ); }
	float MagnitudeSquared() const { return m_fComponents[ 0 ] * m_fComponents[ 0 ] + m_fComponents[ 1 ] * m_fComponents[ 1 ] + m_fComponents[ 2 ] * m_fComponents[ 2 ] + m_fComponents[ 3 ] * m_fComponents[ 3 ]; }
	float Magnitude() const;

	void Normalise();

	GLToy_Matrix_3 GetOrientationMatrix();

protected:

    float m_fComponents[ 4 ];

};

#endif
