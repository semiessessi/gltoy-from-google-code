/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2011 Semi Essessi
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

#ifndef __GLTOY_GRAPH_H_
#define __GLTOY_GRAPH_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Core/Data Structures/GLToy_Array.h>
#include <Core/GLToy_Functor.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

template< class T >
class GLToy_Graph
{

public:

    GLToy_Graph()
    : m_xNodeData()
    , m_xAdjacencyLists()
    {
    }

    GLToy_Graph( const GLToy_Graph& xGraph )
    : m_xNodeData( xGraph.m_xNodeData )
    , m_xAdjacencyLists( xGraph.m_xAdjacencyLists ) // CopyFrom requires this
    {
    }
    
    GLToy_Graph& operator =( const GLToy_Graph& xGraph )
    {
        m_xNodeData = xGraph.m_xNodeData;
        m_xAdjacencyLists = xGraph.m_xAdjacencyLists;
        return *this;
    }

    virtual ~GLToy_Graph()
    {
    }

    GLToy_ForceInline T& operator []( const int iIndex )
    {
        return m_xNodeData[ iIndex ];
    }

    GLToy_ForceInline const T& operator []( const int iIndex ) const
    {
        return m_xNodeData[ iIndex ];
    }

    const bool AdjacencyTest( const int iIndex1, const int iIndex2 ) const
    {
        return m_xAdjacencyLists[ iIndex1 ].Contains( iIndex2 );
    }

    const GLToy_Array< int >& GetAdjacencyList( const int iIndex ) const
    {
        return m_xAdjacencyLists[ iIndex ];
    }

    void Connect( const int iIndex1, const int iIndex2 )
    {
        m_xAdjacencyLists[ iIndex1 ].Append( iIndex2 );
        m_xAdjacencyLists[ iIndex2 ].Append( iIndex1 );
    }

    void Attach( const T& xData, const int iConnectToIndex = -1 )
    {
        m_xNodeData.Append( xData );
        m_xAdjacencyLists.Append( GLToy_Array< int >() );
        if( iConnectToIndex >= 0 )
        {
            Connect( m_xNodeData.GetCount() - 1, iConnectToIndex );
        }
    }

protected:

    GLToy_Array< T > m_xNodeData;
    GLToy_Array< GLToy_Array< int > > m_xAdjacencyLists;

};

#endif
