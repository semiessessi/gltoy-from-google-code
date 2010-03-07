#ifndef __GLTOY_MATRIX_H_
#define __GLTOY_MATRIX_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

#include <Maths/GLToy_Vector.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Matrix_3
{

public:

    GLToy_Matrix_3();
    GLToy_Matrix_3( const GLToy_Vector_3& xX, const GLToy_Vector_3& xY, const GLToy_Vector_3& xZ );
    GLToy_Matrix_3( const GLToy_Matrix_3& xMatrix );

    GLToy_Matrix_3& operator =( const GLToy_Matrix_3& xMatrix );

    GLToy_Inline GLToy_Vector_3& operator[] ( int i ) { return xComponents[ i ]; }
    GLToy_Inline const GLToy_Vector_3& operator[] ( int i ) const { return xComponents[ i ]; }

    GLToy_Inline GLToy_Vector_3* GetVectorPointer() { return xComponents; }
    GLToy_Inline const GLToy_Vector_3* const GetVectorPointer() const { return xComponents; }

    GLToy_Matrix_3 operator -() const;

private:
    
    GLToy_Vector_3 xComponents[ 3 ];

};

#endif