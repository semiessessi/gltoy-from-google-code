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

#ifndef __GLTOY_TREE_H_
#define __GLTOY_TREE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/Data Structures/GLToy_DataStructure.h>

// GLToy
#include <Core/GLToy_Functor.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class T >
class GLToy_TreeIndexFunctor
: public GLToy_Functor< T >
{

public:

    GLToy_TreeIndexFunctor( const int iIndex )
    : m_iIndex( iIndex )
    , m_pxData( NULL )
    {
    }

    virtual void operator ()( T* const pxData )
    {
        if( m_iIndex == 0 )
        {
            m_pxData = pxData;
        }
        --m_iIndex;
    }

    virtual T* GetResult()
    {
        return m_pxData;
    }

protected:

    int m_iIndex;
    T* m_pxData;
};

template < class T >
class GLToy_ConstTreeIndexFunctor
: public GLToy_ConstFunctor< T >
{

public:

    GLToy_ConstTreeIndexFunctor( const int iIndex )
    : m_iIndex( iIndex )
    , m_pxData( NULL )
    {
    }

    virtual void operator ()( const T* const pxData )
    {
        if( m_iIndex == 0 )
        {
            m_pxData = pxData;
        }
        --m_iIndex;
    }

    virtual const T* GetResult()
    {
        return m_pxData;
    }

protected:

    int m_iIndex;
    const T* m_pxData;
};

template < class T >
class GLToy_TreeCountFunctor
: public GLToy_ConstFunctor< T >
{

public:

    GLToy_TreeCountFunctor()
    : m_uCount( 0 )
    {
    }

    virtual void operator ()( const T* const pxData )
    {
        ++m_uCount;
    }

    virtual u_int GetResult()
    {
        return m_uCount;
    }

protected:

    u_int m_uCount;
};

template < class T >
class GLToy_Tree
: public GLToy_DataStructure< T >
{

    typedef GLToy_DataStructure< T > GLToy_Parent;

public:

    virtual T& operator []( const int iIndex )
    {
        GLToy_TreeIndexFunctor< T > xFunctor( iIndex );
        Traverse( xFunctor );
        return *( xFunctor.GetResult() );
    }

    virtual const T& operator []( const int iIndex ) const
    {
        GLToy_ConstTreeIndexFunctor< T > xFunctor( iIndex );
        Traverse( xFunctor );
        return *( xFunctor.GetResult() );
    }

    virtual bool IsFlat() const { return false; }
    
    virtual u_int GetCount() const
    {
        GLToy_TreeCountFunctor< T > xFunctor;
        Traverse( xFunctor );
        return xFunctor.GetResult();
    }

    virtual u_int GetMemoryUsage() const
    {
        return 0;
    }

    GLToy_Inline void DeleteAll()
    {
        GLToy_DeleteFunctor< T > xFunctor;
        Traverse( xFunctor );
        Clear();
    }

    virtual void Clear() = 0;

protected:

    virtual void CopyFrom( const GLToy_DataStructure< T >* const pxDataStructure ) {}

};

#endif
