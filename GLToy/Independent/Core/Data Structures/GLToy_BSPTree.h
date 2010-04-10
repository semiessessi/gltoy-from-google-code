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

#ifndef __GLTOY_BSPTREE_H_
#define __GLTOY_BSPTREE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/Data Structures/GLToy_Tree.h>

// GLToy
#include <Maths/GLToy_Plane.h>
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
    friend class GLToy_EnvironmentFile;

public:

    GLToy_BSPNode()
    : m_pxPositive( NULL )
    , m_pxNegative( NULL )
    , m_xPlane()
    , m_pData( NULL )
    {
    }

    virtual ~GLToy_BSPNode()
    {
        delete m_pxPositive;
        delete m_pxNegative;
    }

    GLToy_BSPNode( const GLToy_Vector_3& xNormal, const float fDistance )
    : m_pxPositive( NULL )
    , m_pxNegative( NULL )
    , m_xPlane( xNormal, fDistance )
    , m_pData( NULL )
    {
    }

    GLToy_BSPNode( const GLToy_Vector_3& xPosition, T* const pData )
    : m_pxPositive( NULL )
    , m_pxNegative( NULL )
    , m_xPlane( xPosition, 0.0f )
    , m_pData( pData )
    {
    }

    GLToy_BSPNode( T* const pData )
    : m_pxPositive( NULL )
    , m_pxNegative( NULL )
    , m_xPlane()
    , m_pData( pData )
    {
    }

    GLToy_Inline bool IsOnPositiveSide( const GLToy_Vector_3& xPosition ) const
    {
        return m_xPlane.IsOnPositiveSide( xPosition );
    }

    GLToy_Inline double SignedDistance( const GLToy_Vector_3& xPosition ) const
    {
        return m_xPlane.SignedDistance( xPosition );
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

    const GLToy_BSPNode* FindLeaf( const GLToy_Vector_3& xPosition ) const
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
            m_pData = NULL;

            const GLToy_Vector_3 xMyPosition = m_xPlane.GetNormal();
            GLToy_Vector_3 xCentre = ( xPosition + xMyPosition ) * 0.5;
            GLToy_Vector_3 xNormal = xMyPosition - xPosition;
            xNormal.Normalise();
            m_xPlane = GLToy_Plane( xNormal, -( xCentre * xNormal ) );

            if( SignedDistance( xMyPosition ) < 0 )
            {
                m_pxPositive = new GLToy_BSPNode( xPosition, pData );
                m_pxNegative = new GLToy_BSPNode( xMyPosition, m_pData );

                return m_pxPositive;
            }

            m_pxNegative = new GLToy_BSPNode( xPosition, pData );
            m_pxPositive = new GLToy_BSPNode( xMyPosition, m_pData );

            return m_pxNegative;
        }

        return GetChild( xPosition )->AddLeaf( xPosition, pData );
    }

    virtual void SplitLeaf( const GLToy_Vector_3& xPosition, const GLToy_Plane& xSplitPlane, T* const pDataPositive, T* const pDataNegative )
    {
        if( !m_pData )
        {
            GetChild( xPosition )->SplitLeaf( xPosition, xSplitPlane, pDataPositive, pDataNegative );
        }

        m_xPlane = xSplitPlane;

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

    GLToy_Inline T* GetData() const { return m_pData; }

protected:

    GLToy_BSPNode* m_pxPositive;
    GLToy_BSPNode* m_pxNegative;
    GLToy_Plane m_xPlane;
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

    GLToy_Inline T* GetLeafData( const GLToy_Vector_3& xPosition ) const
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

    GLToy_Inline void SplitLeaf( const GLToy_Vector_3& xPosition, const GLToy_Plane& xSplitPlane, T* const pDataPositive, T* const pDataNegative )
    {
        if( !m_pxHead )
        {
            return;
        }

        m_pxHead->SplitLeaf( xPosition, xSplitPlane, pDataPositive, pDataNegative );;
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

    GLToy_Inline bool IsEmpty() const { return m_pxHead == NULL; }

    void SetToNodePointer( GLToy_BSPNode< T >* const pxNode )
    {
        delete m_pxHead;
        m_pxHead = pxNode;
    }

    virtual void Clear()
    {
        delete m_pxHead;
        m_pxHead = NULL;
    }

protected:

    GLToy_BSPNode< T >* m_pxHead;

};

#endif