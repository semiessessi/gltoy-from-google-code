/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// This file's headers
#include <Maths/GLToy_Matrix.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Matrix_3::GLToy_Matrix_3( const GLToy_Vector_3& xX, const GLToy_Vector_3& xY, const GLToy_Vector_3& xZ )
{
    xComponents[ 0 ] = xX;
    xComponents[ 1 ] = xY;
    xComponents[ 2 ] = xZ;
}

GLToy_Matrix_3::GLToy_Matrix_3( const GLToy_Matrix_3& xMatrix )
{
    xComponents[ 0 ] = xMatrix.xComponents[ 0 ];
    xComponents[ 1 ] = xMatrix.xComponents[ 1 ];
    xComponents[ 2 ] = xMatrix.xComponents[ 2 ];
}

GLToy_Matrix_3& GLToy_Matrix_3::operator =( const GLToy_Matrix_3& xMatrix )
{
    xComponents[ 0 ] = xMatrix.xComponents[ 0 ];
    xComponents[ 1 ] = xMatrix.xComponents[ 1 ];
    xComponents[ 2 ] = xMatrix.xComponents[ 2 ];

    return *this;
}

GLToy_Matrix_3 GLToy_Matrix_3::operator -() const
{
    return GLToy_Matrix_3( -xComponents[ 0 ], -xComponents[ 1 ], -xComponents[ 2 ] );
}