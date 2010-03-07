#ifndef __GLTOY_BSPTREE_H_
#define __GLTOY_BSPTREE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/Data Structures/GLToy_Tree.h>

// GLToy
#include <Maths/GLToy_Vector.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class T > class GLToy_BSPTree;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class T >
class GLToy_BSPNode
{
    friend GLToy_BSPTree< T >;

public:

    GLToy_BSPNode()
    : m_pxPositive( NULL )
    , m_pxNegative( NULL )
    , m_xNormal( 0.0, 0.0, 0.0 )
    , m_fDistance( 0.0 )
    , m_pData( NULL )
    {
    }

    virtual ~GLToy_BSPNode()
    {
        delete m_pxPositive;
        delete m_pxNegative;
    }

protected:

    GLToy_BSPNode( const GLToy_Vector_3& xNormal, const float fDistance )
    : m_pxPositive( NULL )
    , m_pxNegative( NULL )
    , m_xNormal( xNormal )
    , m_fDistance( fDistance )
    , m_pData( NULL )
    {
    }

    GLToy_BSPNode( const GLToy_Vector_3& xPosition, T* const pData )
    : m_pxPositive( NULL )
    , m_pxNegative( NULL )
    , m_xNormal( xPosition )
    , m_fDistance( 0.0 )
    , m_pData( pData )
    {
    }

    GLToy_BSPNode( T* const pData )
    : m_pxPositive( NULL )
    , m_pxNegative( NULL )
    , m_xNormal( 0.0f, 0.0f, 0.0f )
    , m_fDistance( 0.0 )
    , m_pData( pData )
    {
    }

    GLToy_Inline bool IsOnPositiveSide( const GLToy_Vector_3& xPosition ) const
    {
        return xPosition * m_xNormal > -m_fDistance;
    }

    GLToy_Inline double SignedDistance( const GLToy_Vector_3& xPosition ) const
    {
        return xPosition * m_xNormal + m_fDistance;
    }

    GLToy_Inline GLToy_BSPNode* GetChild( const GLToy_Vector_3& xPosition )
    {
        if( SignedDistance( xPosition ) < 0 )
        {
            return m_pxNegative;
        }

        return m_pxPositive;
    }

    GLToy_BSPNode* FindLeaf( const GLToy_Vector_3& xPosition )
    {
        if( m_pData )
        {
            return this;
        }

        return GetChild( xPosition )->FindLeaf( xPosition );
    }

    virtual GLToy_BSPNode* AddLeaf( const GLToy_Vector_3& xPosition, T* const pData )
    {
        if( m_pData )
        {
            m_pData = false;

            const GLToy_Vector_3 xMyPosition = m_xNormal;
            GLToy_Vector_3 xCentre = ( xPosition + xMyPosition ) * 0.5;
            m_xNormal = xMyPosition - xPosition;
            m_xNormal.Normalise();
            m_fDistance = -( xCentre * m_xNormal );

            if( SignedDistance( xMyPosition ) < 0 )
            {
                m_pxPositive = new GLToy_BSPNode( xPosition, pData );
                m_pxNegative = new GLToy_BSPNode( xMyPosition, m_pData );
                m_pData = NULL;

                return m_pxPositive;
            }

            m_pxNegative = new GLToy_BSPNode( xPosition, pData );
            m_pxPositive = new GLToy_BSPNode( xMyPosition, m_pData );
            m_pData = NULL;

            return m_pxNegative;
        }

        return GetChild( xPosition )->AddLeaf( xPosition, pData );
    }

    virtual void SplitLeaf( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xNormal, const float fDistance, T* const pDataPositive, T* const pDataNegative )
    {
        if( !m_pData )
        {
            GetChild( xPosition )->SplitLeaf( xPosition, xNormal, fDistance, pDataPositive, pDataNegative );
        }

        m_xNormal = xNormal;
        m_fDistance = fDistance;

        m_pxPositive = new GLToy_BSPNode( pDataPositive );
        m_pxNegative = new GLToy_BSPNode( pDataNegative );
        m_pData = NULL;
    }

    void Traverse( GLToy_Functor< T >& xFunctor )
    {
        if( m_pData )
        {
            xFunctor( m_pData );
        }
        else
        {
            m_pxPositive->Traverse( xFunctor );
            m_pxNegative->Traverse( xFunctor );
        }
    }

    void Traverse( GLToy_ConstFunctor< T >& xFunctor ) const
    {
        if( m_pData )
        {
            xFunctor( m_pData );

        }
        else
        {
            m_pxPositive->Traverse( xFunctor );
            m_pxNegative->Traverse( xFunctor );
        }
    }

    void DistanceSortedLeafTraverse( GLToy_Functor< T >& xFunctor, const GLToy_Vector_3& xPosition )
    {
        if( m_pData )
        {
            xFunctor( m_pData );

        }
        else
        {
            if( IsOnPositiveSide( xPosition ) )
            {
                m_pxPositive->DistanceSortedLeafTraverse( xFunctor, xPosition );
                m_pxNegative->DistanceSortedLeafTraverse( xFunctor, xPosition );
            }
            else
            {
                m_pxNegative->DistanceSortedLeafTraverse( xFunctor, xPosition );
                m_pxPositive->DistanceSortedLeafTraverse( xFunctor, xPosition );
            }
        }
    }

    void DistanceSortedLeafTraverse( GLToy_ConstFunctor< T >& xFunctor, const GLToy_Vector_3& xPosition ) const
    {
        if( m_pData )
        {
            xFunctor( m_pData );

        }
        else
        {
            if( IsOnPositiveSide( xPosition ) )
            {
                m_pxPositive->DistanceSortedLeafTraverse( xFunctor, xPosition );
                m_pxNegative->DistanceSortedLeafTraverse( xFunctor, xPosition );
            }
            else
            {
                m_pxNegative->DistanceSortedLeafTraverse( xFunctor, xPosition );
                m_pxPositive->DistanceSortedLeafTraverse( xFunctor, xPosition );
            }
        }
    }

    void ReverseDistanceSortedLeafTraverse( GLToy_Functor< T >& xFunctor, const GLToy_Vector_3& xPosition )
    {
        if( m_pData )
        {
            xFunctor( m_pData );

        }
        else
        {
            if( IsOnPositiveSide( xPosition ) )
            {
                m_pxNegative->ReverseDistanceSortedLeafTraverse( xFunctor, xPosition );
                m_pxPositive->ReverseDistanceSortedLeafTraverse( xFunctor, xPosition );
            }
            else
            {
                m_pxPositive->ReverseDistanceSortedLeafTraverse( xFunctor, xPosition );
                m_pxNegative->ReverseDistanceSortedLeafTraverse( xFunctor, xPosition );
            }
        }
    }

    void ReverseDistanceSortedLeafTraverse( GLToy_ConstFunctor< T >& xFunctor, const GLToy_Vector_3& xPosition ) const
    {
        if( m_pData )
        {
            xFunctor( m_pData );

        }
        else
        {
            if( IsOnPositiveSide( xPosition ) )
            {
                m_pxNegative->ReverseDistanceSortedLeafTraverse( xFunctor, xPosition );
                m_pxPositive->ReverseDistanceSortedLeafTraverse( xFunctor, xPosition );
            }
            else
            {
                m_pxPositive->ReverseDistanceSortedLeafTraverse( xFunctor, xPosition );
                m_pxNegative->ReverseDistanceSortedLeafTraverse( xFunctor, xPosition );
            }
        }
    }

    GLToy_Inline T* GetData() { return m_pData; }
    GLToy_Inline const T* GetData() const { return m_pData; }

    GLToy_BSPNode* m_pxPositive;
    GLToy_BSPNode* m_pxNegative;
    GLToy_Vector_3 m_xNormal;
    float m_fDistance;
    T* m_pData;
};

template < class T >
class GLToy_BSPTree
: public GLToy_Tree< T >
{

public:

    GLToy_BSPTree()
    : m_pxHead( NULL )
    {
    }

    virtual ~GLToy_BSPTree()
    {
        delete m_pxHead;
    }

    GLToy_Inline static GLToy_BSPTree< T >* CreateBSPTree( T* pData, const unsigned int uCount )
    {
        GLToy_BSPTree< T >* pxTree = new GLToy_BSPTree< T >();

        for( unsigned int u = 0; u < uCount; ++u )
        {
            pxTree->AddLeaf( pData[ u ].GetPosition(), &( pData[ u ] ) );
        }

        return pxTree;
    }

    GLToy_Inline T* GetLeafData( const GLToy_Vector_3& xPosition )
    {
        return m_pxHead
            ? m_pxHead->FindLeaf( xPosition )->GetData()
            : 0;
    }

    GLToy_Inline void AddLeaf( const GLToy_Vector_3& xPosition, T* const pData )
    {
        if( m_pxHead )
        {
            m_pxHead->AddLeaf( xPosition, pData );
            return;
        }

        m_pxHead = new GLToy_BSPNode< T >( xPosition, pData );
    }

    GLToy_Inline void SplitLeaf( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xNormal, const float fDistance, T* const pDataPositive, T* const pDataNegative )
    {
        if( !m_pxHead )
        {
            return;
        }

        m_pxHead->SplitLeaf( xPosition, xNormal, fDistance, pDataPositive, pDataNegative );;
    }

    virtual void Traverse( GLToy_Functor< T >& xFunctor )
    {
        if( m_pxHead )
        {
            m_pxHead->Traverse( xFunctor );
        }
    }

    virtual void Traverse( GLToy_ConstFunctor< T >& xFunctor ) const
    {
        if( m_pxHead )
        {
            m_pxHead->Traverse( xFunctor );
        }
    }

    GLToy_Inline void DistanceSortedLeafTraverse( GLToy_Functor< T >& xFunctor, const GLToy_Vector_3& xPosition )
    {
        if( m_pxHead )
        {
            m_pxHead->DistanceSortedLeafTraverse( xFunctor, xPosition );
        }
    }

    GLToy_Inline void DistanceSortedLeafTraverse( GLToy_ConstFunctor< T >& xFunctor, const GLToy_Vector_3& xPosition ) const
    {
        if( m_pxHead )
        {
            m_pxHead->DistanceSortedLeafTraverse( xFunctor, xPosition );
        }
    }

    GLToy_Inline void ReverseDistanceSortedLeafTraverse( GLToy_Functor< T >& xFunctor, const GLToy_Vector_3& xPosition )
    {
        if( m_pxHead )
        {
            m_pxHead->ReverseDistanceSortedLeafTraverse( xFunctor, xPosition );
        }
    }

    GLToy_Inline void ReverseDistanceSortedLeafTraverse( GLToy_ConstFunctor< T >& xFunctor, const GLToy_Vector_3& xPosition ) const
    {
        if( m_pxHead )
        {
            m_pxHead->ReverseDistanceSortedLeafTraverse( xFunctor, xPosition );
        }
    }

private:

    GLToy_BSPNode< T >* m_pxHead;
};

#endif