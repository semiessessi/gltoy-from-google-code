#ifndef __GLTOY_DATASTRUCTURE_H
#define __GLTOY_DATASTRUCTURE_H

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy_Functor.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

template< class T >
class GLToy_DataStructure
{

public:

    GLToy_DataStructure() {}
    GLToy_DataStructure( const GLToy_DataStructure& xDataStructure ) {}
    virtual ~GLToy_DataStructure() {}
    
    virtual u_int GetCount() const = 0;
    virtual u_int GetMemoryUsage() const = 0;

    virtual bool IsFlat() const = 0;

    virtual T& operator []( const int iIndex ) = 0;
    virtual const T& operator []( const int iIndex ) const = 0;

    virtual void Traverse( GLToy_Functor< T >& xFunctor ) = 0;
    virtual void Traverse( GLToy_ConstFunctor< T >& xFunctor ) const = 0;

protected:

    virtual void CopyFrom( const GLToy_DataStructure< T >* const pxDataStructure ) = 0;

};

template< class T >
class GLToy_PointerArray
: public GLToy_DataStructure< T >
{

public:

    GLToy_PointerArray( T* const pxData, const u_int uCount )
    : m_pxData( pxData )
    , m_uCount( uCount )
    {
    }

    GLToy_PointerArray( const GLToy_PointerArray& xDataStructure )
    : m_pxData( xDataStructure.m_pxData )
    , m_uCount( xDataStructure.m_uCount )
    {
    }

    virtual u_int GetCount() const { return m_uCount; }
    virtual u_int GetMemoryUsage() const { return sizeof( GLToy_PointerArray ); }

    virtual bool IsFlat() const { return false; }

    virtual T& operator []( const int iIndex ) { return m_pxData[ iIndex ]; }
    virtual const T& operator []( const int iIndex ) const  { return m_pxData[ iIndex ]; }

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

protected:

    virtual void CopyFrom( const GLToy_DataStructure< T >* const pxDataStructure )
    {
        GLToy_ConstIterate( T, xIterator, pxDataStructure )
        {
            m_pxData[ xIterator.Index() ] = xIterator.Current();
        }
    }

    T* m_pxData;
    u_int uCount;

};

template< class T >
class GLToy_Iterator
{

public:

    GLToy_Iterator( GLToy_DataStructure< T >* const pxDataStructure )
    : m_pxDataStructure( pxDataStructure )
    , m_iIndex( 0 )
    {
    }

    T& Current() const { return m_pxDataStructure->operator []( m_iIndex ); }
    int Index() const { return m_iIndex; }

    void Next() { ++m_iIndex; }
    bool Done() const { return static_cast< u_int >( m_iIndex ) >= m_pxDataStructure->GetCount(); }

protected:

    GLToy_DataStructure< T >* m_pxDataStructure;
    int m_iIndex;

};

template< class T >
class GLToy_ConstIterator
{

public:

    GLToy_ConstIterator( const GLToy_DataStructure< T >* const pxDataStructure )
    : m_pxDataStructure( pxDataStructure )
    , m_iIndex( 0 )
    {
    }

    const T& Current() const { return m_pxDataStructure->operator []( m_iIndex ); }
    int Index() const { return m_iIndex; }

    void Next() { ++m_iIndex; }
    bool Done() const { return static_cast< u_int >( m_iIndex ) >= m_pxDataStructure->GetCount(); }

protected:

    const GLToy_DataStructure< T >* m_pxDataStructure;
    int m_iIndex;

};

#define GLToy_Iterate( T, xIteratorName, pxDataStructure ) for( GLToy_Iterator< T > xIterator( pxDataStructure ); !xIterator.Done(); xIterator.Next() )
#define GLToy_ConstIterate( T, xIteratorName, pxDataStructure ) for( GLToy_ConstIterator< T > xIterator( pxDataStructure ); !xIterator.Done(); xIterator.Next() )

#endif