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

#ifndef __GLTOY_ALIGNEDPOOL_H_
#define __GLTOY_ALIGNEDPOOL_H_

// TODO: this lazy pile of crap can be seriously optimised...
// its not even a real pool

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Core/Data Structures/GLToy_Array.h>
#include <Core/Data Structures/GLToy_Stack.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

template< class T >
class GLToy_AlignedPool
{

public:

    GLToy_AlignedPool( const u_int uInitialSize = 256 )
    {
    }

    ~GLToy_AlignedPool()
    {
    }

    GLToy_Inline T* Allocate( const u_int uCount = 1 )
    {
		// allocate enough space for the offset and the aligned block
		u_char* const pucAllocation = new u_char[ sizeof( T ) * ( uCount + 1 ) + 4 ];
		// calculate the offset from the start of the aligned block to the first aligned address
		const u_int uOffset = 4 + sizeof( T ) - ( reinterpret_cast< u_int >( pucAllocation + 4 ) % sizeof( T ) );
		// store the offset
		*reinterpret_cast< u_int* >( pucAllocation + uOffset - 4 ) = uOffset;
		// return the first aligned address
		return reinterpret_cast< T* >( pucAllocation + uOffset );
    }

    GLToy_Inline void Free( T* const pxPointer )
    {
		if( !pxPointer )
		{
			return;
		}

		// extract the offset to the start of the allocation
		const u_int uOffset = *( reinterpret_cast< u_int* >( pxPointer ) - 1 );
		// free the allocation
		delete[] ( reinterpret_cast< u_char* >( pxPointer ) - uOffset );
    }

};

#include <Core/GLToy_Memory_DebugOff.h>

template< class T >
class GLToy_AlignedPoolAllocated
{

public:

#ifdef GLTOY_DEBUG
    GLToy_Inline void* operator new( const u_int uSize, const char* szFile, const int iLine ) { return operator new( uSize ); }
#endif

    GLToy_ForceInline void* operator new( const u_int uSize )
    {
        return s_xPool.Allocate();
    }

    GLToy_ForceInline void* operator new[]( const u_int uSize )
    {
        return s_xPool.Allocate( uSize / sizeof( T ) );
    }

    GLToy_ForceInline void operator delete( void* const pxPointer )
    {
        s_xPool.Free( reinterpret_cast< T* const >( pxPointer ) );
    }

    GLToy_ForceInline void operator delete[]( void* const pxPointer )
    {
        s_xPool.Free( reinterpret_cast< T* const >( pxPointer ) );
    }

private:

    static GLToy_AlignedPool< T > s_xPool;

};

template< class T >
GLToy_AlignedPool< T > GLToy_AlignedPoolAllocated< T >::s_xPool;

#include <Core/GLToy_Memory_DebugOn.h>

#endif
