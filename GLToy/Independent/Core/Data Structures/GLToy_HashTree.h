#ifndef __GLTOY_HASHTREE_H_
#define __GLTOY_HASHTREE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// GLToy
#include <Core/GLToy_Hash.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
class GLToy_HashTree
{

    template<class T>
    struct Node
    {

        GLToy_Hash m_uHash;
        T m_xData;
        Node<T>* m_pxHigher;
        Node<T>* m_pxLower;

        Node()
        : m_uHash( uGLTOY_BAD_HASH )
        , m_xData()
        , m_pxHigher( NULL )
        , m_pxLower( NULL )
        {
        }

    };

public:
    
    GLToy_HashTree( u_int uInitialAllocCount = 16 )
    : m_pxRootNode( NULL )
    , m_uNodeCount( 0 )
    , m_uAllocCount( uInitialAllocCount )
    , m_uInitialAllocCount( uInitialAllocCount )
    {
        m_pxRootNode = new Node<T>[m_uAllocCount]();
    }

    virtual ~GLToy_HashTree()
    {
        delete[] m_pxRootNode;
    }

    void AddNode( const GLToy_Hash uHash, const T& xItem )
    {
        CheckAlloc();

        m_pxRootNode[m_uNodeCount].m_uHash = uHash;
        m_pxRootNode[m_uNodeCount].m_xData = xItem;

        if( m_uNodeCount == 0 )
        {
            m_pxRootNode[m_uNodeCount].m_pxHigher = NULL;
            m_pxRootNode[m_uNodeCount].m_pxLower = NULL;
            ++m_uNodeCount;
            return;
        }

        // find node to add as child
        Node<T>* pxParent = m_pxRootNode;
        while( true )
        {
            if( !pxParent->m_pxHigher && uHash > pxParent->m_uHash )
            {
                pxParent->m_pxHigher = &(m_pxRootNode[m_uNodeCount]);
                break;
            }
            else if( !pxParent->m_pxLower && uHash < pxParent->m_uHash )
            {
                pxParent->m_pxLower = &(m_pxRootNode[m_uNodeCount]);
                break;
            }

            if( uHash > pxParent->m_uHash )
            {
                pxParent = pxParent->m_pxHigher;
            }
            else
            {
                pxParent = pxParent->m_pxLower;
            }
        };

        ++m_uNodeCount;
    }

protected:

    void CheckAlloc()
    {
        if( m_uNodeCount + 1 > m_uAllocCount )
        {
            m_uAllocCount <<= 1;

            Node<T>* pxOldNodes = m_pxRootNode;
            m_pxRootNode = new Node<T>[m_uAllocCount];

            for( u_int u = 0; u < m_uNodeCount; ++u )
            {
                m_pxRootNode[u] = pxOldNodes[u];
            }

            delete pxOldNodes;
        }
    }

    Node<T>* m_pxRootNode;
    u_int m_uNodeCount;
    u_int m_uAllocCount;
    u_int m_uInitialAllocCount;
};

#endif
