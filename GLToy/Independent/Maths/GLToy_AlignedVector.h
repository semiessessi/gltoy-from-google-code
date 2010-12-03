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

#ifndef __GLTOY_ALIGNED_VECTOR_H_
#define __GLTOY_ALIGNED_VECTOR_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/Data Structures/GLToy_AlignedPool.h>

// GLToy
#include <Maths/GLToy_Vector.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_AlignedVector_Data
: public GLToy_AlignedPoolAllocated< GLToy_AlignedVector_Data >
{

public:

    float& operator[] ( const int i ) { return m_afComponents[ i ]; }
    const float& operator[] ( const int i ) const { return m_afComponents[ i ]; }

private:

    float m_afComponents[ 4 ];

};

// NOTE: no virtual functions - this is important for this sort of speed...
class GLToy_AlignedVector_Base
: public GLToy_Destroyable< GLToy_AlignedVector_Base >
{

public:

    GLToy_AlignedVector_Base()
    : m_pxComponents( new GLToy_AlignedVector_Data )
    {
    }

    GLToy_AlignedVector_Base( const float fFloat )
    : m_pxComponents( new GLToy_AlignedVector_Data )
    {
        Platform_Set1( fFloat );
    }

	GLToy_ForceInline void Destroy() { delete m_pxComponents; }

	GLToy_ForceInline float& operator[] ( const int i ) { return ( *m_pxComponents )[ i ]; }
    GLToy_ForceInline const float& operator[] ( const int i ) const { return ( *m_pxComponents )[ i ]; }

	template < class AlignedVectorType >
	GLToy_PointerArray< AlignedVectorType > CreateFastArray( const u_int uCount )
	{
		GLToy_AlignedVector_Data* const pxData = new GLToy_AlignedVector_Data[ uCount ];
		// to avoid all the new calls for the data allocate u_char to avoid the AlignedVectorType constructor
		AlignedVectorType* const pxVectors = reinterpret_cast< AlignedVectorType* >( new u_char[ sizeof( AlignedVectorType ) * uCount ] );
		for( u_int u = 0; u < uCount; ++u )
		{
			pxVectors[ u ].m_pxComponents = &( pxData[ u ] );
		}

		return GLToy_PointerArray< AlignedVectorType >( pxVectors );
	}

	template < class AlignedVectorType >
	void DestroyFastArray( GLToy_PointerArray< AlignedVectorType >& xArray )
	{
		delete[] xArray[ 0 ].m_pxComponents;
		delete[] &( xArray[ 0 ] );
	}

protected:

	void Platform_Copy4( const GLToy_AlignedVector_Data& xData );
	void Platform_Copy4( const float* const pfData );

	void Platform_Set1( const float fValue );
	void Platform_Set2( const float fValue1, const float fValue2 );
	void Platform_Set3( const float fValue1, const float fValue2, const float fValue3 );
	void Platform_Set4( const float fValue1, const float fValue2, const float fValue3, const float fValue4 );

    void Platform_Add4( const GLToy_AlignedVector_Data& xData );
    void Platform_Sub4( const GLToy_AlignedVector_Data& xData );

	float Platform_Dot2( const GLToy_AlignedVector_Data& xData ) const;
	float Platform_Dot3( const GLToy_AlignedVector_Data& xData ) const;
	float Platform_Dot4( const GLToy_AlignedVector_Data& xData ) const;

    GLToy_AlignedVector_Data* m_pxComponents;

};

class GLToy_AlignedVector_2
: public GLToy_AlignedVector_Base
{

    typedef GLToy_AlignedVector_Base GLToy_Parent;

public:

	GLToy_AlignedVector_2( const float fX, const float fY ) { Platform_Set2( fX, fY ); }
    GLToy_AlignedVector_2( const GLToy_Vector_2& xVector ) { Platform_Copy4( &( xVector[ 0 ] ) ); }
	GLToy_AlignedVector_2( const GLToy_AlignedVector_2& xVector ) { Platform_Copy4( *xVector.m_pxComponents ); }

	GLToy_ForceInline GLToy_AlignedVector_2& operator =( const GLToy_AlignedVector_2& xVector ) { Platform_Copy4( *xVector.m_pxComponents ); return *this; }
    
    GLToy_ForceInline GLToy_AlignedVector_2& operator +=( const GLToy_AlignedVector_2& xVector ) { Platform_Add4( *xVector.m_pxComponents ); return *this; }
    GLToy_ForceInline GLToy_AlignedVector_2& operator -=( const GLToy_AlignedVector_2& xVector ) { Platform_Sub4( *xVector.m_pxComponents ); return *this; }

	GLToy_ForceInline GLToy_AlignedVector_2 operator +( const GLToy_AlignedVector_2& xVector ) const { GLToy_AlignedVector_2 xReturnValue( *this ); xReturnValue.Platform_Add4( *xVector.m_pxComponents ); return xReturnValue; }
	GLToy_ForceInline GLToy_AlignedVector_2 operator -( const GLToy_AlignedVector_2& xVector ) const { GLToy_AlignedVector_2 xReturnValue( *this ); xReturnValue.Platform_Sub4( *xVector.m_pxComponents ); return xReturnValue; }

	GLToy_ForceInline float operator *( const GLToy_AlignedVector_2& xVector ) const { return Platform_Dot2( *xVector.m_pxComponents ); }

};

class GLToy_AlignedVector_3
: public GLToy_AlignedVector_Base
{

    typedef GLToy_AlignedVector_Base GLToy_Parent;

public:

	GLToy_AlignedVector_3( const float fX, const float fY, const float fZ ) { Platform_Set3( fX, fY, fZ ); }
    GLToy_AlignedVector_3( const GLToy_Vector_3& xVector ) { Platform_Copy4( &( xVector[ 0 ] ) ); }
	GLToy_AlignedVector_3( const GLToy_AlignedVector_3& xVector ) { Platform_Copy4( *xVector.m_pxComponents ); }

	GLToy_ForceInline GLToy_AlignedVector_3& operator =( const GLToy_AlignedVector_3& xVector ) { Platform_Copy4( *xVector.m_pxComponents ); return *this; }
    
    GLToy_ForceInline GLToy_AlignedVector_3& operator +=( const GLToy_AlignedVector_3& xVector ) { Platform_Add4( *xVector.m_pxComponents ); return *this; }
    GLToy_ForceInline GLToy_AlignedVector_3& operator -=( const GLToy_AlignedVector_3& xVector ) { Platform_Sub4( *xVector.m_pxComponents ); return *this; }

	GLToy_ForceInline GLToy_AlignedVector_3 operator +( const GLToy_AlignedVector_3& xVector ) const { GLToy_AlignedVector_3 xReturnValue( *this ); xReturnValue.Platform_Add4( *xVector.m_pxComponents ); return xReturnValue; }
	GLToy_ForceInline GLToy_AlignedVector_3 operator -( const GLToy_AlignedVector_3& xVector ) const { GLToy_AlignedVector_3 xReturnValue( *this ); xReturnValue.Platform_Sub4( *xVector.m_pxComponents ); return xReturnValue; }

	GLToy_ForceInline float operator *( const GLToy_AlignedVector_3& xVector ) const { return Platform_Dot3( *xVector.m_pxComponents ); }

};

class GLToy_AlignedVector_4
: public GLToy_AlignedVector_Base
{

    typedef GLToy_AlignedVector_Base GLToy_Parent;

public:

	GLToy_AlignedVector_4( const float fX, const float fY, const float fZ, const float fW ) { Platform_Set4( fX, fY, fZ, fW ); }
    GLToy_AlignedVector_4( const GLToy_Vector_4& xVector ) { Platform_Copy4( &( xVector[ 0 ] ) ); }
	GLToy_AlignedVector_4( const GLToy_AlignedVector_4& xVector ) { Platform_Copy4( *xVector.m_pxComponents ); }

	GLToy_ForceInline GLToy_AlignedVector_4& operator =( const GLToy_AlignedVector_4& xVector ) { Platform_Copy4( *xVector.m_pxComponents ); return *this; }
    
    GLToy_ForceInline GLToy_AlignedVector_4& operator +=( const GLToy_AlignedVector_4& xVector ) { Platform_Add4( *xVector.m_pxComponents ); return *this; }
    GLToy_ForceInline GLToy_AlignedVector_4& operator -=( const GLToy_AlignedVector_4& xVector ) { Platform_Sub4( *xVector.m_pxComponents ); return *this; }

	GLToy_ForceInline GLToy_AlignedVector_4 operator +( const GLToy_AlignedVector_4& xVector ) const { GLToy_AlignedVector_4 xReturnValue( *this ); xReturnValue.Platform_Add4( *xVector.m_pxComponents ); return xReturnValue; }
	GLToy_ForceInline GLToy_AlignedVector_4 operator -( const GLToy_AlignedVector_4& xVector ) const { GLToy_AlignedVector_4 xReturnValue( *this ); xReturnValue.Platform_Sub4( *xVector.m_pxComponents ); return xReturnValue; }

	GLToy_ForceInline float operator *( const GLToy_AlignedVector_4& xVector ) const { return Platform_Dot4( *xVector.m_pxComponents ); }

};

#endif