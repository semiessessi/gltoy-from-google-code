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

// This file's headers
#include <Maths/GLToy_Matrix.h>

// GLToy
#include <Core/Data Structures/GLToy_BitStream.h>
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

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
    GLToy_Matrix_3 xReturnValue;
    for( u_int u = 0; u < 3; ++u )
    {
        for( u_int v = 0; v < 3; ++v )
        {
            xReturnValue[ u ][ v ] = 0.0f;
            
            // M^a_b = M^a_c M^c_b
            for( u_int w = 0; w < 3; ++w )
            {
                xReturnValue[ u ][ v ] += m_xComponents[ u ][ w ] * xMatrix.m_xComponents[ w ][ v ];
            }
        }
    }

    return xReturnValue;
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
