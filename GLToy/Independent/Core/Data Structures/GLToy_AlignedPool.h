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

    struct AlignedBlock
    {
        void* m_pData;
        void* m_pAlignedPointer;
        bool  m_bFree;
        
        AlignedBlock( const u_int uSize = 1 )
        : m_pData( NULL )
        , m_pAlignedPointer( NULL )
        , m_bFree( true )
        {
            Size( uSize );
        }

        void Size( const u_int uSize )
        {
            delete[] m_pData;
            m_pData = reinterpret_cast< void* >( new u_char[ sizeof( T ) * ( uSize + 1 ) ] );
            m_pAlignedPointer = m_pData;
            while( reinterpret_cast< u_int >( m_pAlignedPointer ) % sizeof( T ) )
            {
                m_pAlignedPointer = reinterpret_cast< u_char* >( m_pAlignedPointer ) + 1;
            }
        }

        ~AlignedBlock()
        {
            delete[] m_pData;
        }
    };

public:
#include <Core/GLToy_Memory_DebugOff.h>
    GLToy_AlignedPool( const u_int uInitialSize = 256 )
    : m_xData()
    , m_uAllocated( 0 )
    , m_uCapacity( uInitialSize )
    , m_uCursor( 0 )
    , m_uListCursor( 0 )
    {
        m_xData.Push( new GLToy_Array< AlignedBlock >() );
        m_xData.Peek()->Resize( uInitialSize );
    }
#include <Core/GLToy_Memory_DebugOn.h>
    ~GLToy_AlignedPool()
    {
        m_xData.DeleteAll();
    }

    GLToy_Inline T* Allocate( const u_int uCount = 1 )
    {
        ++m_uAllocated;
        if( m_uAllocated == m_uCapacity )
        {
            const u_int uNewSize = m_uCapacity;
            m_uCapacity <<= 1;
            m_xData.Push( new GLToy_Array< AlignedBlock >() );
            m_xData.Peek()->Resize( uNewSize );
        }

        GLToy_Array< AlignedBlock >* pxAllocationData = m_xData[ m_uListCursor ];
        while( !( ( *pxAllocationData )[ m_uCursor ].m_bFree ) )
        {
            ++m_uCursor;
            if( m_uCursor == pxAllocationData->GetCount() )
            {
                ++m_uListCursor;
                m_uCursor = 0;
                if( m_uListCursor == m_xData.GetCount() )
                {
                    m_uListCursor = 0;
                }
            }
            pxAllocationData = m_xData[ m_uListCursor ];
        }

        ( *pxAllocationData )[ m_uCursor ].Size( uCount );

#ifdef GLTOY_DEBUG
        GLToy_Memory::MarkUninitialised( &( ( *pxAllocationData )[ m_uCursor ].m_pAlignedPointer ), sizeof( T ) * uCount );
#endif

        ( *pxAllocationData )[ m_uCursor ].m_bFree = false;
        return reinterpret_cast< T* >( &( ( *pxAllocationData )[ m_uCursor ].m_pAlignedPointer ) );
    }

    GLToy_Inline void Free( T* const pxPointer )
    {
        reinterpret_cast< AlignedBlock* const >( pxPointer )->m_bFree = true;
        --m_uAllocated;

#ifdef GLTOY_DEBUG
        GLToy_Memory::MarkDestroyed( reinterpret_cast< Allocation* const >( pxPointer )->m_aucData, sizeof( T ) );
#endif
    }

protected:


    GLToy_Stack< GLToy_Array< AlignedBlock >* > m_xData;
    u_int m_uAllocated;
    u_int m_uCapacity;
    u_int m_uCursor;
    u_int m_uListCursor;

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
