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
                m_pxLower->AddNode( xData, xKey );
            }
            else
            {
                m_pxLower = new GLToy_BinaryTreeNode( xData, xKey );
            }
        }
    }

    GLToy_BinaryTreeNode* FindNode( const KeyType xKey )
    {
        if( xKey > m_xKey )
        {
            return m_pxHigher ? m_pxHigher->FindNode( xKey ) : NULL;
        }

        if( xKey < m_xKey )
        {
            return m_pxLower ? m_pxLower->FindNode( xKey ) : NULL;
        }

        return this;
    }

    virtual void Traverse( GLToy_Functor< DataType >& xFunctor )
    {
        xFunctor( &m_xData );

        if( m_pxLower )
        {
            m_pxLower->Traverse( xFunctor );
        }

        if( m_pxHigher )
        {
            m_pxHigher->Traverse( xFunctor );
        }
    }

    virtual void Traverse( GLToy_ConstFunctor< DataType >& xFunctor ) const
    {
        xFunctor( &m_xData );

        if( m_pxLower )
        {
            m_pxLower->Traverse( xFunctor );
        }

        if( m_pxHigher )
        {
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

    void AddNode( const DataType& xData, const KeyType xKey )
    {
        if( m_pxHead )
        {
            m_pxHead->AddNode( xData, xKey );
        }
        else
        {
            m_pxHead = new GLToy_BinaryTreeNode( xData, xKey );
        }
    }

    DataType* FindData( const KeyType xKey )
    {
        if( !m_pxHead )
        {
            return NULL;
        }

        return m_pxHead->FindNode( xKey )->GetDataPointer();
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
    
    void Clear()
    {
        delete m_pxHead;
        m_pxHead = NULL;
    }

protected:

    GLToy_BinaryTreeNode< DataType, KeyType >* m_pxHead;

};

#endif
