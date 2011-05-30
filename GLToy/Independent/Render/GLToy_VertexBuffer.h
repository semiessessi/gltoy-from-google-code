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

#ifndef __GLTOY_VERTEXBUFFER_H_
#define __GLTOY_VERTEXBUFFER_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_IndexBuffer
: GLToy_Destroyable< GLToy_IndexBuffer >
{

public:

    static GLToy_IndexBuffer* Create( const u_int uCount, const u_short* pusIndices );
	static GLToy_IndexBuffer* Create( const u_int uCount, const u_int* puIndices );

    void Destroy();

    void Bind();

    void SubmitTriangles( const u_int uStart, const u_int uEnd, const u_int uCount, const u_int uOffset );
    void SubmitTriangleStrip( const u_int uStart, const u_int uEnd, const u_int uCount, const u_int uOffset );
    void SubmitPolygon( const u_int uStart, const u_int uEnd, const u_int uCount, const u_int uOffset );

	void SubmitAllAsTriangles();

protected:

    GLToy_IndexBuffer( const u_int uID, const u_int uCount )
    : m_uID( uID )
    , m_uCount( uCount )
    {
    }

    u_int m_uCount;

    union
    {
        u_int m_uID;
        int m_iID;
    };

};

struct GLToy_Vertex_Minimal
{
	GLToy_Vector_3 m_xPosition;
};

struct GLToy_Vertex_Deferred
{
    GLToy_Vector_4 m_xPosition;
    GLToy_Vector_4 m_xUV;

    struct
    {
        GLToy_Vector_2 m_xEncodedNormal;
        GLToy_Vector_2 m_xEncodedTangent;
    } m_xBasisVectors;

    GLToy_Vector_4 m_xPadding;
};

class GLToy_VertexBuffer_Minimal
: GLToy_Destroyable< GLToy_VertexBuffer_Minimal >
{

public:

    static GLToy_VertexBuffer_Minimal* Create( const u_int uCount, const GLToy_Vertex_Minimal* const pxVertices );

    void Destroy();

    void Bind();

protected:

    GLToy_VertexBuffer_Minimal( const u_int uID, const u_int uCount )
    : m_uID( uID )
    , m_uCount( uCount )
    {
    }

    u_int m_uCount;

    union
    {
        u_int m_uID;
        int m_iID;
    };
};

class GLToy_VertexBuffer_Deferred
: GLToy_Destroyable< GLToy_VertexBuffer_Deferred >
{

public:

    static GLToy_VertexBuffer_Deferred* Create( const u_int uCount, const GLToy_Vertex_Deferred* const pxVertices );

    void Destroy();

    void Bind();

protected:

    GLToy_VertexBuffer_Deferred( const u_int uID, const u_int uCount )
    : m_uID( uID )
    , m_uCount( uCount )
    {
    }

    u_int m_uCount;

    union
    {
        u_int m_uID;
        int m_iID;
    };
};

#endif
