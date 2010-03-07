#ifndef __GLTOY_VECTOR_H_
#define __GLTOY_VECTOR_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Vector_2
{

public:

    GLToy_Vector_2();
    GLToy_Vector_2( float fX, float fY );
    GLToy_Vector_2( const GLToy_Vector_2& xVector );

    GLToy_Inline float& operator[] ( int i ) { return fComponents[i]; }
    GLToy_Inline const float& operator[] ( int i ) const { return fComponents[i]; }

    GLToy_Inline float* GetFloatPointer() { return fComponents; }
    GLToy_Inline const float* const GetConstFloatPointer() const { return fComponents; }

private:
    
    float fComponents[ 2 ];

};

class GLToy_Vector_3
{

public:

    GLToy_Vector_3();
    GLToy_Vector_3( float fX, float fY, float fZ );
    GLToy_Vector_3( const GLToy_Vector_3& xVector );

    GLToy_Vector_3& operator =( const GLToy_Vector_3& xVector );

    GLToy_Inline float& operator[] ( int i ) { return fComponents[i]; }
    GLToy_Inline const float& operator[] ( int i ) const { return fComponents[i]; }

    GLToy_Inline float* GetFloatPointer() { return fComponents; }
    GLToy_Inline const float* const GetFloatPointer() const { return fComponents; }

    GLToy_Vector_3 operator -() const;

    GLToy_Vector_3 operator +( const GLToy_Vector_3& xVector ) const;
    GLToy_Vector_3 operator -( const GLToy_Vector_3& xVector ) const;
    GLToy_Vector_3 operator *( const float fFloat ) const;

    GLToy_Inline float operator *( const GLToy_Vector_3& xVector ) const
    {
        return fComponents[ 0 ] * xVector[ 0 ]
            + fComponents[ 1 ] * xVector[ 1 ]
            + fComponents[ 2 ] * xVector[ 2 ];
    }

    void Normalise();

    bool operator ==( const GLToy_Vector_3& xVector );

private:
    
    float fComponents[ 3 ];

};

class GLToy_Vector_4
{

public:

    GLToy_Vector_4();
    GLToy_Vector_4( float fX, float fY, float fZ, float fW );
    GLToy_Vector_4( const GLToy_Vector_4& xVector );

    float& operator[] ( int i ) { return fComponents[i]; }
    const float& operator[] ( int i ) const { return fComponents[i]; }

    float* GetFloatPointer() { return fComponents; }
    const float* const GetConstFloatPointer() const { return fComponents; }

private:
    
    float fComponents[ 4 ];

};

#endif