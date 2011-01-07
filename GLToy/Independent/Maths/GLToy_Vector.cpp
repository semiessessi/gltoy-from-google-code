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

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's headers
#include <Maths/GLToy_Vector.h>
#include <Maths/Platform_GLToy_Vector.h>

// GLToy
#include <Core/Data Structures/GLToy_BitStream.h>
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// 2 D
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Vector_2::GLToy_Vector_2()
{
    m_fComponents[ 0 ] = m_fComponents[ 1 ] = 0.0f;
}

GLToy_Vector_2::GLToy_Vector_2( float fX, float fY )
{
    m_fComponents[ 0 ] = fX;
    m_fComponents[ 1 ] = fY;
}

GLToy_Vector_2::GLToy_Vector_2( const GLToy_Vector_2& xVector )
{
    m_fComponents[ 0 ] = xVector[ 0 ];
    m_fComponents[ 1 ] = xVector[ 1 ];
}

GLToy_Vector_2::GLToy_Vector_2( const GLToy_Vector_3& xVector )
{
    m_fComponents[ 0 ] = xVector[ 0 ];
    m_fComponents[ 1 ] = xVector[ 1 ];
}

GLToy_Vector_2::GLToy_Vector_2( const GLToy_Vector_4& xVector )
{
    m_fComponents[ 0 ] = xVector[ 0 ];
    m_fComponents[ 1 ] = xVector[ 1 ];
}

GLToy_Vector_2& GLToy_Vector_2::operator =( const GLToy_Vector_2& xVector )
{
    // TODO - use platform copy
    m_fComponents[ 0 ] = xVector[ 0 ];
    m_fComponents[ 1 ] = xVector[ 1 ];

    return *this;
}


void GLToy_Vector_2::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    xStream >> m_fComponents[ 0 ];
    xStream >> m_fComponents[ 1 ];
}

void GLToy_Vector_2::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    xStream << m_fComponents[ 0 ];
    xStream << m_fComponents[ 1 ];
}


GLToy_Vector_2 GLToy_Vector_2::operator *( const float fFloat ) const
{
    return GLToy_Vector_2( m_fComponents[ 0 ] * fFloat, m_fComponents[ 1 ] * fFloat );
}

GLToy_Vector_2 GLToy_Vector_2::operator /( const float fFloat ) const
{
    const float fInverse = 1 / fFloat;
    return GLToy_Vector_2( m_fComponents[ 0 ] * fInverse, m_fComponents[ 1 ] * fInverse );
}

void GLToy_Vector_2::Normalise()
{
    const float fMagnitudeSquared = MagnitudeSquared();
    GLToy_Assert( fMagnitudeSquared != 0.0f, "Trying to normalise a zero vector!" );
    *this = *this * ( 1.0f / GLToy_Maths::Sqrt( fMagnitudeSquared ) );
}

float GLToy_Vector_2::Magnitude() const
{
    return GLToy_Maths::Sqrt( MagnitudeSquared() );
}

bool GLToy_Vector_2::operator ==( const GLToy_Vector_2& xVector ) const
{
    // TODO - use platform compare
    return ( m_fComponents[ 0 ] == xVector[ 0 ] )
        && ( m_fComponents[ 1 ] == xVector[ 1 ] );
}

/////////////////////////////////////////////////////////////////////////////////////////////
// 3 D
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Vector_3::GLToy_Vector_3()
{
    // TODO - use platform zero
    m_fComponents[ 0 ]
        = m_fComponents[ 1 ]
        = m_fComponents[ 2 ]
        = 0.0f;
}

GLToy_Vector_3::GLToy_Vector_3( float fX, float fY, float fZ )
{
    m_fComponents[ 0 ] = fX;
    m_fComponents[ 1 ] = fY;
    m_fComponents[ 2 ] = fZ;
}

GLToy_Vector_3::GLToy_Vector_3( const GLToy_Vector_3& xVector )
{
    // TODO - use platform copy
    m_fComponents[ 0 ] = xVector[ 0 ];
    m_fComponents[ 1 ] = xVector[ 1 ];
    m_fComponents[ 2 ] = xVector[ 2 ];
}

GLToy_Vector_3::GLToy_Vector_3( const GLToy_Vector_4& xVector )
{
    // TODO - use platform copy
    m_fComponents[ 0 ] = xVector[ 0 ];
    m_fComponents[ 1 ] = xVector[ 1 ];
    m_fComponents[ 2 ] = xVector[ 2 ];
}

GLToy_Vector_3::GLToy_Vector_3( const GLToy_Vector_2& xVector, const float fZ )
{
    m_fComponents[ 0 ] = xVector[ 0 ];
    m_fComponents[ 1 ] = xVector[ 1 ];
    m_fComponents[ 2 ] = fZ;
}

void GLToy_Vector_3::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    xStream >> m_fComponents[ 0 ];
    xStream >> m_fComponents[ 1 ];
    xStream >> m_fComponents[ 2 ];
}

void GLToy_Vector_3::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    xStream << m_fComponents[ 0 ];
    xStream << m_fComponents[ 1 ];
    xStream << m_fComponents[ 2 ];
}

GLToy_Vector_3& GLToy_Vector_3::operator =( const GLToy_Vector_3& xVector )
{
    // TODO - use platform copy
    m_fComponents[ 0 ] = xVector[ 0 ];
    m_fComponents[ 1 ] = xVector[ 1 ];
    m_fComponents[ 2 ] = xVector[ 2 ];

    return *this;
}

GLToy_Vector_3 GLToy_Vector_3::operator -() const
{
    // TODO - use platform negate
    return GLToy_Vector_3( -m_fComponents[ 0 ], -m_fComponents[ 1 ], -m_fComponents[ 2 ] );
}

GLToy_Vector_3& GLToy_Vector_3::operator +=( const GLToy_Vector_3& xVector )
{
    // TODO - use platform add
    m_fComponents[ 0 ] += xVector.m_fComponents[ 0 ];
    m_fComponents[ 1 ] += xVector.m_fComponents[ 1 ];
    m_fComponents[ 2 ] += xVector.m_fComponents[ 2 ];
    return *this;
}

GLToy_Vector_3& GLToy_Vector_3::operator -=( const GLToy_Vector_3& xVector )
{
    // TODO - use platform sub
    m_fComponents[ 0 ] -= xVector.m_fComponents[ 0 ];
    m_fComponents[ 1 ] -= xVector.m_fComponents[ 1 ];
    m_fComponents[ 2 ] -= xVector.m_fComponents[ 2 ];
    return *this;
}

GLToy_Vector_3& GLToy_Vector_3::operator *=( const float fFloat )
{
    // TODO - use platform mul
    m_fComponents[ 0 ] *= fFloat;
    m_fComponents[ 1 ] *= fFloat;
    m_fComponents[ 2 ] *= fFloat;
    return *this;
}

GLToy_Vector_3& GLToy_Vector_3::operator *=( const GLToy_Matrix_3& xMatrix )
{
    return ( *this = ( *this * xMatrix ) );
}

GLToy_Vector_3 GLToy_Vector_3::operator +( const GLToy_Vector_3& xVector ) const
{
    return Platform_GLToy_Vector::Add( *this, xVector );
}

GLToy_Vector_3 GLToy_Vector_3::operator -( const GLToy_Vector_3& xVector ) const
{
    return Platform_GLToy_Vector::Sub( *this, xVector );
}

GLToy_Vector_3 GLToy_Vector_3::operator *( const float fFloat ) const
{
    return Platform_GLToy_Vector::Mul( fFloat, *this );
}

bool GLToy_Vector_3::operator ==( const GLToy_Vector_3& xVector ) const
{
    // TODO - use platform compare
    return ( m_fComponents[ 0 ] == xVector[ 0 ] )
        && ( m_fComponents[ 1 ] == xVector[ 1 ] )
        && ( m_fComponents[ 2 ] == xVector[ 2 ] );
}

GLToy_Vector_3 GLToy_Vector_3::operator *( const GLToy_Matrix_3& xMatrix ) const
{
    GLToy_Vector_3 xReturnValue;
    for( u_int u = 0; u < 3; ++u )
    {
        xReturnValue[ u ] = 0.0f;
        
        // V_a = V^c M^a_c
        for( u_int w = 0; w < 3; ++w )
        {
            xReturnValue[ u ] += m_fComponents[ w ] * xMatrix.m_xComponents[ u ][ w ];
        }
    }

    return xReturnValue;
}

void GLToy_Vector_3::Normalise()
{
    const float fMagnitudeSquared = MagnitudeSquared();
    GLToy_Assert( fMagnitudeSquared != 0.0f, "Trying to normalise a zero vector!" );
    *this = *this * ( 1.0f / GLToy_Maths::Sqrt( fMagnitudeSquared ) );
}

float GLToy_Vector_3::Magnitude() const
{
    return GLToy_Maths::Sqrt( MagnitudeSquared() );
}

/////////////////////////////////////////////////////////////////////////////////////////////
// 4 D
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Vector_4::GLToy_Vector_4()
{
    m_fComponents[ 0 ]
        = m_fComponents[ 1 ]
        = m_fComponents[ 2 ]
        = m_fComponents[ 3 ]
        = 0.0f;
}

GLToy_Vector_4::GLToy_Vector_4( const GLToy_Vector_2& xXY, const GLToy_Vector_2& xZW )
{
    m_fComponents[ 0 ] = xXY[ 0 ];
    m_fComponents[ 1 ] = xXY[ 1 ];
    m_fComponents[ 2 ] = xZW[ 0 ];
    m_fComponents[ 3 ] = xZW[ 1 ];
}

GLToy_Vector_4::GLToy_Vector_4( const GLToy_Vector_2& xXY, const float fX, const float fW )
{
    m_fComponents[ 0 ] = xXY[ 0 ];
    m_fComponents[ 1 ] = xXY[ 1 ];
    m_fComponents[ 2 ] = fX;
    m_fComponents[ 3 ] = fW;
}

GLToy_Vector_4::GLToy_Vector_4( const GLToy_Vector_3& xVector, const float fW )
{
    m_fComponents[ 0 ] = xVector[ 0 ];
    m_fComponents[ 1 ] = xVector[ 1 ];
    m_fComponents[ 2 ] = xVector[ 2 ];
    m_fComponents[ 3 ] = fW;
}

GLToy_Vector_4::GLToy_Vector_4( float fX, float fY, float fZ, float fW )
{
    m_fComponents[ 0 ] = fX;
    m_fComponents[ 1 ] = fY;
    m_fComponents[ 2 ] = fZ;
    m_fComponents[ 3 ] = fW;
}

GLToy_Vector_4::GLToy_Vector_4( const u_int uRGBA )
{
    m_fComponents[ 0 ] = static_cast< float >( uRGBA & 0xFF ) / 255.0f;
    m_fComponents[ 1 ] = static_cast< float >( ( uRGBA & 0xFF00 ) >> 8 ) / 255.0f;
    m_fComponents[ 2 ] = static_cast< float >( ( uRGBA & 0xFF0000 ) >> 16 ) / 255.0f;
    m_fComponents[ 3 ] = static_cast< float >( uRGBA >> 24 ) / 255.0f;
}

GLToy_Vector_4::GLToy_Vector_4( const GLToy_Vector_4& xVector )
{
    m_fComponents[ 0 ] = xVector[ 0 ];
    m_fComponents[ 1 ] = xVector[ 1 ];
    m_fComponents[ 2 ] = xVector[ 2 ];
    m_fComponents[ 3 ] = xVector[ 3 ];
}

void GLToy_Vector_4::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    xStream >> m_fComponents[ 0 ];
    xStream >> m_fComponents[ 1 ];
    xStream >> m_fComponents[ 2 ];
    xStream >> m_fComponents[ 3 ];
}

void GLToy_Vector_4::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    xStream << m_fComponents[ 0 ];
    xStream << m_fComponents[ 1 ];
    xStream << m_fComponents[ 2 ];
    xStream << m_fComponents[ 3 ];
}

bool GLToy_Vector_4::operator ==( const GLToy_Vector_4& xVector ) const
{
    // TODO - use platform compare
    return ( m_fComponents[ 0 ] == xVector[ 0 ] )
        && ( m_fComponents[ 1 ] == xVector[ 1 ] )
        && ( m_fComponents[ 2 ] == xVector[ 2 ] )
        && ( m_fComponents[ 3 ] == xVector[ 3 ] );
}

u_int GLToy_Vector_4::GetRGBA() const
{
    return static_cast< u_int >( GLToy_Maths::Clamp( m_fComponents[ 0 ] * 255.0f, 0.0f, 255.0f ) )
        | ( static_cast< u_int >( GLToy_Maths::Clamp( m_fComponents[ 1 ] * 255.0f, 0.0f, 255.0f ) ) << 8 )
        | ( static_cast< u_int >( GLToy_Maths::Clamp( m_fComponents[ 2 ] * 255.0f, 0.0f, 255.0f ) ) << 16 )
        | ( static_cast< u_int >( GLToy_Maths::Clamp( m_fComponents[ 3 ] * 255.0f, 0.0f, 255.0f ) ) << 24 );
}
