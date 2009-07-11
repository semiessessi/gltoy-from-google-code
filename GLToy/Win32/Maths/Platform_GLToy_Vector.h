#ifndef __GLTOY_VECTOR_H_
#define __GLTOY_VECTOR_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Vector_2
{

public:

    GLToy_Vector_2();
    GLToy_Vector_2( float fX, float fY );
    GLToy_Vector_2( const GLToy_Vector_2& xVector );

    float& operator[] ( int i ) { return fComponents[i]; }
    const float& operator[] ( int i ) const { return fComponents[i]; }

private:
    
    float fComponents[2];

};

class GLToy_Vector_3
{

public:

    GLToy_Vector_3();
    GLToy_Vector_3( float fX, float fY, float fZ );
    GLToy_Vector_3( const GLToy_Vector_3& xVector );

    float& operator[] ( int i ) { return fComponents[i]; }
    const float& operator[] ( int i ) const { return fComponents[i]; }

private:
    
    float fComponents[3];

};

class GLToy_Vector_4
{

public:

    GLToy_Vector_4();
    GLToy_Vector_4( float fX, float fY, float fZ, float fW );
    GLToy_Vector_4( const GLToy_Vector_4& xVector );

    float& operator[] ( int i ) { return fComponents[i]; }
    const float& operator[] ( int i ) const { return fComponents[i]; }

private:
    
    float fComponents[4];

};

#endif