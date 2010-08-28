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

#ifndef __GLTOY_POOL_H_
#define __GLTOY_POOL_H_

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
class GLToy_Pool
{

    struct Allocation
    {
        u_char      m_aucData[ sizeof( T ) ];
        bool        m_bFree;

        Allocation()
        : m_bFree( true )
        {
        }
    };

public:

    GLToy_Pool( const u_int uInitialSize = 256 )
    : m_xData()
    , m_uAllocated( 0 )
    , m_uCapacity( uInitialSize )
    , m_uCursor( 0 )
    , m_uListCursor( 0 )
    {
        m_xData.Push( new GLToy_Array< Allocation >() );
        m_xData.Peek()->Resize( uInitialSize );
    }

    ~GLToy_Pool()
    {
        m_xData.DeleteAll();
    }

    GLToy_Inline T* Allocate()
    {
        ++m_uAllocated;
        if( m_uAllocated == m_uCapacity )
        {
            const u_int uNewSize = m_uCapacity;
            m_uCapacity <<= 1;
            m_xData.Push( new GLToy_Array< Allocation >() );
            m_xData.Peek()->Resize( uNewSize );
        }

        GLToy_Array< Allocation >* pxAllocationData = m_xData[ m_uListCursor ];
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

        ( *pxAllocationData )[ m_uCursor ].m_bFree = false;
        return reinterpret_cast< T* >( &( ( *pxAllocationData )[ m_uCursor ].m_aucData ) );
    }

    GLToy_Inline void Free( T* const pxPointer )
    {
        reinterpret_cast< Allocation* const >( pxPointer )->m_bFree = true;
        --m_uAllocated;
#ifndef GLTOY_FINAL
        for( u_int u = 0; u < sizeof( T ); ++u )
        {
            reinterpret_cast< Allocation* const >( pxPointer )->m_aucData[ u ] = 0xDD;
        }
#endif
    }

protected:


    GLToy_Stack< GLToy_Array< Allocation >* > m_xData;
    u_int m_uAllocated;
    u_int m_uCapacity;
    u_int m_uCursor;
    u_int m_uListCursor;

};

template< class T >
class GLToy_PoolAllocated
{

public:

    GLToy_Inline void* operator new( const size_t uSize )
    {
        return s_pxPool.Allocate();
    }

    GLToy_Inline void operator delete( void* const pxPointer )
    {
        s_pxPool.Free( reinterpret_cast< T* const >( pxPointer ) );
    }

private:

    static GLToy_Pool< T > s_pxPool;

};

template< class T >
GLToy_Pool< T > GLToy_PoolAllocated< T >::s_pxPool;

#endif
