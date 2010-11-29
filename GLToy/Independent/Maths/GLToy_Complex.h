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

#ifndef __GLTOY_COMPLEX_H_
#define __GLTOY_COMPLEX_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Maths/GLToy_Matrix.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Complex
{

public:

    GLToy_Complex()
    {
        m_fComponents[ 0 ] = 0.0f;
        m_fComponents[ 1 ] = 0.0f;
    }

    GLToy_Complex( const float fRe, const float fIm = 0.0f )
    {
        m_fComponents[ 0 ] = fRe;
        m_fComponents[ 1 ] = fIm;
    }

	float& operator[] ( int i ) { return m_fComponents[ i ]; }
    const float& operator[] ( int i ) const { return m_fComponents[ i ]; }

	GLToy_Complex& operator =( const GLToy_Complex& xComplex );

	GLToy_Complex operator +( const GLToy_Complex& xComplex ) const;
    GLToy_Complex operator -( const GLToy_Complex& xComplex ) const;
    GLToy_Complex operator *( const float fFloat ) const;
	friend GLToy_Complex operator *( const float fFloat, const GLToy_Complex& xComplex ) { return xComplex * fFloat; }
    GLToy_Complex operator /( const float fFloat ) const { return operator *( 1.0f / fFloat ); }

	GLToy_Complex& operator *=( const float fFloat ) { return ( *this = *this * fFloat ); }

	GLToy_Complex Conjugate() const { return GLToy_Complex( m_fComponents[ 0 ], -m_fComponents[ 1 ] ); }
	float MagnitudeSquared() const { return m_fComponents[ 0 ] * m_fComponents[ 0 ] + m_fComponents[ 1 ] * m_fComponents[ 1 ]; }
	float Magnitude() const;

	void Normalise();

protected:

    float m_fComponents[ 2 ];

};

#endif
