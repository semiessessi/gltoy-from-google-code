/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009, 2010 Semi Essessi
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

#ifndef __GLTOY_VECTOR_H_
#define __GLTOY_VECTOR_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_BitStream;
class GLToy_Matrix_3;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Vector_2
// TODO: Investigate the possibility of moving GLToy_Serialisable over to a templated type, like GLToy_Destroyable
// : public GLToy_Serialisable - don't do this, its nicer if we can map this to float[ 2 ], which we can't if it needs a virtual function table pointer
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

    GLToy_Inline GLToy_Vector_2& operator *=( const float fFloat ) { return *this = ( *this * fFloat ); }
    GLToy_Inline GLToy_Vector_2& operator /=( const float fFloat ) { return operator *=( 1.0f / fFloat ); }

    GLToy_Inline GLToy_Vector_2 operator +( const GLToy_Vector_2& xVector ) const { return GLToy_Vector_2( m_fComponents[ 0 ] + xVector[ 0 ], m_fComponents[ 1 ] + xVector[ 1 ] ); }
    GLToy_Inline GLToy_Vector_2 operator -( const GLToy_Vector_2& xVector ) const { return GLToy_Vector_2( m_fComponents[ 0 ] - xVector[ 0 ], m_fComponents[ 1 ] - xVector[ 1 ] ); }
    GLToy_Vector_2 operator *( const float fFloat ) const;
	friend GLToy_Vector_2 operator *( const float fFloat, const GLToy_Vector_2& xVector ) { return xVector * fFloat; }
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
    GLToy_Vector_3& operator /=( const float fFloat ) { return operator *=( 1.0f / fFloat ); }

    GLToy_Vector_3 operator +( const GLToy_Vector_3& xVector ) const;
    GLToy_Vector_3 operator -( const GLToy_Vector_3& xVector ) const;
    GLToy_Vector_3 operator *( const float fFloat ) const;
	friend GLToy_Vector_3 operator *( const float fFloat, const GLToy_Vector_3& xVector ) { return xVector * fFloat; }
    GLToy_Vector_3 operator *( const GLToy_Matrix_3& xMatrix ) const;
    GLToy_Vector_3 operator /( const float fFloat ) const { return operator *( 1.0f / fFloat ); }

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
// : public GLToy_Serialisable - don't do this, its nicer if we can map this to float[ 4 ], which we can't if it needs a virtual function table pointer
{

public:

    GLToy_Vector_4();
    GLToy_Vector_4( const GLToy_Vector_3& xVector, const float fW = 1.0f );
    GLToy_Vector_4( float fX, float fY, float fZ, float fW );
    GLToy_Vector_4( const u_int uRGBA );
    GLToy_Vector_4( const GLToy_Vector_4& xVector );

    float& operator[] ( int i ) { return m_fComponents[ i ]; }
    const float& operator[] ( int i ) const { return m_fComponents[ i ]; }

    float* GetFloatPointer() { return m_fComponents; }
    const float* GetFloatPointer() const { return m_fComponents; }

    bool operator ==( const GLToy_Vector_4& xVector ) const;

    GLToy_Inline GLToy_Vector_4& operator *=( const float fFloat ) { return *this = ( *this * fFloat ); }
    GLToy_Inline GLToy_Vector_4& operator /=( const float fFloat ) { return operator *=( 1.0f / fFloat ); }

    GLToy_Inline GLToy_Vector_4 operator +( const GLToy_Vector_4& xVector ) const { return GLToy_Vector_4( m_fComponents[ 0 ] + xVector[ 0 ], m_fComponents[ 1 ] + xVector[ 1 ], m_fComponents[ 2 ] + xVector[ 2 ], m_fComponents[ 3 ] + xVector[ 3 ] ); }
    GLToy_Inline GLToy_Vector_4 operator -( const GLToy_Vector_4& xVector ) const { return GLToy_Vector_4( m_fComponents[ 0 ] - xVector[ 0 ], m_fComponents[ 1 ] - xVector[ 1 ], m_fComponents[ 2 ] - xVector[ 2 ], m_fComponents[ 3 ] - xVector[ 3 ] ); }
    GLToy_Inline GLToy_Vector_4 operator *( const float fFloat ) const { return GLToy_Vector_4( m_fComponents[ 0 ] * fFloat, m_fComponents[ 1 ] * fFloat, m_fComponents[ 2 ] * fFloat, m_fComponents[ 3 ] * fFloat ); }

    u_int GetRGBA() const;

private:
    
    float m_fComponents[ 4 ];

};

#endif
