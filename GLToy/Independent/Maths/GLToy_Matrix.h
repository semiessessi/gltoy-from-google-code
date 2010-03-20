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

private:
    
    GLToy_Vector_3 m_xComponents[ 3 ];

};

#endif