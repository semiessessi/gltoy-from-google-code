/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010 Semi Essessi
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

#ifndef __GLTOY_STACK_H_
#define __GLTOY_STACK_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/Data Structures/GLToy_FlatDataStructure.h>

// C/C++
#include <new>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy_Memory_DebugOff.h>

template < class T >
class GLToy_Stack
: public GLToy_FlatDataStructure< T >
{

    typedef GLToy_FlatDataStructure< T > GLToy_Parent;

public:

    GLToy_Stack()
    : GLToy_Parent()
    , m_iStackPointer( -1 )
    {
    }

    GLToy_Stack( const GLToy_Stack& xStack )
    : GLToy_Parent( xStack )
    , m_iStackPointer( -1 )  // CopyFrom requires this
    {
        CopyFrom( &xStack );
        m_iStackPointer = xStack.m_iStackPointer;
    }
    
    T& Push( const T& xValue )
    {
        ++m_iStackPointer;
        
        CheckAlloc( m_iStackPointer + 1 );
        
        new ( &( GLToy_Parent::m_pxData[ m_iStackPointer ] ) ) T( xValue );

        return GLToy_Parent::m_pxData[ m_iStackPointer ];
    }

    GLToy_Inline T& Peek( const u_int uFromTop = 0 )
    {
        return GLToy_Parent::m_pxData[ m_iStackPointer - uFromTop ];
    }
    
    GLToy_Inline T Pop()
    {
        T xReturnValue = GLToy_Parent::m_pxData[ m_iStackPointer ];
        GLToy_Parent::m_pxData[ m_iStackPointer ].~T();
        --m_iStackPointer;
        return xReturnValue;
    }
    
    void Clear()
    {
        m_iStackPointer = -1;

        // be careful to destroy the now unused entries
        for( int i = 0; i <= m_iStackPointer; ++i )
        {
            GLToy_Parent::m_pxData[ i ].~T();
        }
    }

    void DeleteAll()
    {
        for( GLToy_Iterator< T > xIterator; !xIterator.Done( *this ); xIterator.Next() )
        {
            T& xElement = xIterator.Current( *this );
            delete xElement;
        }

        Clear();
    }
    
    virtual u_int GetCount() const
    {
        return static_cast< u_int >( m_iStackPointer + 1 );
    }

    virtual u_int GetMemoryUsage() const
    {
        return GLToy_Parent::GetMemoryUsage() + sizeof( m_iStackPointer );
    }

    GLToy_Stack& operator =( const GLToy_DataStructure< T >& xDataStructure )
    {
        CopyFrom( &xDataStructure );
        m_iStackPointer = xDataStructure.GetCount() - 1;

        return *this;
    }

protected:

    int m_iStackPointer;

};

#include <Core/GLToy_Memory_DebugOn.h>

#endif
