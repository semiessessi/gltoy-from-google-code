/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of GLToy.
//
// GLToy is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// GLToy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with GLToy.
// If not, see <http://www.gnu.org/licenses/>.
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
    static GLToy_5Bytes Vector_5Bytes( const GLToy_Vector_3& xVector );
    static u_char UnitVector_Byte( const GLToy_Vector_3& xVector );
    static u_short UnitVector_2Bytes( const GLToy_Vector_3& xVector );
    static u_short OrthonormalMatrix_2Bytes( const GLToy_Matrix_3& xMatrix );
    static u_int OrthonormalMatrix_4Bytes( const GLToy_Matrix_3& xMatrix );

};

class GLToy_Decompress
{

public:

    static float Float_3Bytes( const u_int uInt );
    static GLToy_Vector_3 Vector_5Bytes( const GLToy_5Bytes& x5Bytes );
    static GLToy_Vector_3 UnitVector_Byte( const u_char ucChar );
    static GLToy_Vector_3 UnitVector_2Bytes( const u_short usShort );
    static GLToy_Matrix_3 OrthonormalMatrix_2Bytes( const u_short usShort );
    static GLToy_Matrix_3 OrthonormalMatrix_4Bytes( const u_int uInt );

};

#endif