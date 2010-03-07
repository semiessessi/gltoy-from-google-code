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

public:

    GLToy_FlatDataStructure() : GLToy_DataStructure< T >()
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
        CheckAlloc( pxDataStructure->GetCount() );

        GLToy_ConstIterate( T, xIterator, pxDataStructure )
        {
            m_pxData[ xIterator.Index() ] = xIterator.Current();
        }
    }

    T* m_pxData;
    u_int m_uSize;

};

#endif