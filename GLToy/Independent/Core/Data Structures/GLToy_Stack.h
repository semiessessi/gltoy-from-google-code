#ifndef __GLTOY_STACK_H_
#define __GLTOY_STACK_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/Data Structures/GLToy_FlatDataStructure.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class T >
class GLToy_Stack : public GLToy_FlatDataStructure< T >
{

    typedef GLToy_FlatDataStructure< T > PARENT;

public:

    GLToy_Stack()
    : PARENT()
    , m_iStackPointer( -1 )
    {
    }

    GLToy_Stack( const GLToy_Stack& xStack )
    : PARENT( xStack )
    , m_iStackPointer( xStack.m_iStackPointer )
    {
        CopyFrom( &xStack );
    }
    
    T& Push( const T& xValue )
    {
        ++m_iStackPointer;
        
        CheckAlloc( m_iStackPointer + 1 );
        
        m_pxData[ m_iStackPointer ] = xValue;

        return m_pxData[ m_iStackPointer ];
    }

    T& Peek()
    {
        return m_pxData[ m_iStackPointer ];
    }
    
    T& Pop()
    {
        return m_pxData[ m_iStackPointer-- ];
    }
    
    void Clear()
    {
        m_iStackPointer = -1;
    }
    
    virtual u_int GetCount() const
    {
        return static_cast< u_int >( m_iStackPointer + 1 );
    }

    virtual u_int GetMemoryUsage() const
    {
        return PARENT::GetMemoryUsage() + sizeof( m_iStackPointer );
    }

    GLToy_Stack& operator =( const GLToy_FlatDataStructure< T >& xDataStructure )
    {
        CopyFrom( &xDataStructure );
        m_iStackPointer = xDataStructure.GetCount() - 1;

        return *this;
    }

protected:

    int m_iStackPointer;

};

#endif