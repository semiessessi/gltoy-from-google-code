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

#ifndef __GLTOY_LIST_H_
#define __GLTOY_LIST_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/Data Structures/GLToy_DataStructure.h>
#include <Core/Data Structures/GLToy_Pool.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

template< class T >
class GLToy_List;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

// TODO - finish and test then implement GLToy_IndirectList< T > with GLToy_List< T* >

template< class T >
class GLToy_ListNode
: public GLToy_DataStructure< T >
, public GLToy_PoolAllocated< GLToy_ListNode< T > >
{
    friend class GLToy_List< T >;

    typedef GLToy_DataStructure< T > GLToy_Parent;

public:

    GLToy_ListNode()
    : GLToy_Parent()
    , m_xData()
    , m_pxTail( NULL )
    {
    }

    GLToy_ListNode( const T& xData )
    : GLToy_Parent()
    , m_xData( xData )
    , m_pxTail( NULL )
    {
    }

    GLToy_ListNode( const GLToy_ListNode& xList )
    : GLToy_Parent( xList )
    , m_xData()
    , m_pxTail( NULL )
    {
        CopyFrom( &xList );
    }

    virtual ~GLToy_ListNode()
    {
    }

    void DestroyList()
    {
        if( !m_pxTail )
        {
            return;
        }

        m_pxTail->DestroyList();
        delete m_pxTail;
    }

    T& operator []( const int iIndex )
    {
        if( iIndex == 0 )
        {
            return m_xData;
        }
        
        return m_pxTail->operator []( iIndex - 1 );
    }

    const T& operator []( const int iIndex ) const
    {
        if( iIndex == 0 )
        {
            return m_xData;
        }
        
        return m_pxTail->operator []( iIndex - 1 );
    }

    void Traverse( GLToy_Functor< T >& xFunctor )
    {
        xFunctor( &m_xData );
        if( m_pxTail )
        {
            m_pxTail->Traverse( xFunctor );
        }
    }

    void Traverse( GLToy_ConstFunctor< T >& xFunctor ) const 
    {
        xFunctor( &m_xData );
        if( m_pxTail )
        {
            m_pxTail->Traverse( xFunctor );
        }
    }

    virtual bool IsFlat() const { return false; }
    
    virtual u_int GetCount() const
    {
        if( !m_pxTail )
        {
            return 1;
        }
        
        return 1 + m_pxTail->GetCount();
    }

    virtual u_int GetMemoryUsage() const
    {
        return ( m_pxTail ? m_pxTail->GetMemoryUsage() : 0 ) + sizeof( this );
    }

    void Append( const T& xData )
    {
        if( !m_pxTail )
        {
            m_pxTail = new GLToy_ListNode( xData );
            return;
        }

        m_pxTail->Append( xData );
    }

protected:

    template < class DataStructure >
    void CopyFrom( const DataStructure* const pxDataStructure )
    {
        delete m_pxTail;
        m_pxTail = 0;
        
        if( pxDataStructure->IsEmpty() )
        {
            return;
        }

        m_xData = ( *pxDataStructure )[ 0 ];

        for( u_int u = 1; u < pxDataStructure->GetCount(); ++u )
        {
            Append( ( *pxDataStructure )[ u ] );
        }
    }

    T m_xData;
    GLToy_ListNode< T >* m_pxTail;

};

template< class T >
class GLToy_List
: public GLToy_DataStructure< T >
{

    typedef GLToy_DataStructure< T > GLToy_Parent;

public:

    GLToy_List()
    : GLToy_Parent()
    , m_pxData( NULL )
#ifdef GLTOY_DEBUG
    , dbg_uCount( 0 )
#endif
    {
    }

    GLToy_List( const T& xData )
    : GLToy_Parent()
    , m_pxData( new GLToy_ListNode< T >( xData ) )
#ifdef GLTOY_DEBUG
    , dbg_uCount( 1 )
#endif
    {
    }

    GLToy_List( const GLToy_List& xList )
    : GLToy_Parent( xList )
    , m_pxData( NULL )
#ifdef GLTOY_DEBUG
    , dbg_uCount( xList.dbg_uCount )
#endif
    {
        CopyFrom( &xList );
    }

    virtual ~GLToy_List()
    {
        Clear();
    }

    T& operator []( const int iIndex )
    {
        return ( *m_pxData )[ iIndex ];
    }

    const T& operator []( const int iIndex ) const
    {
        return ( *m_pxData )[ iIndex ];
    }

    void Traverse( GLToy_Functor< T >& xFunctor )
    {
        if( m_pxData )
        {
            m_pxData->Traverse( xFunctor );
        }
    }

    void Traverse( GLToy_ConstFunctor< T >& xFunctor ) const
    {
        if( m_pxData )
        {
            m_pxData->Traverse( xFunctor );
        }
    }

    virtual bool IsFlat() const { return false; }
    
    virtual u_int GetCount() const
    {
        if( !m_pxData )
        {
#ifdef GLTOY_DEBUG
            GLToy_Assert( dbg_uCount == 0, "List reporting empty but debug count is not zero!" );
#endif
            return 0;
        }
        
        return m_pxData->GetCount();
    }

    virtual u_int GetMemoryUsage() const
    {
        return m_pxData->GetMemoryUsage();
    }

    void Append( const T& xData )
    {
        if( !m_pxData )
        {
            m_pxData = new GLToy_ListNode< T >( xData );
        }
        else
        {
            m_pxData->Append( xData );
        }

#ifdef GLTOY_DEBUG
        ++dbg_uCount;
#endif
    }

    void RemoveAt( const u_int uIndex )
    {
        if( !m_pxData )
        {
            return;
        }

#ifdef GLTOY_DEBUG
        --dbg_uCount;
#endif

        if( uIndex == 0 )
        {
            GLToy_ListNode< T >* pxNode = m_pxData->m_pxTail;
            if( !pxNode )
            {
                m_pxData = NULL;
            }
            else
            {
                delete m_pxData;
                m_pxData = pxNode;
            }

            return;
        }

        GLToy_ListNode< T >* pxParentNode = &GetNode( uIndex - 1 );
        GLToy_ListNode< T >* pxNode = pxParentNode->m_pxTail;
        if( pxNode )
        {
            pxParentNode->m_pxTail = pxNode->m_pxTail;
            delete pxNode;
        }
    }

    void Clear()
    {
        if( !m_pxData )
        {
            return;
        }

        GLToy_ListNode< T >* pxNode = m_pxData->m_pxTail;
        const u_int uCount = m_pxData ? 0 : m_pxData->GetCount();
        for( u_int u = 1; u < uCount; ++u )
        {
            GLToy_ListNode< T >* pxDelete = pxNode;
            pxNode = pxDelete->m_pxTail;
            delete pxDelete;
        }

        delete m_pxData;
        m_pxData = NULL;
#ifdef GLTOY_DEBUG
        dbg_uCount = 0;
#endif
    }

    T& Head()
    {
        return ( *m_pxData )[ 0 ];
    }

    GLToy_List Tail()
    {
        GLToy_List xTail( *this );
        xTail.RemoveAt( 0 );
        return xTail;
    }

    void RemoveHead()
    {
        RemoveAt( 0 );
    }

protected:

    virtual GLToy_ListNode< T >& GetNode( const u_int uIndex )
    {
        GLToy_ListNode< T >* pxNode = m_pxData;
        for( u_int u = 1; u < uIndex; ++u )
        {
            pxNode = pxNode->m_pxTail;
        }

        return *pxNode;
    }

    virtual const GLToy_ListNode< T >& GetNode( const u_int uIndex ) const
    {
        const GLToy_ListNode< T >* pxNode = m_pxData;
        for( u_int u = 1; u < uIndex; ++u )
        {
            pxNode = pxNode->m_pxTail;
        }

        return *pxNode;
    }

    template < class DataStructure >
    void CopyFrom( const DataStructure* const pxDataStructure )
    {
        if( pxDataStructure->IsEmpty() )
        {
            m_pxData = NULL;
            return;
        }

        m_pxData = new GLToy_ListNode< T >();

#ifdef GLTOY_DEBUG
        dbg_uCount = pxDataStructure->GetCount();
#endif

        m_pxData->CopyFrom( pxDataStructure );
    }

    GLToy_ListNode< T >* m_pxData;

#ifdef GLTOY_DEBUG
    u_int dbg_uCount;
#endif

};

#endif
