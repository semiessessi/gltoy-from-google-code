#ifndef __GLTOY_VECTOR_H_
#define __GLTOY_VECTOR_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_BitStream;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Vector_2
// : public GLToy_Serialisable - don't do this, its nicer if we can map this to float[ 3 ], which we can't if it needs a virtual function table pointer
{

public:

    GLToy_Vector_2();
    GLToy_Vector_2( float fX, float fY );
    GLToy_Vector_2( const GLToy_Vector_2& xVector );

    void ReadFromBitStream( const GLToy_BitStream& xStream );
    void WriteToBitStream( GLToy_BitStream& xStream ) const;

    GLToy_Vector_2& operator =( const GLToy_Vector_2& xVector );

    GLToy_Inline float& operator[] ( int i ) { return m_fComponents[ i ]; }
    GLToy_Inline const float& operator[] ( int i ) const { return m_fComponents[ i ]; }

    GLToy_Inline float* GetFloatPointer() { return m_fComponents; }
    GLToy_Inline const float* GetFloatPointer() const { return m_fComponents; }

    GLToy_Vector_2 operator *( const float fFloat ) const;
    GLToy_Vector_2 operator /( const float fFloat ) const;

    bool operator ==( const GLToy_Vector_2& xVector ) const;

private:
    
    float m_fComponents[ 2 ];

};

class GLToy_Vector_3
// : public GLToy_Serialisable - don't do this, its nicer if we can map this to float[ 3 ], which we can't if it needs a virtual function table pointer
{

    friend class GLToy_Matrix_3;

public:

    GLToy_Vector_3();
    GLToy_Vector_3( float fX, float fY, float fZ );
    GLToy_Vector_3( const GLToy_Vector_3& xVector );

    void ReadFromBitStream( const GLToy_BitStream& xStream );
    void WriteToBitStream( GLToy_BitStream& xStream ) const;

    GLToy_Vector_3& operator =( const GLToy_Vector_3& xVector );

    GLToy_Inline float& operator[] ( int i ) { return m_fComponents[ i ]; }
    GLToy_Inline const float& operator[] ( int i ) const { return m_fComponents[ i ]; }

    GLToy_Inline float* GetFloatPointer() { return m_fComponents; }
    GLToy_Inline const float* GetFloatPointer() const { return m_fComponents; }

    GLToy_Vector_3 operator -() const;

    GLToy_Vector_3& operator +=( const GLToy_Vector_3& xVector );
    GLToy_Vector_3& operator -=( const GLToy_Vector_3& xVector );
    GLToy_Vector_3& operator *=( const float fFloat );

    GLToy_Vector_3 operator +( const GLToy_Vector_3& xVector ) const;
    GLToy_Vector_3 operator -( const GLToy_Vector_3& xVector ) const;
    GLToy_Vector_3 operator *( const float fFloat ) const;
    GLToy_Vector_3 operator *( const GLToy_Matrix_3& xMatrix ) const;

    // TODO - platform version
    GLToy_Inline float operator *( const GLToy_Vector_3& xVector ) const
    {
        return m_fComponents[ 0 ] * xVector[ 0 ]
            + m_fComponents[ 1 ] * xVector[ 1 ]
            + m_fComponents[ 2 ] * xVector[ 2 ];
    }

    // TODO - platform version
    GLToy_Vector_3 Cross( const GLToy_Vector_3& xVector ) const
    {
        return GLToy_Vector_3(
            m_fComponents[ 1 ] * xVector[ 2 ] - m_fComponents[ 2 ] * xVector[ 1 ],
            m_fComponents[ 2 ] * xVector[ 0 ] - m_fComponents[ 0 ] * xVector[ 2 ],
            m_fComponents[ 0 ] * xVector[ 1 ] - m_fComponents[ 1 ] * xVector[ 0 ]
            );
    }

    void Normalise();
    float Magnitude() const;
    float MagnitudeSquared() const { return *this * *this; }

    bool operator ==( const GLToy_Vector_3& xVector ) const;

private:
    
    float m_fComponents[ 3 ];

};

class GLToy_Vector_4
// : public GLToy_Serialisable - don't do this, its nicer if we can map this to float[ 3 ], which we can't if it needs a virtual function table pointer
{

public:

    GLToy_Vector_4();
    GLToy_Vector_4( float fX, float fY, float fZ, float fW );
    GLToy_Vector_4( const GLToy_Vector_4& xVector );

    float& operator[] ( int i ) { return m_fComponents[ i ]; }
    const float& operator[] ( int i ) const { return m_fComponents[ i ]; }

    float* GetFloatPointer() { return m_fComponents; }
    const float* GetFloatPointer() const { return m_fComponents; }

    bool operator ==( const GLToy_Vector_4& xVector ) const;

private:
    
    float m_fComponents[ 4 ];

};

#endif