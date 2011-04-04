/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010, 2011 Semi Essessi
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

#ifndef __GLTOY_MATRIX_H_
#define __GLTOY_MATRIX_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Maths/GLToy_Vector.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// M A C R O S
/////////////////////////////////////////////////////////////////////////////////////////////

#define GLToy_Matrix_RoundOut( T ) \
    GLToy_ForceInline T operator /( const float fFloat ) const { return operator *( 1.0f / fFloat ); }\
    GLToy_ForceInline friend T operator *( const float fFloat, const T& xMatrix ) { return xMatrix * fFloat; } \
    GLToy_ForceInline friend T operator /( const float fFloat, const T& xMatrix ) { return xMatrix / fFloat; } \
    GLToy_ForceInline T& operator +=( const T& xMatrix ) { return *this = ( *this + xMatrix ); } \
    GLToy_ForceInline T& operator -=( const T& xMatrix ) { return *this = ( *this - xMatrix ); } \
    GLToy_ForceInline T& operator *=( const float fFloat ) { return *this = ( *this * fFloat ); } \
    GLToy_ForceInline T& operator /=( const float fFloat ) { return operator *=( 1.0f / fFloat ); }

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Matrix_2
{
    
    friend class GLToy_Vector_2;

public:

    GLToy_CopyableStruct( GLToy_Matrix_2 )
    GLToy_MemoryEqual( GLToy_Matrix_2 )

    GLToy_Matrix_2();
    GLToy_Matrix_2( const GLToy_Vector_2& xX, const GLToy_Vector_2& xY );
    GLToy_Matrix_2( const float f00, const float f01, const float f10, const float f11 );

    ~GLToy_Matrix_2() {}

    GLToy_Inline GLToy_Vector_2& operator[] ( int i ) { return m_xComponents[ i ]; }
    GLToy_Inline const GLToy_Vector_2& operator[] ( int i ) const { return m_xComponents[ i ]; }

    GLToy_Inline GLToy_Vector_2* GetVectorPointer() { return m_xComponents; }
    GLToy_Inline const GLToy_Vector_2* const GetVectorPointer() const { return m_xComponents; }

    GLToy_Matrix_2 operator -() const;

    GLToy_Matrix_2 operator +( const GLToy_Matrix_2& xMatrix ) const;
    GLToy_Matrix_2 operator -( const GLToy_Matrix_2& xMatrix ) const;
    GLToy_Matrix_2 operator *( const GLToy_Matrix_2& xMatrix ) const;
    GLToy_Vector_2 operator *( const GLToy_Vector_2& xVector ) const;
    GLToy_Matrix_2 operator *( const float fValue ) const;

    void Identity();
    void Transpose();
	bool ApproximatelyEqual( const GLToy_Matrix_2& xMatrix, const float fThreshold = 0.0001f )
	{
		for( u_int u = 0; u < 2; ++u )
		{
			if( !m_xComponents[ u ].ApproximatelyEqual( xMatrix.m_xComponents[ u ] ) )
			{
				return false;
			}
		}

		return true;
	}

    GLToy_Matrix_RoundOut( GLToy_Matrix_2 )

private:
    
    GLToy_Vector_2 m_xComponents[ 2 ];

};

class GLToy_Matrix_3
{
    
    friend class GLToy_Vector_3;

public:

    GLToy_CopyableStruct( GLToy_Matrix_3 )
    GLToy_MemoryEqual( GLToy_Matrix_3 )

    GLToy_Matrix_3();
    GLToy_Matrix_3( const GLToy_Vector_3& xX, const GLToy_Vector_3& xY, const GLToy_Vector_3& xZ );
    GLToy_Matrix_3( const float f00, const float f01, const float f02, const float f10, const float f11, const float f12, const float f20, const float f21, const float f22 );

    ~GLToy_Matrix_3() {}

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
	GLToy_ForceInline void InvertTransformationMatrix() { Transpose(); }
	bool ApproximatelyEqual( const GLToy_Matrix_3& xMatrix, const float fThreshold = 0.0001f )
	{
		for( u_int u = 0; u < 3; ++u )
		{
			if( !m_xComponents[ u ].ApproximatelyEqual( xMatrix.m_xComponents[ u ] ) )
			{
				return false;
			}
		}

		return true;
	}

    GLToy_Matrix_RoundOut( GLToy_Matrix_3 )

private:
    
    GLToy_Vector_3 m_xComponents[ 3 ];

};

class GLToy_Matrix_4
{
    
    friend class GLToy_Vector_4;

public:

    GLToy_CopyableStruct( GLToy_Matrix_4 )
    GLToy_MemoryEqual( GLToy_Matrix_4 )

    GLToy_Matrix_4();
    GLToy_Matrix_4( const GLToy_Vector_4& xX, const GLToy_Vector_4& xY, const GLToy_Vector_4& xZ, const GLToy_Vector_4& xW );
    GLToy_Matrix_4(
        const float f00, const float f01, const float f02, const float f03,
        const float f10, const float f11, const float f12, const float f13,
        const float f20, const float f21, const float f22, const float f23,
        const float f30, const float f31, const float f32, const float f33 );
	GLToy_Matrix_4( const GLToy_Matrix_3& xMatrix );

    ~GLToy_Matrix_4() {}

    GLToy_Inline GLToy_Vector_4& operator[] ( int i ) { return m_xComponents[ i ]; }
    GLToy_Inline const GLToy_Vector_4& operator[] ( int i ) const { return m_xComponents[ i ]; }

    GLToy_Inline GLToy_Vector_4* GetVectorPointer() { return m_xComponents; }
    GLToy_Inline const GLToy_Vector_4* const GetVectorPointer() const { return m_xComponents; }

    GLToy_Matrix_4 operator -() const;

    GLToy_Matrix_4 operator +( const GLToy_Matrix_4& xMatrix ) const;
    GLToy_Matrix_4 operator -( const GLToy_Matrix_4& xMatrix ) const;
    GLToy_Matrix_4 operator *( const GLToy_Matrix_4& xMatrix ) const;
    GLToy_Vector_4 operator *( const GLToy_Vector_4& xVector ) const;
    GLToy_Matrix_4 operator *( const float fValue ) const;

    void Identity();
    void Transpose();
    void InvertTransformationMatrix();
	bool ApproximatelyEqual( const GLToy_Matrix_4& xMatrix, const float fThreshold = 0.0001f )
	{
		for( u_int u = 0; u < 4; ++u )
		{
			if( !m_xComponents[ u ].ApproximatelyEqual( xMatrix.m_xComponents[ u ] ) )
			{
				return false;
			}
		}

		return true;
	}

	GLToy_Matrix_3 GetUpper3x3() const { return GLToy_Matrix_3( m_xComponents[ 0 ], m_xComponents[ 1 ], m_xComponents[ 2 ] ); }

    GLToy_Matrix_RoundOut( GLToy_Matrix_4 )

private:
    
    GLToy_Vector_4 m_xComponents[ 4 ];

};

#endif
