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

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// This file's header
#include <Render/GLToy_VertexBuffer.h>

// GLToy
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Render_Metrics.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// M A C R O
/////////////////////////////////////////////////////////////////////////////////////////////

#define VERTEXBUFFER_OFFSET( u ) ( reinterpret_cast< void* >( u ) )

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_IndexBuffer* GLToy_IndexBuffer::Create( const u_int uCount, const u_short* pusIndices )
{
    u_int uID;
    GLToy_Render::GenBuffers( 1, &uID );
    GLToy_Render::BindBuffer( ELEMENT_ARRAY_BUFFER, uID );
    GLToy_Render::BufferData( ELEMENT_ARRAY_BUFFER, uCount * sizeof( u_short ), pusIndices, STATIC_DRAW );

    return new GLToy_IndexBuffer( uID, uCount );
}

GLToy_IndexBuffer* GLToy_IndexBuffer::Create( const u_int uCount, const u_int* puIndices )
{
    u_int uID;
    GLToy_Render::GenBuffers( 1, &uID );
    GLToy_Render::BindBuffer( ELEMENT_ARRAY_BUFFER, uID );
    GLToy_Render::BufferData( ELEMENT_ARRAY_BUFFER, uCount * sizeof( u_int ), puIndices, STATIC_DRAW );

    return new GLToy_IndexBuffer( uID, uCount );
}


void GLToy_IndexBuffer::Destroy()
{
    if( m_iID == -1 )
    {
        return;
    }
    
    GLToy_Render::DeleteBuffers( 1, &m_uID );
}

void GLToy_IndexBuffer::Bind()
{
    GLToy_Render::BindBuffer( ELEMENT_ARRAY_BUFFER, m_uID );
}

void GLToy_IndexBuffer::SubmitTriangles( const u_int uStart, const u_int uEnd, const u_int uCount, const u_int uOffset )
{
    GLToy_Render_Metrics::IncrementTriangleCount( uCount / 3 );
    GLToy_Render::DrawTriangles( uStart, uEnd, uCount, uOffset );
}

void GLToy_IndexBuffer::SubmitTriangleStrip( const u_int uStart, const u_int uEnd, const u_int uCount, const u_int uOffset )
{
    GLToy_Render_Metrics::IncrementTriangleCount( uCount - 2 );
    GLToy_Render::DrawTriangleStrip( uStart, uEnd, uCount, uOffset );
}

void GLToy_IndexBuffer::SubmitPolygon( const u_int uStart, const u_int uEnd, const u_int uCount, const u_int uOffset )
{
    GLToy_Render_Metrics::IncrementTriangleCount( uCount - 2 );
    GLToy_Render::DrawPolygon( uStart, uEnd, uCount, uOffset );
}

void GLToy_IndexBuffer::SubmitAllAsTriangles()
{
    GLToy_Render_Metrics::IncrementTriangleCount( m_uCount / 3 );
    GLToy_Render::DrawTriangles( 0, m_uCount - 1, m_uCount, 0 );
}

// Minimal vertices...

GLToy_VertexBuffer_Minimal* GLToy_VertexBuffer_Minimal::Create( const u_int uCount, const GLToy_Vertex_Minimal* const pxVertices )
{
    GLToy_Assert( sizeof( GLToy_Vertex_Minimal ) == 12, "GLToy_Vertex_Minimal has wrong size!" );

    u_int uID;
    GLToy_Render::GenBuffers( 1, &uID );
    GLToy_Render::BindBuffer( ARRAY_BUFFER, uID );
    GLToy_Render::BufferData( ARRAY_BUFFER, uCount * sizeof( GLToy_Vertex_Minimal ), pxVertices, STATIC_DRAW );

    return new GLToy_VertexBuffer_Minimal( uID, uCount );
}

void GLToy_VertexBuffer_Minimal::Destroy()
{
    if( m_iID == -1 )
    {
        return;
    }
    
    GLToy_Render::DeleteBuffers( 1, &m_uID );
}

void GLToy_VertexBuffer_Minimal::Bind()
{
    GLToy_Render::BindBuffer( ARRAY_BUFFER, m_uID );
    GLToy_Render::VertexPointer( 3, sizeof( GLToy_Vertex_Minimal ), VERTEXBUFFER_OFFSET( 0 ) );
}

// Deferred vertices...

GLToy_VertexBuffer_Deferred* GLToy_VertexBuffer_Deferred::Create( const u_int uCount, const GLToy_Vertex_Deferred* const pxVertices )
{
    GLToy_Assert( sizeof( GLToy_Vertex_Deferred ) == 64, "GLToy_Vertex_Deferred has wrong size!" );

    u_int uID;
    GLToy_Render::GenBuffers( 1, &uID );
    GLToy_Render::BindBuffer( ARRAY_BUFFER, uID );
    GLToy_Render::BufferData( ARRAY_BUFFER, uCount * sizeof( GLToy_Vertex_Deferred ), pxVertices, STATIC_DRAW );

    return new GLToy_VertexBuffer_Deferred( uID, uCount );
}

void GLToy_VertexBuffer_Deferred::Destroy()
{
    if( m_iID == -1 )
    {
        return;
    }
    
    GLToy_Render::DeleteBuffers( 1, &m_uID );
}

void GLToy_VertexBuffer_Deferred::Bind()
{
    GLToy_Render::BindBuffer( ARRAY_BUFFER, m_uID );
    GLToy_Render::VertexPointer( 4, sizeof( GLToy_Vertex_Deferred ), VERTEXBUFFER_OFFSET( 0 ) );
    GLToy_Render::ClientActiveTexture( TEXTURE0 );
    GLToy_Render::TexCoordPointer( 4, sizeof( GLToy_Vertex_Deferred ), VERTEXBUFFER_OFFSET( 16 ) );
    GLToy_Render::ClientActiveTexture( TEXTURE0 + 1 );
    GLToy_Render::TexCoordPointer( 4, sizeof( GLToy_Vertex_Deferred ), VERTEXBUFFER_OFFSET( 32 ) );
}
