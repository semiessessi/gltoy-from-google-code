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

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Model/GLToy_Model_FlatMaterials.h>

// GLToy
#include <Render/GLToy_Render.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_ModelStrip_FlatMaterials::GLToy_ModelStrip_FlatMaterials()
: GLToy_Parent()
, m_uMaterialIndex( 0 )
{
}

GLToy_ModelStrip_FlatMaterials::GLToy_ModelStrip_FlatMaterials( const GLToy_ModelStrip_FlatMaterials& xStrip )
: GLToy_Parent( xStrip )
, m_uMaterialIndex( xStrip.m_uMaterialIndex )
{
}

GLToy_ModelStrip_FlatMaterials::~GLToy_ModelStrip_FlatMaterials()
{
}

GLToy_ModelStrip_FlatMaterials& GLToy_ModelStrip_FlatMaterials::operator =( const GLToy_ModelStrip_FlatMaterials& xStrip )
{
    GLToy_Parent::operator =( xStrip );

    return *this;
}

void GLToy_ModelStrip_FlatMaterials::Render() const
{
    GLToy_Render::StartSubmittingPolygon();
    
    GLToy_Render::SubmitColour( m_pxMaterials[ m_uMaterialIndex ].m_xDiffuse );

    for( GLToy_ConstIterator< GLToy_ModelVertex_FlatMaterials > xIterator; !xIterator.Done( m_xIndices ); xIterator.Next() )
{
const GLToy_ModelVertex_FlatMaterials& xFaceVertex = xIterator.Current( m_xIndices );
        GLToy_Render::SubmitVertex( m_pxVertices[ xFaceVertex[ 0 ] ] );
        GLToy_Render::SubmitUV( m_pxUVs[ xFaceVertex[ 1 ] ] );
        GLToy_Render::SubmitNormal( m_pxNormals[ xFaceVertex[ 2 ] ] );
    }
    
    GLToy_Render::EndSubmit();
}

void GLToy_ModelStrip_FlatMaterials::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    GLToy_Parent::ReadFromBitStream( xStream );
}

void GLToy_ModelStrip_FlatMaterials::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    GLToy_Parent::WriteToBitStream( xStream );
}

void GLToy_ModelStrip_FlatMaterials::AddVertex( const u_int uVertexIndex, const u_int uUVIndex, const u_int uNormalIndex )
{
    GLToy_ModelVertex_FlatMaterials xVertex;
    
    xVertex[ 0 ] = uVertexIndex;
    xVertex[ 1 ] = uUVIndex;
    xVertex[ 2 ] = uNormalIndex;
    
    m_xIndices.Append( xVertex );
}

//
// GLToy_Model_FlatMaterials
//

GLToy_Model_FlatMaterials::GLToy_Model_FlatMaterials()
: GLToy_Parent()
, m_xNormals()
, m_xUVs()
, m_xMaterials()
{
}

GLToy_Model_FlatMaterials::~GLToy_Model_FlatMaterials()
{
}

void GLToy_Model_FlatMaterials::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    GLToy_Model::ReadFromDerivedBitStream< GLToy_ModelStrip_FlatMaterials >( xStream );
}

void GLToy_Model_FlatMaterials::UpdateStripPointers()
{
    GLToy_Parent::UpdateStripPointers();
    for( GLToy_Iterator< GLToy_ModelStrip > xIterator; !xIterator.Done( *this ); xIterator.Next() )
    {
        GLToy_ModelStrip& xStrip = xIterator.Current( *this );
        static_cast< GLToy_ModelStrip_FlatMaterials& >( xStrip ).SetNormalPointer( m_xNormals.GetDataPointer() );
        static_cast< GLToy_ModelStrip_FlatMaterials& >( xStrip ).SetUVPointer( m_xUVs.GetDataPointer() );
        static_cast< GLToy_ModelStrip_FlatMaterials& >( xStrip ).SetMaterialPointer( m_xMaterials.GetDataPointer() );
    }
}
