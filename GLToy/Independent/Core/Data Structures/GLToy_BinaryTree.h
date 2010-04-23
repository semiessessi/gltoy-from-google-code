/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of GLToy.
//
// GLToy is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// GLToy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with GLToy.
// If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GLTOY_BINARYTREE_H_
#define __GLTOY_BINARYTREE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
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
{

    friend GLToy_BinaryTree< DataType, KeyType >;

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
            return ( m_pxHigher->GetKey() == xKey ) ? this : m_pxHigher->FindParentNode( xKey );
        }

        if( xKey < m_xKey )
        {
            return ( m_pxLower->GetKey() == xKey ) ? this : m_pxLower->FindParentNode( xKey );
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

public:

    GLToy_BinaryTree()
    : m_pxHead( NULL )
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

        public:

            AddFunctor( GLToy_BinaryTree* pxThis )
            : m_pxThis( pxThis )
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
        
        pxNode->TraverseNodes( AddFunctor( this ) );

        delete pxNode;
    }

protected:

    GLToy_BinaryTreeNode< DataType, KeyType >* m_pxHead;

};

#endif
