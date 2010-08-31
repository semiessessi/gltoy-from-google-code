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

    virtual T& operator []( const int iIndex )
    {
        if( iIndex == 0 )
        {
            return m_xData;
        }
        
        return m_pxTail->operator []( iIndex - 1 );
    }

    virtual const T& operator []( const int iIndex ) const
    {
        if( iIndex == 0 )
        {
            return m_xData;
        }
        
        return m_pxTail->operator []( iIndex - 1 );
    }

    virtual void Traverse( GLToy_Functor< T >& xFunctor )
    {
        xFunctor( &m_xData );
        if( m_pxTail )
        {
            m_pxTail->Traverse( xFunctor );
        }
    }

    virtual void Traverse( GLToy_ConstFunctor< T >& xFunctor ) const 
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

    virtual void CopyFrom( const GLToy_DataStructure< T >* const pxDataStructure )
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
    , m_bEmpty( true )
    , m_xData()
    {
    }

    GLToy_List( const T& xData )
    : GLToy_Parent()
    , m_bEmpty( false )
    , m_xData( xData )
    {
    }

    GLToy_List( const GLToy_List& xList )
    : GLToy_Parent( xList )
    , m_bEmpty( xList.m_bEmpty )
    , m_xData( xList.m_xData )
    {
        CopyFrom( &xList );
    }

    virtual ~GLToy_List()
    {
        if( !m_bEmpty )
        {
            m_xData.DestroyList();
        }
    }

    virtual T& operator []( const int iIndex )
    {
        return m_xData[ iIndex ];
    }

    virtual const T& operator []( const int iIndex ) const
    {
        return m_xData[ iIndex ];
    }

    virtual void Traverse( GLToy_Functor< T >& xFunctor )
    {
        if( !m_bEmpty )
        {
            m_xData.Traverse( xFunctor );
        }
    }

    virtual void Traverse( GLToy_ConstFunctor< T >& xFunctor ) const
    {
        if( !m_bEmpty )
        {
            m_xData.Traverse( xFunctor );
        }
    }

    virtual bool IsFlat() const { return false; }
    
    virtual u_int GetCount() const
    {
        if( m_bEmpty )
        {
            return 0;
        }
        
        return m_xData.GetCount();
    }

    virtual u_int GetMemoryUsage() const
    {
        return m_xData.GetMemoryUsage();
    }

    void Append( const T& xData )
    {
        if( m_bEmpty )
        {
            m_xData[ 0 ] = xData;
        }
        else
        {
            m_xData.Append( xData );
        }
        m_bEmpty = false;
    }

    void RemoveAt( const u_int uIndex )
    {
        if( m_bEmpty )
        {
            return;
        }

        if( uIndex == 0 )
        {
            GLToy_ListNode< T >* pxNode = m_xData.m_pxTail;
            if( !pxNode )
            {
                m_bEmpty = true;
            }
            else
            {
                m_xData = *pxNode;
                delete pxNode;
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
        GLToy_ListNode< T >* pxNode = m_xData.m_pxTail;
        const u_int uCount = m_bEmpty ? 0 : m_xData.GetCount();
        for( u_int u = 1; u < uCount; ++u )
        {
            GLToy_ListNode< T >* pxDelete = pxNode;
            pxNode = pxDelete->m_pxTail;
            delete pxDelete;
        }

        m_bEmpty = true;
    }

    T& Head()
    {
        return m_xData[ 0 ];
    }

    GLToy_List Tail()
    {
        GLToy_List xTail;
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
        GLToy_ListNode< T >* pxNode = &m_xData;
        for( u_int u = 1; u < uIndex; ++u )
        {
            pxNode = pxNode->m_pxTail;
        }

        return *pxNode;
    }

    virtual const GLToy_ListNode< T >& GetNode( const u_int uIndex ) const
    {
        const GLToy_ListNode< T >* pxNode = &m_xData;
        for( u_int u = 1; u < uIndex; ++u )
        {
            pxNode = pxNode->m_pxTail;
        }

        return *pxNode;
    }

    virtual void CopyFrom( const GLToy_DataStructure< T >* const pxDataStructure )
    {
        m_bEmpty = pxDataStructure->IsEmpty();
        if( m_bEmpty )
        {
            m_xData = GLToy_ListNode< T >();
            return;
        }

        m_xData.CopyFrom( pxDataStructure );
    }

    bool m_bEmpty;
    GLToy_ListNode< T > m_xData;

};

#endif
