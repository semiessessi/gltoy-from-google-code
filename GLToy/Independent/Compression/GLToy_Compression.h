#ifndef __GLTOY_COMPRESSION_H_
#define __GLTOY_COMPRESSION_H_

class GLToy_Vector_3;

class GLToy_Compress
{

public:

    static u_short UnitVector_2Bytes( const GLToy_Vector_3& xVector );
    static u_int UnitVector_4Bytes( const GLToy_Vector_3& xVector );
};

#endif