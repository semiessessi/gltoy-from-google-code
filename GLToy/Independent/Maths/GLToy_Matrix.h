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

#ifndef __GLTOY_MATRIX_H_
#define __GLTOY_MATRIX_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/GLToy_Serialisable.h>

// GLToy
#include <Maths/GLToy_Vector.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Matrix_3
: public GLToy_Serialisable
{
    
    friend class GLToy_Vector_3;

public:

    GLToy_Matrix_3();
    GLToy_Matrix_3( const GLToy_Vector_3& xX, const GLToy_Vector_3& xY, const GLToy_Vector_3& xZ );
    GLToy_Matrix_3( const float f00, const float f01, const float f02, const float f10, const float f11, const float f12, const float f20, const float f21, const float f22 );
    GLToy_Matrix_3( const GLToy_Matrix_3& xMatrix );

    virtual ~GLToy_Matrix_3() {}

    virtual void ReadFromBitStream( const GLToy_BitStream& xStream );
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const;

    GLToy_Matrix_3& operator =( const GLToy_Matrix_3& xMatrix );

    GLToy_Inline GLToy_Vector_3& operator[] ( int i ) { return m_xComponents[ i ]; }
    GLToy_Inline const GLToy_Vector_3& operator[] ( int i ) const { return m_xComponents[ i ]; }

    GLToy_Inline GLToy_Vector_3* GetVectorPointer() { return m_xComponents; }
    GLToy_Inline const GLToy_Vector_3* const GetVectorPointer() const { return m_xComponents; }

    GLToy_Matrix_3 operator -() const;

    GLToy_Matrix_3 operator +( const GLToy_Matrix_3& xMatrix ) const;
    GLToy_Matrix_3 operator -( const GLToy_Matrix_3& xMatrix ) const;
    GLToy_Matrix_3 operator *( const GLToy_Matrix_3& xMatrix ) const;
    GLToy_Vector_3 operator *( const GLToy_Vector_3& xVector ) const;
    GLToy_Matrix_3 operator *( const float fValue ) const;

    void Identity();
    void Transpose();
    void Orthonormalise();

private:
    
    GLToy_Vector_3 m_xComponents[ 3 ];

};

#endif
