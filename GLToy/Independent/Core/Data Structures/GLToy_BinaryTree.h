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

#ifndef __GLTOY_BINARYTREE_H_
#define __GLTOY_BINARYTREE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/Data Structures/GLToy_Pool.h>
#include <Core/Data Structures/GLToy_Tree.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class DataType, class KeyType > class GLToy_BinaryTree;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class DataType, class KeyType >
class GLToy_BinaryTreeNode
: public GLToy_PoolAllocated< GLToy_BinaryTreeNode< DataType, KeyType > >
{

    friend class GLToy_BinaryTree< DataType, KeyType >;

public:

    GLToy_BinaryTreeNode( const DataType& xData, const KeyType xKey )
    : m_pxHigher( NULL )
    , m_pxLower( NULL )
    , m_xData( xData )
    , m_xKey( xKey )
    {
    }

    virtual ~GLToy_BinaryTreeNode()
    {
        delete m_pxHigher;
        delete m_pxLower;
    }

protected:

    void AddNode( const DataType& xData, const KeyType xKey )
    {
        if( xKey > m_xKey )
        {
            if( m_pxHigher )
            {
                GLToy_Assert( m_pxHigher != this, "Get ready for a stack overflow, we have a loop in our tree!" );
                m_pxHigher->AddNode( xData, xKey );
            }
            else
            {
                m_pxHigher = new GLToy_BinaryTreeNode( xData, xKey );
            }
        }
        else
        {
            if( m_pxLower )
            {
                GLToy_Assert( m_pxLower != this, "Get ready for a stack overflow, we have a loop in our tree!" );
                m_pxLower->AddNode( xData, xKey );
            }
            else
            {
                m_pxLower = new GLToy_BinaryTreeNode( xData, xKey );
            }
        }
    }

    GLToy_BinaryTreeNode* FindParentNode( const KeyType xKey )
    {
        if( xKey > m_xKey )
        {
            if( m_pxHigher )
            {
                return ( m_pxHigher->GetKey() == xKey ) ? this : m_pxHigher->FindParentNode( xKey );
            }
            else
            {
                return NULL;
            }
        }

        if( xKey < m_xKey )
        {
            if( m_pxLower )
            {
                return ( m_pxLower->GetKey() == xKey ) ? this : m_pxLower->FindParentNode( xKey );
            }
            else
            {
                return NULL;
            }
        }

        return NULL;
    }

    GLToy_BinaryTreeNode* FindNode( const KeyType xKey )
    {
        if( xKey > m_xKey )
        {
            GLToy_Assert( m_pxHigher != this, "Get ready for a stack overflow, we have a loop in our tree!" );
            return m_pxHigher ? m_pxHigher->FindNode( xKey ) : NULL;
        }

        if( xKey < m_xKey )
        {
            GLToy_Assert( m_pxLower != this, "Get ready for a stack overflow, we have a loop in our tree!" );
            return m_pxLower ? m_pxLower->FindNode( xKey ) : NULL;
        }

        return this;
    }

    const GLToy_BinaryTreeNode* FindNode( const KeyType xKey ) const
    {
        if( xKey > m_xKey )
        {
            GLToy_Assert( m_pxHigher != this, "Get ready for a stack overflow, we have a loop in our tree!" );
            return m_pxHigher ? m_pxHigher->FindNode( xKey ) : NULL;
        }

        if( xKey < m_xKey )
        {
            GLToy_Assert( m_pxLower != this, "Get ready for a stack overflow, we have a loop in our tree!" );
            return m_pxLower ? m_pxLower->FindNode( xKey ) : NULL;
        }

        return this;
    }

    virtual void Traverse( GLToy_Functor< DataType >& xFunctor )
    {
        if( m_pxLower )
        {
            GLToy_Assert( m_pxLower != this, "Get ready for a stack overflow, we have a loop in our tree!" );
            m_pxLower->Traverse( xFunctor );
        }

        xFunctor( &m_xData );

        if( m_pxHigher )
        {
            GLToy_Assert( m_pxHigher != this, "Get ready for a stack overflow, we have a loop in our tree!" );
            m_pxHigher->Traverse( xFunctor );
        }
    }

    virtual void TraverseNodes( GLToy_Functor< GLToy_BinaryTreeNode >& xFunctor )
    {
        if( m_pxLower )
        {
            xFunctor( m_pxLower );
            m_pxLower->TraverseNodes( xFunctor );
        }

        if( m_pxHigher )
        {
            xFunctor( m_pxHigher );
            m_pxHigher->TraverseNodes( xFunctor );
        }
    }

    virtual void Traverse( GLToy_ConstFunctor< DataType >& xFunctor ) const
    {
        if( m_pxLower )
        {
            GLToy_Assert( m_pxLower != this, "Get ready for a stack overflow, we have a loop in our tree!" );
            m_pxLower->Traverse( xFunctor );
        }

        xFunctor( &m_xData );

        if( m_pxHigher )
        {
            GLToy_Assert( m_pxHigher != this, "Get ready for a stack overflow, we have a loop in our tree!" );
            m_pxHigher->Traverse( xFunctor );
        }
    }

    KeyType GetKey() const { return m_xKey; }
    DataType& GetData() { return m_xData; }
    const DataType& GetData() const { return m_xData; }
    DataType* GetDataPointer() { return &m_xData; }
    const DataType* GetDataPointer() const { return &m_xData; }

    GLToy_BinaryTreeNode* m_pxHigher;
    GLToy_BinaryTreeNode* m_pxLower;
    DataType m_xData;
    KeyType m_xKey;

};

template < class DataType, class KeyType >
class GLToy_BinaryTree
: public GLToy_Tree< DataType >
{
    
    typedef GLToy_Tree< DataType > GLToy_Parent;

public:

    GLToy_BinaryTree()
    : GLToy_Parent()
    , m_pxHead( NULL )
    {
    }

    virtual ~GLToy_BinaryTree()
    {
        delete m_pxHead;
    }

    virtual void AddNode( const DataType& xData, const KeyType xKey )
    {
        if( m_pxHead )
        {
            m_pxHead->AddNode( xData, xKey );
        }
        else
        {
            m_pxHead = new GLToy_BinaryTreeNode< DataType, KeyType >( xData, xKey );
        }
    }

    virtual DataType* FindData( const KeyType xKey )
    {
        if( !m_pxHead )
        {
            return NULL;
        }

        GLToy_BinaryTreeNode< DataType, KeyType >* pxNode = m_pxHead->FindNode( xKey );

        return pxNode ? pxNode->GetDataPointer() : NULL;
    }

    virtual const DataType* FindData( const KeyType xKey ) const
    {
        if( !m_pxHead )
        {
            return NULL;
        }

        GLToy_BinaryTreeNode< DataType, KeyType >* pxNode = m_pxHead->FindNode( xKey );

        return pxNode ? pxNode->GetDataPointer() : NULL;
    }

    virtual void Traverse( GLToy_Functor< DataType >& xFunctor )
    {
        if( m_pxHead )
        {
            m_pxHead->Traverse( xFunctor );
        }
    }

    virtual void Traverse( GLToy_ConstFunctor< DataType >& xFunctor ) const
    {
        if( m_pxHead )
        {
            m_pxHead->Traverse( xFunctor );
        }
    }
    
    virtual void Clear()
    {
        delete m_pxHead;
        m_pxHead = NULL;
    }

    virtual void Remove( const KeyType xKey )
    {
        if( !m_pxHead )
        {
            return;
        }

        GLToy_BinaryTreeNode< DataType, KeyType >* pxNode = NULL;
        GLToy_BinaryTreeNode< DataType, KeyType >* pxParent = m_pxHead->FindParentNode( xKey );

        if( xKey == m_pxHead->GetKey() )
        {
            pxNode = m_pxHead;
        }
        else if( pxParent && pxParent->m_pxLower && ( pxParent->m_pxLower->GetKey() == xKey ) )
        {
            pxNode = pxParent->m_pxLower;
            pxParent->m_pxLower = NULL;
        }
        else if( pxParent && pxParent->m_pxHigher && ( pxParent->m_pxHigher->GetKey() == xKey ) )
        {
            pxNode = pxParent->m_pxHigher;
            pxParent->m_pxHigher = NULL;
        }

        if( !pxNode )
        {
            return;
        }

        class AddFunctor
        : public GLToy_Functor< GLToy_BinaryTreeNode< DataType, KeyType > >
        {

            typedef GLToy_Functor< GLToy_BinaryTreeNode< DataType, KeyType > > GLToy_Parent;

        public:

            AddFunctor( GLToy_BinaryTree* pxThis )
            : GLToy_Parent()
            , m_pxThis( pxThis )
            {
            }

            virtual void operator()( GLToy_BinaryTreeNode< DataType, KeyType >* const pxNode )
            {
                m_pxThis->AddNode( pxNode->GetData(), pxNode->GetKey() );
            }

        private:

            GLToy_BinaryTree* m_pxThis;

        };

        if( pxNode == m_pxHead )
        {
            m_pxHead = NULL;
        }
        

        AddFunctor xAddFunctor( this );
        pxNode->TraverseNodes( xAddFunctor );

        delete pxNode;
    }

protected:

    GLToy_BinaryTreeNode< DataType, KeyType >* m_pxHead;

};

#endif
