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

template < class T >
class GLToy_Stack : public GLToy_FlatDataStructure< T >
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
        
        new ( &( m_pxData[ m_iStackPointer ] ) ) T( xValue );

        return m_pxData[ m_iStackPointer ];
    }

    T& Peek()
    {
        return m_pxData[ m_iStackPointer ];
    }
    
    T Pop()
    {
        T xReturnValue = m_pxData[ m_iStackPointer ];
        m_pxData[ m_iStackPointer ].~T();
        return xReturnValue;
    }
    
    void Clear()
    {
        m_iStackPointer = -1;

        // be careful to destroy the now unused entries
        for( int i = 0; i <= m_iStackPointer; ++i )
        {
            m_pxData[ i ].~T();
        }
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

#endif