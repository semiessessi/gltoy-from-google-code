/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009-2011 Semi Essessi
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
// M A C R O S
/////////////////////////////////////////////////////////////////////////////////////////////

#define GLToy_Vector_RoundOut( T ) \
    GLToy_ForceInline float& operator[] ( int i ) { return m_fComponents[ i ]; } \
    GLToy_ForceInline const float& operator[] ( int i ) const { return m_fComponents[ i ]; } \
    GLToy_ForceInline float* GetFloatPointer() { return m_fComponents; } \
    GLToy_ForceInline const float* GetFloatPointer() const { return m_fComponents; } \
    GLToy_ForceInline T& operator +=( const T& xVector ) { return *this = ( *this + xVector ); } \
    GLToy_ForceInline T& operator -=( const T& xVector ) { return *this = ( *this - xVector ); } \
    GLToy_ForceInline T& operator *=( const float fFloat ) { return *this = ( *this * fFloat ); } \
    GLToy_ForceInline T& operator /=( const float fFloat ) { return operator *=( 1.0f / fFloat ); } \
    GLToy_ForceInline friend T operator *( const float fFloat, const T& xVector ) { return xVector * fFloat; } \
    GLToy_ForceInline T operator /( const float fFloat ) const { return operator *( 1.0f / fFloat ); } \
    GLToy_ForceInline float MagnitudeSquared() const { return *this * *this; } \
    GLToy_ForceInline void Normalise() { const float fMagnitudeSquared = MagnitudeSquared(); GLToy_Assert( fMagnitudeSquared != 0.0f, "Trying to normalise a zero vector!" ); *this = *this * GLToy_Vector_InvSqrt_Helper( fMagnitudeSquared ); } \
    GLToy_ForceInline float Magnitude() const { return GLToy_Vector_Sqrt_Helper( MagnitudeSquared() ); } \
	GLToy_ForceInline bool ApproximatelyEqual( const T& xVector, const float fThreshold = 0.0001f ) const { return ( *this - xVector ).MagnitudeSquared() < fThreshold; } \

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_BitStream;
class GLToy_Maths;
class GLToy_Matrix_3;
class GLToy_Vector_3;
class GLToy_Vector_4;

float GLToy_Vector_Sqrt_Helper( const float f );
float GLToy_Vector_InvSqrt_Helper( const float f );

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Vector_2
{

public:

    GLToy_CopyableStruct( GLToy_Vector_2 )
    GLToy_MemoryEqual( GLToy_Vector_2 )

    GLToy_Vector_2();
    GLToy_Vector_2( float fX, float fY );
    GLToy_Vector_2( const GLToy_Vector_3& xVector );
    GLToy_Vector_2( const GLToy_Vector_4& xVector );

    GLToy_ForceInline GLToy_Vector_2 operator -() const { return GLToy_Vector_2( -m_fComponents[ 0 ], -m_fComponents[ 1 ] ); }

    GLToy_Inline GLToy_Vector_2 operator +( const GLToy_Vector_2& xVector ) const { return GLToy_Vector_2( m_fComponents[ 0 ] + xVector[ 0 ], m_fComponents[ 1 ] + xVector[ 1 ] ); }
    GLToy_Inline GLToy_Vector_2 operator -( const GLToy_Vector_2& xVector ) const { return GLToy_Vector_2( m_fComponents[ 0 ] - xVector[ 0 ], m_fComponents[ 1 ] - xVector[ 1 ] ); }
    GLToy_Vector_2 operator *( const float fFloat ) const;

    GLToy_Inline float operator *( const GLToy_Vector_2& xVector ) const
    {
        return m_fComponents[ 0 ] * xVector[ 0 ] + m_fComponents[ 1 ] * xVector[ 1 ];
    }

    GLToy_Vector_RoundOut( GLToy_Vector_2 )

	union
	{
		float m_fComponents[ 2 ];
		struct
		{
			float x;
			float y;
		};
	};

};

class GLToy_Vector_3
{

    friend class GLToy_Matrix_3;

public:

    GLToy_CopyableStruct( GLToy_Vector_3 )
    GLToy_MemoryEqual( GLToy_Vector_3 )

    GLToy_Vector_3();
    GLToy_Vector_3( float fX, float fY, float fZ );
    GLToy_Vector_3( const GLToy_Vector_2& xVector, const float fZ = 1.0f );
    GLToy_Vector_3( const GLToy_Vector_4& xVector );

    GLToy_Vector_3 operator -() const;

    GLToy_Vector_3& operator *=( const GLToy_Matrix_3& xMatrix );

    GLToy_Vector_3 operator +( const GLToy_Vector_3& xVector ) const;
    GLToy_Vector_3 operator -( const GLToy_Vector_3& xVector ) const;
    GLToy_Vector_3 operator *( const float fFloat ) const;
    GLToy_Vector_3 operator *( const GLToy_Matrix_3& xMatrix ) const;

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

    GLToy_Vector_RoundOut( GLToy_Vector_3 )

	union
	{
		float m_fComponents[ 3 ];
		struct
		{
			float x;
			float y;
			float z;
		};
	};

};

class GLToy_Vector_4
{

public:

    GLToy_CopyableStruct( GLToy_Vector_4 )
    GLToy_MemoryEqual( GLToy_Vector_4 )

    GLToy_Vector_4();
    GLToy_Vector_4( const GLToy_Vector_2& xXY, const GLToy_Vector_2& xWZ );
    GLToy_Vector_4( const GLToy_Vector_2& xXY, const float fZ = 0.0f, const float fW = 1.0f  );
    GLToy_Vector_4( const GLToy_Vector_3& xVector, const float fW = 1.0f );
    GLToy_Vector_4( float fX, float fY, float fZ, float fW );
    GLToy_Vector_4( const u_int uRGBA );

    GLToy_Vector_4 operator -() const { return GLToy_Vector_4( -m_fComponents[ 0 ], -m_fComponents[ 1 ], -m_fComponents[ 2 ], -m_fComponents[ 3 ] ); }

    GLToy_Inline GLToy_Vector_4 operator +( const GLToy_Vector_4& xVector ) const { return GLToy_Vector_4( m_fComponents[ 0 ] + xVector[ 0 ], m_fComponents[ 1 ] + xVector[ 1 ], m_fComponents[ 2 ] + xVector[ 2 ], m_fComponents[ 3 ] + xVector[ 3 ] ); }
    GLToy_Inline GLToy_Vector_4 operator -( const GLToy_Vector_4& xVector ) const { return GLToy_Vector_4( m_fComponents[ 0 ] - xVector[ 0 ], m_fComponents[ 1 ] - xVector[ 1 ], m_fComponents[ 2 ] - xVector[ 2 ], m_fComponents[ 3 ] - xVector[ 3 ] ); }
    GLToy_Inline GLToy_Vector_4 operator *( const float fFloat ) const { return GLToy_Vector_4( m_fComponents[ 0 ] * fFloat, m_fComponents[ 1 ] * fFloat, m_fComponents[ 2 ] * fFloat, m_fComponents[ 3 ] * fFloat ); }

    GLToy_Inline float operator *( const GLToy_Vector_4& xVector ) const
    {
        return m_fComponents[ 0 ] * xVector[ 0 ]
            + m_fComponents[ 1 ] * xVector[ 1 ]
            + m_fComponents[ 2 ] * xVector[ 2 ]
            + m_fComponents[ 3 ] * xVector[ 3 ];
    }

    u_int GetRGBA() const;

    GLToy_Vector_RoundOut( GLToy_Vector_4 )

public:
    
	union
	{
		float m_fComponents[ 4 ];
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
	};

};

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

template< class VectorType >
GLToy_ForceInline VectorType Normalise( const VectorType& xVector )
{
    VectorType xReturnValue( xVector );
    xReturnValue.Normalise();
    return xReturnValue;
}

template< class VectorType >
GLToy_ForceInline VectorType Cross( const VectorType& xVector1, const VectorType& xVector2 )
{
    return xVector1.Cross( xVector2 );
}

template< class VectorType >
GLToy_ForceInline float Dot( const VectorType& xVector1, const VectorType& xVector2 )
{
    return xVector1 * xVector2;
}

#endif
