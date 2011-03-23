/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010, 2011 Semi Essessi
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
#include <Maths/GLToy_Volume.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class T > class GLToy_BSPTree;
class GLToy_EnvironmentFile;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class T >
class GLToy_BSPNode
{
    friend class GLToy_BSPTree< T >;
    friend class GLToy_EnvironmentFile;

public:

    GLToy_BSPNode()
    : m_pxPositive( NULL )
    , m_pxNegative( NULL )
    , m_xPlane()
    , m_pData( NULL )
    {
    }

    ~GLToy_BSPNode()
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

    GLToy_ForceInline bool IsOnPositiveSide( const GLToy_Vector_3& xPosition ) const
    {
        return m_xPlane.IsOnPositiveSide( xPosition );
    }

    GLToy_ForceInline double SignedDistance( const GLToy_Vector_3& xPosition ) const
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

    const void FindLeavesTouchingSphere( const GLToy_Sphere& xSphere, GLToy_Array< T* >& xResults ) const
    {
        if( m_pData )
        {
            xResults.Append( m_pData );
            return;
        }

        const bool bNegative = SignedDistance( xSphere.GetPosition() ) < xSphere.GetRadius();
        const bool bPositive = SignedDistance( xSphere.GetPosition() ) > -( xSphere.GetRadius() );

        if( bNegative )
        {
            pxNegative->FindLeavesTouchingSphere( xSphere, xResults );
        }

        if( bPositive )
        {
            pxPositive->FindLeavesTouchingSphere( xSphere, xResults );
        }
    }

    GLToy_BSPNode* AddLeaf( const GLToy_Vector_3& xPosition, T* const pData )
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

    GLToy_ForceInline GLToy_BSPNode* AddPositiveLeaf( T* const pData )
    {
        return m_pxPositive = new GLToy_BSPNode( pData );
    }

    GLToy_ForceInline GLToy_BSPNode* AddNegativeLeaf( T* const pData )
    {
        return m_pxNegative = new GLToy_BSPNode( pData );
    }

    void SplitLeaf( const GLToy_Vector_3& xPosition, const GLToy_Plane& xSplitPlane, T* const pDataPositive, T* const pDataNegative )
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

    void TraverseNodes( GLToy_ConstFunctor< GLToy_BSPNode< T > >& xFunctor ) const
    {
        xFunctor( this );

        if( !m_pData )
        {
            if( m_pxPositive )
            {
                m_pxPositive->TraverseNodes( xFunctor );
            }

            if( m_pxNegative )
            {
                m_pxNegative->TraverseNodes( xFunctor );
            }
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

    GLToy_ForceInline T* GetData() const { return m_pData; }
    GLToy_ForceInline GLToy_BSPNode* GetPositiveNode() const { return m_pxPositive; }
    GLToy_ForceInline GLToy_BSPNode* GetNegativeNode() const { return m_pxNegative; }
    GLToy_ForceInline const GLToy_Plane& GetPlane() const { return m_xPlane; }

    GLToy_ForceInline void SetData( T* const pData ) { m_pData = pData; }
    GLToy_ForceInline void SetPlane( const GLToy_Plane& xPlane ) { m_xPlane = xPlane; }

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
    : m_xSphereQuery()
    , m_pxHead( NULL )
    {
        m_xSphereQuery.Resize( 256 ); // who could ever want more than 256? :P
    }

    virtual ~GLToy_BSPTree()
    {
        delete m_pxHead;
    }

    static GLToy_Inline GLToy_BSPTree< T >* CreateBSPTree( T* pData, const unsigned int uCount )
    {
        GLToy_BSPTree< T >* pxTree = new GLToy_BSPTree< T >();

        for( unsigned int u = 0; u < uCount; ++u )
        {
            pxTree->AddLeaf( pData[ u ].GetPosition(), &( pData[ u ] ) );
        }

        return pxTree;
    }

    GLToy_ForceInline T* GetLeafData( const GLToy_Vector_3& xPosition )
    {
        return m_pxHead
            ? m_pxHead->FindLeaf( xPosition )->GetData()
            : 0;
    }

    GLToy_ForceInline T* GetLeafData( const GLToy_Vector_3& xPosition ) const
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

    GLToy_ForceInline void SplitLeaf( const GLToy_Vector_3& xPosition, const GLToy_Plane& xSplitPlane, T* const pDataPositive, T* const pDataNegative )
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

    virtual void TraverseNodes( GLToy_ConstFunctor< GLToy_BSPNode< T > >& xFunctor ) const
    {
        if( m_pxHead )
        {
            m_pxHead->TraverseNodes( xFunctor );
        }
    }

    GLToy_ForceInline void DistanceSortedLeafTraverse( GLToy_Functor< T >& xFunctor, const GLToy_Vector_3& xPosition )
    {
        if( m_pxHead )
        {
            m_pxHead->DistanceSortedLeafTraverse( xFunctor, xPosition );
        }
    }

    GLToy_ForceInline void DistanceSortedLeafTraverse( GLToy_ConstFunctor< T >& xFunctor, const GLToy_Vector_3& xPosition ) const
    {
        if( m_pxHead )
        {
            m_pxHead->DistanceSortedLeafTraverse( xFunctor, xPosition );
        }
    }

    GLToy_ForceInline void ReverseDistanceSortedLeafTraverse( GLToy_Functor< T >& xFunctor, const GLToy_Vector_3& xPosition )
    {
        if( m_pxHead )
        {
            m_pxHead->ReverseDistanceSortedLeafTraverse( xFunctor, xPosition );
        }
    }

    GLToy_ForceInline void ReverseDistanceSortedLeafTraverse( GLToy_ConstFunctor< T >& xFunctor, const GLToy_Vector_3& xPosition ) const
    {
        if( m_pxHead )
        {
            m_pxHead->ReverseDistanceSortedLeafTraverse( xFunctor, xPosition );
        }
    }

    GLToy_ForceInline bool IsEmpty() const { return m_pxHead == NULL; }

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

    const GLToy_Array< T* >& SphereQuery( const GLToy_Sphere& xSphere ) const
    {
        m_xSphereQuery.Clear();

        if( m_pxHead )
        {
            m_pxHead->FindLeavesTouchingSphere( xSphere, m_xSphereQuery );
        }

        return m_xSphereQuery;
    }

protected:

    mutable GLToy_Array< T* > m_xSphereQuery;

    GLToy_BSPNode< T >* m_pxHead;

};

#endif
