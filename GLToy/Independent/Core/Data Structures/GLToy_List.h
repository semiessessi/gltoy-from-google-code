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

#ifndef __GLTOY_LIST_H_
#define __GLTOY_LIST_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/Data Structures/GLToy_DataStructure.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

// TODO - finish and test then implement GLToy_IndirectList< T > with GLToy_List< T* >

template< class T >
class GLToy_List
: public GLToy_DataStructure< T >
{

    typedef GLToy_DataStructure< T > GLToy_Parent;

public:

    GLToy_List()
    : GLToy_Parent()
    , m_xData()
    , m_pxTail( NULL )
    {
    }

    GLToy_List( const T& xData )
    : GLToy_Parent()
    , m_xData( xData )
    , m_pxTail( NULL )
    {
    }

    GLToy_List( const GLToy_List& xList )
    : GLToy_Parent( xList )
    , m_xData()
    , m_pxTail( NULL )
    {
        CopyFrom( &xList );
    }

    virtual ~GLToy_List()
    {
        delete m_pxTail;
    }

    virtual T& operator []( const int iIndex )
    {
        if( iIndex == 0 )
        {
            return m_xData;
        }
        
        return m_pxTail->operator []( iIndex - 1 );
    }

    virtual const T& operator []( const int iIndex ) const
    {
        if( iIndex == 0 )
        {
            return m_xData;
        }
        
        return m_pxTail->operator []( iIndex - 1 );
    }

    virtual bool IsFlat() const { return false; }
    
    virtual u_int GetCount() const
    {
        if( !m_pxTail )
        {
            return 1;
        }
        
        return 1 + m_pxTail->GetCount();
    }

    virtual u_int GetMemoryUsage() const
    {
        return ( m_pxTail ? m_pxTail->GetMemoryUsage() : 0 ) + sizeof( this );
    }

    void Append( const T& xData )
    {
        if( !m_pxTail )
        {
            m_pxTail = new GLToy_List( xData );
            return;
        }

        m_pxTail->Append( xData );
    }

protected:

    virtual void CopyFrom( const GLToy_DataStructure< T >* const pxDataStructure )
    {
        delete m_pxTail;
        m_pxTail = 0;
        
        if( pxDataStructure->GetCount() == 0 )
        {
            return;
        }

        m_xData = ( *pxDataStructure )[ 0 ];

        for( u_int u = 1; u < pxDataStructure->GetCount(); ++u )
        {
            Append( ( *pxDataStructure )[ u ] );
        }
    }

    T m_xData;
    GLToy_List< T >* m_pxTail;

};

#endif