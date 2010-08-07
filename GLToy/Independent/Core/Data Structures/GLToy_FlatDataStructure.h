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

#ifndef __GLTOY_FLATDATASTRUCTURE_H
#define __GLTOY_FLATDATASTRUCTURE_H

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/Data Structures/GLToy_DataStructure.h>

// GLToy
#include <Core/Data Structures/GLToy_BitStream.h>

// C/C++
#include <malloc.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const u_int uGLTOY_FLATDATASTRUCTURE_START_ALLOC = 4;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

template< class T >
class GLToy_FlatDataStructure
: public GLToy_DataStructure< T >
{

    typedef GLToy_DataStructure< T > GLToy_Parent;

public:

    GLToy_FlatDataStructure()
    : GLToy_Parent()
    {
        // set up initial memory usage
        m_uSize = uGLTOY_FLATDATASTRUCTURE_START_ALLOC;
        m_pxData = static_cast< T* >( malloc( sizeof( T ) * m_uSize ) ); 
    }

    virtual ~GLToy_FlatDataStructure()
    {
        // clean up any allocated memory
        if( m_pxData )
        {
            free( m_pxData );
        }
    }

    GLToy_FlatDataStructure( const GLToy_FlatDataStructure& xFlatDataStructure )
    {
        m_uSize = xFlatDataStructure.m_uSize;
        m_pxData = static_cast< T* >( malloc( sizeof( T ) * m_uSize ) );
    }
    
    virtual u_int GetCount() const
    {
        return 0; // we can't store data in this thing, only allocate for it
    }

    virtual u_int GetMemoryUsage() const
    {
        return sizeof( *this ) + sizeof( T ) * m_uSize;
    }

    virtual bool IsFlat() const
    {
        return true;
    }

    virtual T& operator []( const int iIndex )
    {
        return m_pxData[ iIndex ];
    }

    virtual const T& operator []( const int iIndex ) const
    {
        return m_pxData[ iIndex ];
    }

    GLToy_FlatDataStructure& operator =( const GLToy_FlatDataStructure& xDataStructure )
    {
        CopyFrom( &xDataStructure );
        return *this;
    }

    virtual void Traverse( GLToy_Functor< T >& xFunctor )
    {
        GLToy_Iterate( T, xIterator, this )
        {
            xFunctor( &( xIterator.Current() ) );
        }
    }

    virtual void Traverse( GLToy_ConstFunctor< T >& xFunctor ) const
    {
        GLToy_ConstIterate( T, xIterator, this )
        {
            xFunctor( &( xIterator.Current() ) );
        }
    }

    T* GetDataPointer() { return m_pxData; }
    const T* GetDataPointer() const { return m_pxData; }

protected:

    void CheckAlloc( const u_int& uCount )
    {
        if( uCount < uGLTOY_FLATDATASTRUCTURE_START_ALLOC )
        {
            return; // do nothing if we are smaller than the initial alloc
        }

        // if uCount goes over m_uSize allocate another m_uSize entries
        if( uCount > m_uSize )
        {
            while( uCount > m_uSize )
            {
                m_uSize <<= 1;
            }

            m_pxData = static_cast< T* >( realloc( m_pxData, sizeof( T ) * m_uSize ) );
        }
        // else if uCount goes below half of m_uSize then reallocate
        else if( uCount < ( m_uSize >> 1 ) )
        {
            while( uCount < ( m_uSize >> 1 ) )
            {
                if( m_uSize == uGLTOY_FLATDATASTRUCTURE_START_ALLOC )
                {
                    break;
                }

                m_uSize >>= 1;
            }
            m_pxData = static_cast< T* >( realloc( m_pxData, sizeof( T ) * m_uSize ) );
        }
    }

    virtual void CopyFrom( const GLToy_DataStructure< T >* const pxDataStructure )
    {
        const u_int uNewCount = pxDataStructure->GetCount();
        const u_int uOldCount = GetCount();
        CheckAlloc( pxDataStructure->GetCount() );

        // make sure existing data is destoyed correctly
        if( uOldCount > uNewCount )
        {
            for( u_int u = uNewCount; u < uOldCount; ++u )
            {
                m_pxData[ u ].~T();
            }
        }

        const u_int uAllocCount = ( uOldCount > uNewCount ) ? uNewCount : uOldCount;

        GLToy_ConstIterate( T, xIterator, pxDataStructure )
        {
            if( static_cast< u_int >( xIterator.Index() ) < uAllocCount )
            {
                m_pxData[ xIterator.Index() ] = xIterator.Current();
            }
            else
            {
                new ( &( m_pxData[ xIterator.Index() ] ) ) T( xIterator.Current() );
            }
        }
    }

    T* m_pxData;
    u_int m_uSize;

};

#endif
