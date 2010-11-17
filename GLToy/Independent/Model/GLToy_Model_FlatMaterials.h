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

#ifndef __GLTOY_MODEL_FLATMATERIALS_H_
#define __GLTOY_MODEL_FLATMATERIALS_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parent
#include <Model/GLToy_Model_Coloured.h>

// GLToy
#include <Maths/GLToy_Vector.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_ModelVertex_FlatMaterials
{

public:

    GLToy_ModelVertex_FlatMaterials() { m_uIndices[ 0 ] = m_uIndices[ 1 ] = m_uIndices[ 2 ] = 0; }

    u_int& operator[]( const int iIndex ) { return m_uIndices[ iIndex ]; }
    const u_int& operator[]( const int iIndex ) const { return m_uIndices[ iIndex ]; }

private:

    u_int m_uIndices[ 3 ];

};

class GLToy_ModelMaterial_FlatMaterials
{

public:

    GLToy_ModelMaterial_FlatMaterials()
    : m_xDiffuse( GLToy_Maths::ZeroVector3 )
    , m_xSpecular( GLToy_Maths::ZeroVector3 )
    , m_fSpecularPower( 32.0f )
    {
    }

    GLToy_ModelMaterial_FlatMaterials( const GLToy_Vector_3 xDiffuse, const GLToy_Vector_3 xSpecular, const float fSpecularPower )
    : m_xDiffuse( xDiffuse )
    , m_xSpecular( xSpecular )
    , m_fSpecularPower( fSpecularPower )
    {
    }

    GLToy_Vector_3 m_xDiffuse;
    GLToy_Vector_3 m_xSpecular;
    float m_fSpecularPower;

};

class GLToy_ModelStrip_FlatMaterials
: public GLToy_ModelStrip_Coloured
{

    typedef GLToy_ModelStrip_Coloured GLToy_Parent;

public:

    GLToy_ModelStrip_FlatMaterials();
    GLToy_ModelStrip_FlatMaterials( const GLToy_ModelStrip_FlatMaterials& xStrip );
    virtual ~GLToy_ModelStrip_FlatMaterials();

    virtual GLToy_ModelStrip_FlatMaterials& operator =( const GLToy_ModelStrip_FlatMaterials& xStrip );

    virtual void Render() const;

    // overrides
    virtual void ReadFromBitStream( const GLToy_BitStream& xStream );
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const;

    void SetNormalPointer( const GLToy_Vector_3* pxNormals ) { m_pxNormals = pxNormals; }
    void SetUVPointer( const GLToy_Vector_2* pxUVs ) { m_pxUVs = pxUVs; }

    void AddVertex( const u_int uVertexIndex, const u_int uUVIndex, const u_int uNormalIndex );

protected:
    
    GLToy_Vector_3 m_xSpecularColour;
    float m_fSpecularPower;

    const GLToy_Vector_3* m_pxNormals;
    const GLToy_Vector_2* m_pxUVs;

    GLToy_Array< GLToy_ModelVertex_FlatMaterials > m_xIndices;

};

class GLToy_Model_FlatMaterials
: public GLToy_Model
{

    typedef GLToy_Model GLToy_Parent;

public:

    GLToy_Model_FlatMaterials();
    virtual ~GLToy_Model_FlatMaterials();

    // overrides
    virtual void ReadFromBitStream( const GLToy_BitStream& xStream );

    virtual void UpdateStripPointers();

    void AppendVertex( const GLToy_Vector_3& xVertex ) { m_xVertices.Append( xVertex ); }
    void AppendNormal( const GLToy_Vector_3& xNormal ) { m_xNormals.Append( xNormal ); }
    void AppendUV( const GLToy_Vector_2& xUV ) { m_xUVs.Append( xUV ); }
    void AppendFace() { Append( new GLToy_ModelStrip_FlatMaterials() ); }
    void AppendFaceVertex( const u_int uVertex, const u_int uUV, const u_int uNormal ) { static_cast< GLToy_ModelStrip_FlatMaterials& >( End() ).AddVertex( uVertex, uUV, uNormal ); }
    void AppendMaterial( const GLToy_Vector_3& xDiffuse, const GLToy_Vector_3& xSpecular, const float fSpecularExponent ) { m_xMaterials.Append( GLToy_ModelMaterial_FlatMaterials( xDiffuse, xSpecular, fSpecularExponent ) ); }

    GLToy_Array< GLToy_ModelMaterial_FlatMaterials >& GetMaterials() { return m_xMaterials; }
    const GLToy_Array< GLToy_ModelMaterial_FlatMaterials >& GetMaterials() const { return m_xMaterials; }

protected:

    GLToy_Array< GLToy_Vector_3 > m_xNormals;
    GLToy_Array< GLToy_Vector_2 > m_xUVs;
    GLToy_Array< GLToy_ModelMaterial_FlatMaterials > m_xMaterials;

};

#endif
