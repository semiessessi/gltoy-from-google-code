/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010, 2011 Semi Essessi
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
#include <Maths/GLToy_Matrix.h>

// GLToy
#include <Core/Data Structures/GLToy_BitStream.h>
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

template< class T, int D >
GLToy_Inline T MultiplyHelper( const T& xMatrix, const T& xThis )
{
    T xReturnValue;
    for( u_int u = 0; u < D; ++u )
    {
        for( u_int v = 0; v < D; ++v )
        {
            xReturnValue[ u ][ v ] = 0.0f;
            
            // M^a_b = M^a_c M^c_b
            for( u_int w = 0; w < D; ++w )
            {
                xReturnValue[ u ][ v ] += xThis[ u ][ w ] * xMatrix[ w ][ v ];
            }
        }
    }

    return xReturnValue;
}

//
// GLToy_Matrix_2
//

GLToy_Matrix_2::GLToy_Matrix_2()
{
    m_xComponents[ 0 ] = GLToy_Maths::ZeroVector2;
    m_xComponents[ 1 ] = GLToy_Maths::ZeroVector2;
}

GLToy_Matrix_2::GLToy_Matrix_2( const GLToy_Vector_2& xX, const GLToy_Vector_2& xY )
{
    m_xComponents[ 0 ] = xX;
    m_xComponents[ 1 ] = xY;
}

GLToy_Matrix_2::GLToy_Matrix_2( const float f00, const float f01, const float f10, const float f11 )
{
    m_xComponents[ 0 ] = GLToy_Vector_2( f00, f01 );
    m_xComponents[ 1 ] = GLToy_Vector_2( f10, f11 );
}

GLToy_Matrix_2::GLToy_Matrix_2( const GLToy_Matrix_2& xMatrix )
{
    m_xComponents[ 0 ] = xMatrix.m_xComponents[ 0 ];
    m_xComponents[ 1 ] = xMatrix.m_xComponents[ 1 ];
}

void GLToy_Matrix_2::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    xStream >> m_xComponents[ 0 ];
    xStream >> m_xComponents[ 1 ];
}

void GLToy_Matrix_2::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    xStream << m_xComponents[ 0 ];
    xStream << m_xComponents[ 1 ];
}

GLToy_Matrix_2& GLToy_Matrix_2::operator =( const GLToy_Matrix_2& xMatrix )
{
    m_xComponents[ 0 ] = xMatrix.m_xComponents[ 0 ];
    m_xComponents[ 1 ] = xMatrix.m_xComponents[ 1 ];

    return *this;
}

GLToy_Matrix_2 GLToy_Matrix_2::operator -() const
{
    return GLToy_Matrix_2( -m_xComponents[ 0 ], -m_xComponents[ 1 ] );
}

GLToy_Matrix_2 GLToy_Matrix_2::operator +( const GLToy_Matrix_2& xMatrix ) const
{
    return GLToy_Matrix_2(
        m_xComponents[ 0 ] + xMatrix.m_xComponents[ 0 ],
        m_xComponents[ 1 ] + xMatrix.m_xComponents[ 1 ] );
}

GLToy_Matrix_2 GLToy_Matrix_2::operator -( const GLToy_Matrix_2& xMatrix ) const
{
    return GLToy_Matrix_2(
        m_xComponents[ 0 ] - xMatrix.m_xComponents[ 0 ],
        m_xComponents[ 1 ] - xMatrix.m_xComponents[ 1 ] );
}

GLToy_Matrix_2 GLToy_Matrix_2::operator *( const GLToy_Matrix_2& xMatrix ) const
{
    return MultiplyHelper< GLToy_Matrix_2, 2 >( xMatrix, *this );
}

GLToy_Matrix_2 GLToy_Matrix_2::operator *( const float fValue ) const
{
    return GLToy_Matrix_2(
        m_xComponents[ 0 ] * fValue,
        m_xComponents[ 1 ] * fValue );
}

GLToy_Vector_2 GLToy_Matrix_2::operator *( const GLToy_Vector_2& xValue ) const
{
    return GLToy_Vector_2(
        m_xComponents[ 0 ] * xValue,
        m_xComponents[ 1 ] * xValue );
}

void GLToy_Matrix_2::Identity()
{
    *this = GLToy_Maths::IdentityMatrix2;
}

// TODO - this could be done without copies using xor swaps
void GLToy_Matrix_2::Transpose()
{
    GLToy_Vector_2 xX = GLToy_Vector_2( m_xComponents[ 0 ][ 0 ], m_xComponents[ 1 ][ 0 ] );
    GLToy_Vector_2 xY = GLToy_Vector_2( m_xComponents[ 0 ][ 1 ], m_xComponents[ 1 ][ 1 ] );

    m_xComponents[ 0 ] = xX;
    m_xComponents[ 1 ] = xY;
}

//
// GLToy_Matrix_3
//

GLToy_Matrix_3::GLToy_Matrix_3()
{
    m_xComponents[ 0 ] = GLToy_Maths::ZeroVector3;
    m_xComponents[ 1 ] = GLToy_Maths::ZeroVector3;
    m_xComponents[ 2 ] = GLToy_Maths::ZeroVector3;
}

GLToy_Matrix_3::GLToy_Matrix_3( const GLToy_Vector_3& xX, const GLToy_Vector_3& xY, const GLToy_Vector_3& xZ )
{
    m_xComponents[ 0 ] = xX;
    m_xComponents[ 1 ] = xY;
    m_xComponents[ 2 ] = xZ;
}

GLToy_Matrix_3::GLToy_Matrix_3( const float f00, const float f01, const float f02, const float f10, const float f11, const float f12, const float f20, const float f21, const float f22 )
{
    m_xComponents[ 0 ] = GLToy_Vector_3( f00, f01, f02 );
    m_xComponents[ 1 ] = GLToy_Vector_3( f10, f11, f12 );
    m_xComponents[ 2 ] = GLToy_Vector_3( f20, f21, f22 );
}

GLToy_Matrix_3::GLToy_Matrix_3( const GLToy_Matrix_3& xMatrix )
{
    m_xComponents[ 0 ] = xMatrix.m_xComponents[ 0 ];
    m_xComponents[ 1 ] = xMatrix.m_xComponents[ 1 ];
    m_xComponents[ 2 ] = xMatrix.m_xComponents[ 2 ];
}

void GLToy_Matrix_3::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    xStream >> m_xComponents[ 0 ];
    xStream >> m_xComponents[ 1 ];
    xStream >> m_xComponents[ 2 ];
}

void GLToy_Matrix_3::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    xStream << m_xComponents[ 0 ];
    xStream << m_xComponents[ 1 ];
    xStream << m_xComponents[ 2 ];
}

GLToy_Matrix_3& GLToy_Matrix_3::operator =( const GLToy_Matrix_3& xMatrix )
{
    m_xComponents[ 0 ] = xMatrix.m_xComponents[ 0 ];
    m_xComponents[ 1 ] = xMatrix.m_xComponents[ 1 ];
    m_xComponents[ 2 ] = xMatrix.m_xComponents[ 2 ];

    return *this;
}

GLToy_Matrix_3 GLToy_Matrix_3::operator -() const
{
    return GLToy_Matrix_3( -m_xComponents[ 0 ], -m_xComponents[ 1 ], -m_xComponents[ 2 ] );
}

GLToy_Matrix_3 GLToy_Matrix_3::operator +( const GLToy_Matrix_3& xMatrix ) const
{
    return GLToy_Matrix_3(
        m_xComponents[ 0 ] + xMatrix.m_xComponents[ 0 ],
        m_xComponents[ 1 ] + xMatrix.m_xComponents[ 1 ],
        m_xComponents[ 2 ] + xMatrix.m_xComponents[ 2 ] );
}

GLToy_Matrix_3 GLToy_Matrix_3::operator -( const GLToy_Matrix_3& xMatrix ) const
{
    return GLToy_Matrix_3(
        m_xComponents[ 0 ] - xMatrix.m_xComponents[ 0 ],
        m_xComponents[ 1 ] - xMatrix.m_xComponents[ 1 ],
        m_xComponents[ 2 ] - xMatrix.m_xComponents[ 2 ] );
}

GLToy_Matrix_3 GLToy_Matrix_3::operator *( const GLToy_Matrix_3& xMatrix ) const
{
    return MultiplyHelper< GLToy_Matrix_3, 3 >( xMatrix, *this );
}

GLToy_Matrix_3 GLToy_Matrix_3::operator *( const float fValue ) const
{
    return GLToy_Matrix_3(
        m_xComponents[ 0 ] * fValue,
        m_xComponents[ 1 ] * fValue,
        m_xComponents[ 2 ] * fValue );
}

GLToy_Vector_3 GLToy_Matrix_3::operator *( const GLToy_Vector_3& xValue ) const
{
    return GLToy_Vector_3(
        m_xComponents[ 0 ] * xValue,
        m_xComponents[ 1 ] * xValue,
        m_xComponents[ 2 ] * xValue );
}

void GLToy_Matrix_3::Identity()
{
    *this = GLToy_Maths::IdentityMatrix3;
}

// TODO - this could be done without copies using xor swaps
void GLToy_Matrix_3::Transpose()
{
    GLToy_Vector_3 xX = GLToy_Vector_3( m_xComponents[ 0 ][ 0 ], m_xComponents[ 1 ][ 0 ], m_xComponents[ 2 ][ 0 ] );
    GLToy_Vector_3 xY = GLToy_Vector_3( m_xComponents[ 0 ][ 1 ], m_xComponents[ 1 ][ 1 ], m_xComponents[ 2 ][ 1 ] );
    GLToy_Vector_3 xZ = GLToy_Vector_3( m_xComponents[ 0 ][ 2 ], m_xComponents[ 1 ][ 2 ], m_xComponents[ 2 ][ 2 ] );

    m_xComponents[ 0 ] = xX;
    m_xComponents[ 1 ] = xY;
    m_xComponents[ 2 ] = xZ;
}

void GLToy_Matrix_3::Orthonormalise()
{
    m_xComponents[ 0 ].Normalise();
    m_xComponents[ 1 ].Normalise();
    m_xComponents[ 2 ] = m_xComponents[ 0 ].Cross( m_xComponents[ 1 ] );
}

//
// GLToy_Matrix_4
//

GLToy_Matrix_4::GLToy_Matrix_4()
{
    m_xComponents[ 0 ] = GLToy_Maths::ZeroVector4;
    m_xComponents[ 1 ] = GLToy_Maths::ZeroVector4;
    m_xComponents[ 2 ] = GLToy_Maths::ZeroVector4;
    m_xComponents[ 3 ] = GLToy_Maths::ZeroVector4;
}

GLToy_Matrix_4::GLToy_Matrix_4( const GLToy_Vector_4& xX, const GLToy_Vector_4& xY, const GLToy_Vector_4& xZ, const GLToy_Vector_4& xW )
{
    m_xComponents[ 0 ] = xX;
    m_xComponents[ 1 ] = xY;
    m_xComponents[ 2 ] = xZ;
    m_xComponents[ 3 ] = xW;
}

GLToy_Matrix_4::GLToy_Matrix_4(
        const float f00, const float f01, const float f02, const float f03,
        const float f10, const float f11, const float f12, const float f13,
        const float f20, const float f21, const float f22, const float f23,
        const float f30, const float f31, const float f32, const float f33 )
{
    m_xComponents[ 0 ] = GLToy_Vector_4( f00, f01, f02, f03 );
    m_xComponents[ 1 ] = GLToy_Vector_4( f10, f11, f12, f13 );
    m_xComponents[ 2 ] = GLToy_Vector_4( f20, f21, f22, f23 );
    m_xComponents[ 3 ] = GLToy_Vector_4( f30, f31, f32, f33 );
}

GLToy_Matrix_4::GLToy_Matrix_4( const GLToy_Matrix_4& xMatrix )
{
    m_xComponents[ 0 ] = xMatrix.m_xComponents[ 0 ];
    m_xComponents[ 1 ] = xMatrix.m_xComponents[ 1 ];
    m_xComponents[ 2 ] = xMatrix.m_xComponents[ 2 ];
    m_xComponents[ 3 ] = xMatrix.m_xComponents[ 3 ];
}

GLToy_Matrix_4::GLToy_Matrix_4( const GLToy_Matrix_3& xMatrix )
{
    m_xComponents[ 0 ] = GLToy_Vector_4( xMatrix[ 0 ], 0.0f );
    m_xComponents[ 1 ] = GLToy_Vector_4( xMatrix[ 1 ], 0.0f );
    m_xComponents[ 2 ] = GLToy_Vector_4( xMatrix[ 2 ], 0.0f );
    m_xComponents[ 3 ] = GLToy_Vector_4( 0.0f, 0.0f, 0.0f, 1.0f );
}

void GLToy_Matrix_4::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    xStream >> m_xComponents[ 0 ];
    xStream >> m_xComponents[ 1 ];
    xStream >> m_xComponents[ 2 ];
    xStream >> m_xComponents[ 3 ];
}

void GLToy_Matrix_4::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    xStream << m_xComponents[ 0 ];
    xStream << m_xComponents[ 1 ];
    xStream << m_xComponents[ 2 ];
    xStream << m_xComponents[ 3 ];
}

GLToy_Matrix_4& GLToy_Matrix_4::operator =( const GLToy_Matrix_4& xMatrix )
{
    m_xComponents[ 0 ] = xMatrix.m_xComponents[ 0 ];
    m_xComponents[ 1 ] = xMatrix.m_xComponents[ 1 ];
    m_xComponents[ 2 ] = xMatrix.m_xComponents[ 2 ];
    m_xComponents[ 3 ] = xMatrix.m_xComponents[ 3 ];

    return *this;
}

GLToy_Matrix_4 GLToy_Matrix_4::operator -() const
{
    return GLToy_Matrix_4( -m_xComponents[ 0 ], -m_xComponents[ 1 ], -m_xComponents[ 2 ], -m_xComponents[ 3 ] );
}

GLToy_Matrix_4 GLToy_Matrix_4::operator +( const GLToy_Matrix_4& xMatrix ) const
{
    return GLToy_Matrix_4(
        m_xComponents[ 0 ] + xMatrix.m_xComponents[ 0 ],
        m_xComponents[ 1 ] + xMatrix.m_xComponents[ 1 ],
        m_xComponents[ 2 ] + xMatrix.m_xComponents[ 2 ],
        m_xComponents[ 3 ] + xMatrix.m_xComponents[ 3 ] );
}

GLToy_Matrix_4 GLToy_Matrix_4::operator -( const GLToy_Matrix_4& xMatrix ) const
{
    return GLToy_Matrix_4(
        m_xComponents[ 0 ] - xMatrix.m_xComponents[ 0 ],
        m_xComponents[ 1 ] - xMatrix.m_xComponents[ 1 ],
        m_xComponents[ 2 ] - xMatrix.m_xComponents[ 2 ],
        m_xComponents[ 3 ] - xMatrix.m_xComponents[ 3 ]);
}

GLToy_Matrix_4 GLToy_Matrix_4::operator *( const GLToy_Matrix_4& xMatrix ) const
{
    return MultiplyHelper< GLToy_Matrix_4, 4 >( xMatrix, *this );
}

GLToy_Matrix_4 GLToy_Matrix_4::operator *( const float fValue ) const
{
    return GLToy_Matrix_4(
        m_xComponents[ 0 ] * fValue,
        m_xComponents[ 1 ] * fValue,
        m_xComponents[ 2 ] * fValue,
        m_xComponents[ 3 ] * fValue );
}

GLToy_Vector_4 GLToy_Matrix_4::operator *( const GLToy_Vector_4& xValue ) const
{
    return GLToy_Vector_4(
        m_xComponents[ 0 ] * xValue,
        m_xComponents[ 1 ] * xValue,
        m_xComponents[ 2 ] * xValue,
        m_xComponents[ 3 ] * xValue );
}

void GLToy_Matrix_4::Identity()
{
    *this = GLToy_Maths::IdentityMatrix4;
}

// TODO - this could be done without copies using xor swaps
void GLToy_Matrix_4::Transpose()
{
    GLToy_Vector_4 xX = GLToy_Vector_4( m_xComponents[ 0 ][ 0 ], m_xComponents[ 1 ][ 0 ], m_xComponents[ 2 ][ 0 ], m_xComponents[ 3 ][ 0 ] );
    GLToy_Vector_4 xY = GLToy_Vector_4( m_xComponents[ 0 ][ 1 ], m_xComponents[ 1 ][ 1 ], m_xComponents[ 2 ][ 1 ], m_xComponents[ 3 ][ 1 ] );
    GLToy_Vector_4 xZ = GLToy_Vector_4( m_xComponents[ 0 ][ 2 ], m_xComponents[ 1 ][ 2 ], m_xComponents[ 2 ][ 2 ], m_xComponents[ 3 ][ 2 ] );
    GLToy_Vector_4 xW = GLToy_Vector_4( m_xComponents[ 0 ][ 3 ], m_xComponents[ 1 ][ 3 ], m_xComponents[ 2 ][ 3 ], m_xComponents[ 3 ][ 3 ] );

    m_xComponents[ 0 ] = xX;
    m_xComponents[ 1 ] = xY;
    m_xComponents[ 2 ] = xZ;
    m_xComponents[ 3 ] = xW;
}

void GLToy_Matrix_4::InvertTransformationMatrix()
{
    GLToy_Matrix_3 xTopLeft( m_xComponents[ 0 ], m_xComponents[ 1 ], m_xComponents[ 2 ] );

    xTopLeft.Transpose();

    m_xComponents[ 0 ] = xTopLeft[ 0 ];
    m_xComponents[ 1 ] = xTopLeft[ 1 ];
    m_xComponents[ 2 ] = xTopLeft[ 2 ];
    m_xComponents[ 3 ][ 0 ] = -m_xComponents[ 3 ][ 0 ];
    m_xComponents[ 3 ][ 1 ] = -m_xComponents[ 3 ][ 1 ];
    m_xComponents[ 3 ][ 2 ] = -m_xComponents[ 3 ][ 2 ];
}
