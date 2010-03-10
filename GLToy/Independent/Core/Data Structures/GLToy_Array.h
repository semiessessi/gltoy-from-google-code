#ifndef __GLTOY_ARRAY_H_
#define __GLTOY_ARRAY_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/Data Structures/GLToy_FlatDataStructure.h>
#include <Core/GLToy_Serialisable.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

template< class T >
class GLToy_Array
: public GLToy_FlatDataStructure< T >
{
    typedef GLToy_FlatDataStructure< T > GLToy_Parent;

public:

    GLToy_Array()
    : GLToy_Parent()
    , m_uCount( 0 )
    {
    }
    
    GLToy_Array( const GLToy_Array& xArray )
    : GLToy_Parent( xArray )
    , m_uCount( xArray.m_uCount )
    {
        CopyFrom( &xArray );
    }
    
    GLToy_Array& operator =( const GLToy_FlatDataStructure< T >& xDataStructure )
    {
        CopyFrom( &xDataStructure );
        m_uCount = xDataStructure.GetCount();

        return *this;
    }
    
    void Append( const T& xValue )
    {
        // increment m_uCount then make sure memory is allocated before setting the new entry
        ++m_uCount;
        CheckAlloc( m_uCount );
        m_pxData[ m_uCount - 1 ] = xValue;
    }
    
    void Append( const GLToy_Array& xValues )
    {
        // store m_uCount for later use when copying
        u_int oldCount = m_uCount;

        // increase m_uCount and allocate accordingly
        m_uCount += xValues.m_uCount;
        CheckAlloc( m_uCount );

        for( u_int i = 0; i < xValues.m_uCount; ++i )
        {
            m_pxData[ oldCount + i ] = xValues.m_pxData[ i ];
        }
    }
    
    void InsertAt( const int iIndex, const T& xValue )
    {
        // increase m_uCount and check allocation
        ++m_uCount;
        CheckAlloc( m_uCount );

        // move the existing xValues along to make room
        for( u_int i = m_uCount - 1; i >= iIndex; --i )
        {
            m_pxData[ i ] = m_pxData[ i - 1 ];
        }
        m_pxData[ iIndex ] = xValue;
    }
    
    void InsertAt( const int iIndex, const GLToy_Array& xValues )
    {
        // increase m_uCount and check allocation
        m_uCount += xValues.m_uCount;
        CheckAlloc( m_uCount );

        // move the existing xValues along to make room
        for( u_int i = m_uCount - 1; i >= ( iIndex + xValues.m_uCount ); --i )
        {
            m_pxData[ i ] = m_pxData[ i - xValues.m_uCount ];
        }

        // insert new xValues
        for( u_int i = 0; i < xValues.m_uCount; ++i )
        {
            m_pxData[ iIndex + i ] = xValues.m_pxData[ i ];
        }
    }
    
    void RemoveAt( const int iIndex, const u_int uAmount = 1 )
    {
        m_uCount -= uAmount;
        for( u_int i = iIndex; i < m_uCount; ++i )
        {
            m_pxData[ i ] = m_pxData[ i + uAmount ];
        }
    }
    
    void RemoveFromEnd( const u_int uAmount = 1 )
    {
        m_uCount -= uAmount;
    }

    void Resize( const u_int uCount )
    {
        m_uCount = uCount;
        CheckAlloc( m_uCount );
    }
    
    virtual u_int GetCount() const
    {
        return m_uCount;
    }

    virtual u_int GetMemoryUsage() const
    {
        return GLToy_Parent::GetMemoryUsage() + sizeof( m_uCount );
    }
    
    T& Start()
    {
        return &( m_pxData[ 0 ] );
    }
    
    T& End()
    {
        return &( m_pxData[ m_uCount - 1 ] );
    }
    
    void Clear()
    {
        m_uCount = 0;
    }

protected:

    u_int m_uCount;

};

template< class T >
class GLToy_IndirectArray
: public GLToy_DataStructure< T >
{

public:

    virtual u_int GetCount() const { return m_xArray.GetCount(); }
    virtual u_int GetMemoryUsage() const { return m_xArray.GetMemoryUsage(); }
    virtual bool IsFlat() const { return false; }

    virtual T& operator []( const int iIndex )
    {
        return *( m_xArray[ iIndex ] );
    }

    virtual const T& operator []( const int iIndex ) const
    {
        return *( m_xArray[ iIndex ] );
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

    // array functions
    void Append( T* const pxValue ) { m_xArray.Append( pxValue ); }
    void InsertAt( const int iIndex, T* const pxValue ) { m_xArray.InsertAt( iIndex, pxValue ); }
    void RemoveAt( const int iIndex, const u_int uAmount = 1 ) { m_xArray.RemoveAt( iIndex, uAmount ); }
    void RemoveFromEnd( const u_int uAmount = 1 ) { m_xArray.RemoveFromEnd( uAmount ); }
    T& Start() { return *( m_xArray.Start() ); }
    T& End() { return *( m_xArray.End() ); }
    void Clear() { m_xArray.Clear(); }

    void DeleteAll()
    {
        for( u_int u = 0; u < GetCount(); ++u )
        {
            delete m_xArray[ u ];
        }

        m_xArray.Clear();
    }

    void DeleteAt( const int iIndex, const u_int uAmount = 1 )
    {
        for( int i = iIndex; i < iIndex + uAmount; ++i )
        {
            delete m_xArray[ i ];
        }

        m_xArray.RemoveAt( iIndex, uAmount );
    }
    
    void DeleteFromEnd( const u_int uAmount = 1 )
    {
        for( int i = GetCount() - uAmount - 1; i < GetCount(); ++i )
        {
            delete m_xArray[ i ];
        }
        
        m_xArray.RemoveFromEnd( uAmount );
    }

protected:

    virtual void CopyFrom( const GLToy_DataStructure< T >* const pxDataStructure )
    {
        DeleteAll();

        GLToy_ConstIterate( T, xIterator, pxDataStructure )
        {
            Append( new T( xIterator.Current() ) );
        }
    }

    GLToy_Array< T* > m_xArray;
};

template < class T >
class GLToy_SerialisableArray
: public GLToy_Array< T >
, public GLToy_Serialisable
{

public:
    
    virtual void ReadFromBitStream( const GLToy_BitStream& xStream )
    {
        Clear();

        u_int uCount;
        xStream >> uCount;

        for( u_int u = 0; u < uCount; ++u )
        {
            T xData;
            xStream >> xData;
            Append( xData );
        }
    }

    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const
    {
        xStream << GetCount();

        GLToy_ConstIterate( T, xIterator, this )
        {
            xStream << xIterator.Current();
        }
    }
};

template < class T >
class GLToy_SerialisableIndirectArray
: public GLToy_IndirectArray< T >
, public GLToy_Serialisable
{

public:
    
    template < class Derived >
    void ReadFromDerivedBitStream( const GLToy_BitStream& xStream )
    {
        DeleteAll();

        u_int uCount;
        xStream >> uCount;

        for( u_int u = 0; u < uCount; ++u )
        {
            Derived* pxData = new Derived();
            xStream >> pxData;
            Append( pxData );
        }
    }

    virtual void ReadFromBitStream( const GLToy_BitStream& xStream )
    {
        DeleteAll();

        u_int uCount;
        xStream >> uCount;

        for( u_int u = 0; u < uCount; ++u )
        {
            T* pxData = new T();
            xStream >> pxData;
            Append( pxData );
        }
    }

    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const
    {
        xStream << GetCount();

        GLToy_ConstIterate( T*, xIterator, &m_xArray )
        {
            xStream << xIterator.Current();
        }
    }
};

#endif