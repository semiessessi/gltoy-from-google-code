/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2009-2011 Semi Essessi
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

// SE - TODO: remove the initialisation in default constructors for more speed - sadly I'm
// pretty sure I remember relying on the behaviour of them initialising to zero in places...

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

float GLToy_Vector_Sqrt_Helper( const float f )
{
    return GLToy_Maths::Sqrt( f );
}

float GLToy_Vector_InvSqrt_Helper( const float f )
{
    return GLToy_Maths::InvSqrt( f );
}

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

GLToy_Vector_2::GLToy_Vector_2( const GLToy_Vector_3& xVector )
{
    GLToy_Memory::FixedCopy< sizeof( GLToy_Vector_2 ) >( this, &xVector );
}

GLToy_Vector_2::GLToy_Vector_2( const GLToy_Vector_4& xVector )
{
    GLToy_Memory::FixedCopy< sizeof( GLToy_Vector_2 ) >( this, &xVector );
}

GLToy_Vector_2 GLToy_Vector_2::operator *( const float fFloat ) const
{
    return GLToy_Vector_2( m_fComponents[ 0 ] * fFloat, m_fComponents[ 1 ] * fFloat );
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

GLToy_Vector_3::GLToy_Vector_3( const GLToy_Vector_4& xVector )
{
    GLToy_Memory::FixedCopy< sizeof( GLToy_Vector_3 ) >( this, &xVector );
}

GLToy_Vector_3::GLToy_Vector_3( const GLToy_Vector_2& xVector, const float fZ )
{
    GLToy_Memory::FixedCopy< sizeof( GLToy_Vector_2 ) >( this, &xVector );
    m_fComponents[ 2 ] = fZ;
}

GLToy_Vector_3 GLToy_Vector_3::operator -() const
{
    // TODO - use platform negate
    return GLToy_Vector_3( -m_fComponents[ 0 ], -m_fComponents[ 1 ], -m_fComponents[ 2 ] );
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
    GLToy_Memory::FixedCopy< sizeof( GLToy_Vector_2 ) >( this, &xXY );
    GLToy_Memory::FixedCopy< sizeof( GLToy_Vector_2 ) >( &( m_fComponents[ 2 ] ), &xZW );
}

GLToy_Vector_4::GLToy_Vector_4( const GLToy_Vector_2& xXY, const float fX, const float fW )
{
    GLToy_Memory::FixedCopy< sizeof( GLToy_Vector_2 ) >( this, &xXY );
    m_fComponents[ 2 ] = fX;
    m_fComponents[ 3 ] = fW;
}

GLToy_Vector_4::GLToy_Vector_4( const GLToy_Vector_3& xVector, const float fW )
{
    GLToy_Memory::FixedCopy< sizeof( GLToy_Vector_3 ) >( this, &xVector );
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

u_int GLToy_Vector_4::GetRGBA() const
{
    return static_cast< u_int >( GLToy_Maths::Clamp( m_fComponents[ 0 ] * 255.0f, 0.0f, 255.0f ) )
        | ( static_cast< u_int >( GLToy_Maths::Clamp( m_fComponents[ 1 ] * 255.0f, 0.0f, 255.0f ) ) << 8 )
        | ( static_cast< u_int >( GLToy_Maths::Clamp( m_fComponents[ 2 ] * 255.0f, 0.0f, 255.0f ) ) << 16 )
        | ( static_cast< u_int >( GLToy_Maths::Clamp( m_fComponents[ 3 ] * 255.0f, 0.0f, 255.0f ) ) << 24 );
}
