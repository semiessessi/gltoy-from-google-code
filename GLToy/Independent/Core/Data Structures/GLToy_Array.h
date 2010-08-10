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

#ifndef __GLTOY_ARRAY_H_
#define __GLTOY_ARRAY_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/Data Structures/GLToy_FlatDataStructure.h>
#include <Core/GLToy_Serialisable.h>

// C/C++
#include <new> 

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
    , m_uCount( 0 ) // CopyFrom requires this
    {
        CopyFrom( &xArray );
        m_uCount = xArray.m_uCount;
    }
    
    GLToy_Array& operator =( const GLToy_DataStructure< T >& xDataStructure )
    {
        CopyFrom( &xDataStructure );
        m_uCount = xDataStructure.GetCount();

        return *this;
    }

    virtual ~GLToy_Array()
    {
        GLToy_Iterate( T, xIterator, this )
        {
            xIterator.Current().~T();
        }
    }
    
    virtual void Append( const T& xValue )
    {
        // increment m_uCount then make sure memory is allocated before setting the new entry
        ++m_uCount;
        CheckAlloc( m_uCount );
        new ( &( GLToy_Parent::m_pxData[ m_uCount - 1 ] ) ) T( xValue );
    }
    
    virtual void Append( const GLToy_Array& xValues )
    {
        // store m_uCount for later use when copying
        const u_int uOldCount = m_uCount;

        // increase m_uCount and allocate accordingly
        m_uCount += xValues.m_uCount;
        CheckAlloc( m_uCount );

        for( u_int i = 0; i < xValues.m_uCount; ++i )
        {
            new ( &( GLToy_Parent::m_pxData[ uOldCount + i ] ) ) T( xValues.GLToy_Parent::m_pxData[ i ] );
        }
    }
    
    virtual void InsertAt( const int iIndex, const T& xValue )
    {
        // increase m_uCount and check allocation
        ++m_uCount;
        CheckAlloc( m_uCount );

        // make sure to construct the new entry
        new ( &( GLToy_Parent::m_pxData[ m_uCount - 1 ] ) ) T( GLToy_Parent::m_pxData[ m_uCount - 2 ] );

        // move the existing xValues along to make room
        for( int i = m_uCount - 2; i > iIndex; --i )
        {
            GLToy_Parent::m_pxData[ i ] = GLToy_Parent::m_pxData[ i - 1 ];
        }

        // set the new value
        GLToy_Parent::m_pxData[ iIndex ] = xValue;
    }
    
    virtual void RemoveAt( const int iIndex, const u_int uAmount = 1 )
    {
        u_int uActualAmount = uAmount;
        if( uAmount > m_uCount )
        {
            uActualAmount = m_uCount;
        }

        m_uCount -= uActualAmount;
        for( u_int i = iIndex; i < m_uCount; ++i )
        {
            GLToy_Parent::m_pxData[ i ] = GLToy_Parent::m_pxData[ i + uActualAmount ];
        }

        // be careful to destroy the now unused entries
        for( u_int u = m_uCount; u < m_uCount + uActualAmount; ++u )
        {
            GLToy_Parent::m_pxData[ u ].~T();
        }
    }
    
    virtual void RemoveFromEnd( const u_int uAmount = 1 )
    {
        u_int uActualAmount = uAmount;
        if( uAmount > m_uCount )
        {
            uActualAmount = m_uCount;
        }

        m_uCount -= uActualAmount;

        // be careful to destroy the now unused entries
        for( u_int u = m_uCount; u < m_uCount + uActualAmount; ++u )
        {
            GLToy_Parent::m_pxData[ u ].~T();
        }
    }

    virtual void Resize( const u_int uCount )
    {
        if( uCount < m_uCount )
        {
            for( u_int u = uCount; u < m_uCount; ++u )
            {
                GLToy_Parent::m_pxData[ u ].~T();
            }
        }
        else
        {
            CheckAlloc( uCount );
            for( u_int u = m_uCount; u < uCount; ++u )
            {
                new ( &( GLToy_Parent::m_pxData[ u ] ) ) T;
            }
        }

        m_uCount = uCount;
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
        return GLToy_Parent::m_pxData[ 0 ];
    }
    
    T& End()
    {
        return GLToy_Parent::m_pxData[ m_uCount - 1 ];
    }
    
    void Clear()
    {
        // be careful to destroy the now unused entries
        for( u_int u = 0; u < m_uCount; ++u )
        {
            GLToy_Parent::m_pxData[ u ].~T();
        }

        m_uCount = 0;
    }

    void DeleteAll()
    {
        GLToy_Iterate( T, xIterator, this )
        {
            delete xIterator.Current();
        }

        Clear();
    }

protected:

    u_int m_uCount;

};

template< class T >
class GLToy_IndirectArray
: public GLToy_DataStructure< T >
{

	typedef GLToy_DataStructure< T > GLToy_Parent;

public:

	GLToy_IndirectArray()
	: GLToy_Parent()
	{
	}

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
        GLToy_Iterate( T*, xIterator, &m_xArray )
        {
            xFunctor( xIterator.Current() );
        }
    }

    virtual void Traverse( GLToy_ConstFunctor< T >& xFunctor ) const
    {
        GLToy_ConstIterate( T*, xIterator, &m_xArray )
        {
            xFunctor( xIterator.Current() );
        }
    }

    virtual void Traverse( GLToy_Functor< T* >& xFunctor )
    {
        GLToy_Iterate( T*, xIterator, &m_xArray )
        {
            xFunctor( &( xIterator.Current() ) );
        }
    }

    virtual void Traverse( GLToy_ConstFunctor< T* >& xFunctor ) const
    {
        GLToy_ConstIterate( T*, xIterator, &m_xArray )
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

// TODO: it would be nice to protect this...
public:

    GLToy_Array< T* > m_xArray;
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
    u_int m_uCount;

};

template< class T >
class GLToy_ConstPointerArray
: public GLToy_DataStructure< T >
{

public:

    GLToy_ConstPointerArray( const T* const pxData, const u_int uCount )
    : m_pxData( pxData )
    , m_uCount( uCount )
    {
    }

    GLToy_ConstPointerArray( const GLToy_ConstPointerArray& xDataStructure )
    : m_pxData( xDataStructure.m_pxData )
    , m_uCount( xDataStructure.m_uCount )
    {
    }

    virtual u_int GetCount() const { return m_uCount; }
    virtual u_int GetMemoryUsage() const { return sizeof( GLToy_ConstPointerArray ); }

    virtual bool IsFlat() const { return false; }

    virtual T& operator []( const int iIndex )  { static T ls_Dummy; return ls_Dummy; }
    virtual const T& operator []( const int iIndex ) const  { return m_pxData[ iIndex ]; }

    virtual void Traverse( GLToy_Functor< T >& xFunctor ) {}

    virtual void Traverse( GLToy_ConstFunctor< T >& xFunctor ) const
    {
        GLToy_ConstIterate( T, xIterator, this )
        {
            xFunctor( &( xIterator.Current() ) );
        }
    }

protected:

    virtual void CopyFrom( const GLToy_DataStructure< T >* const pxDataStructure ) {}

    const T* m_pxData;
    u_int m_uCount;

};

template < class T >
class GLToy_SerialisableArray
: public GLToy_Array< T >
, public GLToy_Serialisable
{

    typedef GLToy_Array< T > GLToy_Parent;

public:

    GLToy_SerialisableArray()
    : GLToy_Parent()
    {
    }
    
    GLToy_SerialisableArray( const GLToy_Array< T >& xArray )
    : GLToy_Parent( xArray )
    {
        CopyFrom( &xArray );
        GLToy_Parent::m_uCount = xArray.m_uCount;
    }
    
    GLToy_SerialisableArray& operator =( const GLToy_DataStructure< T >& xDataStructure )
    {
        CopyFrom( &xDataStructure );
        GLToy_Parent::m_uCount = xDataStructure.GetCount();

        return *this;
    }

    virtual ~GLToy_SerialisableArray()
    {
        GLToy_Iterate( T, xIterator, this )
        {
            xIterator.Current().~T();
        }
    }
    
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

    typedef GLToy_IndirectArray< T > GLToy_Parent;

public:

    GLToy_SerialisableIndirectArray()
    : GLToy_Parent()
    , GLToy_Serialisable()
    {
    }

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
            T* pxData = new T;
            xStream >> pxData;
            Append( pxData );
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

#endif
