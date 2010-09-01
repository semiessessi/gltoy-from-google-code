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

#ifndef __GLTOY_DATASTRUCTURE_H_
#define __GLTOY_DATASTRUCTURE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy_Functor.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

template< class T >
class GLToy_DataStructure
{

public:

    GLToy_DataStructure() {}
    GLToy_DataStructure( const GLToy_DataStructure& xDataStructure ) {}
    virtual ~GLToy_DataStructure() {}
    
    virtual u_int GetCount() const = 0;
    virtual u_int GetMemoryUsage() const = 0;

    virtual bool IsFlat() const = 0;

    virtual T& operator []( const int iIndex ) = 0;
    virtual const T& operator []( const int iIndex ) const = 0;

    virtual void Traverse( GLToy_Functor< T >& xFunctor ) = 0;
    virtual void Traverse( GLToy_ConstFunctor< T >& xFunctor ) const = 0;

    bool IsEmpty() const { return GetCount() == 0; }

protected:

    virtual void CopyFrom( const GLToy_DataStructure< T >* const pxDataStructure ) = 0;

};

template< class T >
class GLToy_Iterator
{
        
public:

    GLToy_Iterator( GLToy_DataStructure< T >* const pxDataStructure )
    : m_pxDataStructure( pxDataStructure )
    , m_iIndex( 0 )
    {
    }

    T& Current() const { return m_pxDataStructure->operator []( m_iIndex ); }
    const int& Index() const { return m_iIndex; }

    void Next() { ++m_iIndex; }
    bool Done() const { return static_cast< u_int >( m_iIndex ) >= m_pxDataStructure->GetCount(); }

protected:

    GLToy_DataStructure< T >* m_pxDataStructure;
    int m_iIndex;

private:

    GLToy_Iterator( const GLToy_Iterator& ) {}

};

template< class T >
class GLToy_ConstIterator
{

public:

    GLToy_ConstIterator( const GLToy_DataStructure< T >* const pxDataStructure )
    : m_pxDataStructure( pxDataStructure )
    , m_iIndex( 0 )
    {
    }

    const T& Current() const { return m_pxDataStructure->operator []( m_iIndex ); }
    const int& Index() const { return m_iIndex; }

    void Next() { ++m_iIndex; }
    bool Done() const { return static_cast< u_int >( m_iIndex ) >= m_pxDataStructure->GetCount(); }

protected:

    const GLToy_DataStructure< T >* m_pxDataStructure;
    int m_iIndex;

private:

    GLToy_ConstIterator( const GLToy_ConstIterator& ) {}

};

#define GLToy_Iterate( T, xIteratorName, pxDataStructure ) for( GLToy_Iterator< T > xIteratorName( pxDataStructure ); !xIteratorName.Done(); xIteratorName.Next() )
#define GLToy_ConstIterate( T, xIteratorName, pxDataStructure ) for( GLToy_ConstIterator< T > xIteratorName( pxDataStructure ); !xIteratorName.Done(); xIteratorName.Next() )

#endif
