#ifndef __GLTOY_COMPRESSION_H_
#define __GLTOY_COMPRESSION_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Compress
{

public:

    static u_char UnitVector_Byte( const GLToy_Vector_3& xVector );
    static u_short UnitVector_2Bytes( const GLToy_Vector_3& xVector );
    static u_short OrthonormalMatrix_2Bytes( const GLToy_Matrix_3& xMatrix );
    static u_int OrthonormalMatrix_4Bytes( const GLToy_Matrix_3& xMatrix );

};

class GLToy_Decompress
{

public:

    static GLToy_Vector_3 UnitVector_Byte( const u_char ucChar );
    static GLToy_Vector_3 UnitVector_2Bytes( const u_short usShort );
    static GLToy_Matrix_3 OrthonormalMatrix_2Bytes( const u_short usShort );
    static GLToy_Matrix_3 OrthonormalMatrix_4Bytes( const u_int uInt );

};

#endif