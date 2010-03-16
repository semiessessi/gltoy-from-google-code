/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// This file's headers
#include <Maths/GLToy_Matrix.h>

// GLToy
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Matrix_3::GLToy_Matrix_3( const GLToy_Vector_3& xX, const GLToy_Vector_3& xY, const GLToy_Vector_3& xZ )
{
    m_xComponents[ 0 ] = xX;
    m_xComponents[ 1 ] = xY;
    m_xComponents[ 2 ] = xZ;
}

GLToy_Matrix_3::GLToy_Matrix_3( const GLToy_Matrix_3& xMatrix )
{
    m_xComponents[ 0 ] = xMatrix.m_xComponents[ 0 ];
    m_xComponents[ 1 ] = xMatrix.m_xComponents[ 1 ];
    m_xComponents[ 2 ] = xMatrix.m_xComponents[ 2 ];
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