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

struct GLToy_5Bytes
{
    u_int m_u4Bytes;
    u_char m_uByte;
};

class GLToy_Compress
{

public:

    static u_int Float_3Bytes( const float fFloat );
    static u_int Float_FixedBits( const float fFloat, const u_int uNumBits = 16, const float fMin = 0.0f, const float fMax = 1.0f );
    static GLToy_5Bytes Vector_5Bytes( const GLToy_Vector_3& xVector );
    static u_char UnitVector_Byte( const GLToy_Vector_3& xVector );
    static u_short UnitVector_2Bytes( const GLToy_Vector_3& xVector );
    static u_short OrthonormalMatrix_2Bytes( const GLToy_Matrix_3& xMatrix );
    static u_int OrthonormalMatrix_4Bytes( const GLToy_Matrix_3& xMatrix );
    static u_int RGBA_28Bits( const u_int uRGBA );
    static u_int RGBA_24Bits( const u_int uRGBA );

};

class GLToy_Decompress
{

public:

    static float Float_3Bytes( const u_int uInt );
    static float Float_FixedBits( const u_int uInt, const u_int uNumBits = 16, const float fMin = 0.0f, const float fMax = 1.0f );
    static GLToy_Vector_3 Vector_5Bytes( const GLToy_5Bytes& x5Bytes );
    static GLToy_Vector_3 UnitVector_Byte( const u_char ucChar );
    static GLToy_Vector_3 UnitVector_2Bytes( const u_short usShort );
    static GLToy_Matrix_3 OrthonormalMatrix_2Bytes( const u_short usShort );
    static GLToy_Matrix_3 OrthonormalMatrix_4Bytes( const u_int uInt );
    static u_int RGBA_28Bits( const u_int uCompressed );
    static u_int RGBA_24Bits( const u_int uCompressed );

};

#endif
