#ifndef __GLTOY_ENVIRONMENT_LIGHTMAPPED_H_
#define __GLTOY_ENVIRONMENT_LIGHTMAPPED_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parent
#include <Environment/GLToy_Environment.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Environment_LightmappedFaceVertex
{

public:

    GLToy_Environment_LightmappedFaceVertex()
    : m_uVertexIndex( 0 )
    , m_fU( 0.0f )
    , m_fV( 0.0f )
    , m_fLightmapU( 0.0f )
    , m_fLightmapV( 0.0f )
    {
    }

    GLToy_Environment_LightmappedFaceVertex( const GLToy_Environment_LightmappedFaceVertex& xVertex )
    : m_uVertexIndex( xVertex.m_uVertexIndex )
    , m_fU( xVertex.m_fU )
    , m_fV( xVertex.m_fV )
    , m_fLightmapU( xVertex.m_fLightmapU )
    , m_fLightmapV( xVertex.m_fLightmapV )
    {
    }

    GLToy_Environment_LightmappedFaceVertex& operator =( const GLToy_Environment_LightmappedFaceVertex& xVertex )
    {
        m_uVertexIndex = xVertex.m_uVertexIndex;
        m_fU = xVertex.m_fU;
        m_fV = xVertex.m_fV;
        m_fLightmapU = xVertex.m_fLightmapU;
        m_fLightmapV = xVertex.m_fLightmapV;

        return *this;
    }

    u_int m_uVertexIndex;
    float m_fU;
    float m_fV;
    float m_fLightmapU;
    float m_fLightmapV;

};

class GLToy_Environment_LightmappedFace
{

public:

    GLToy_Environment_LightmappedFace()
    : m_uTextureHash( uGLTOY_BAD_HASH )
    , m_xVertices()
    {
    }

    u_char m_aucLightmapStyles[ 8 ]; // inherited from quakes
    GLToy_Hash m_uTextureHash;
    GLToy_Array< GLToy_Environment_LightmappedFaceVertex > m_xVertices;

};

class GLToy_EnvironmentLeaf_Lightmapped
: public GLToy_EnvironmentLeaf
{

    friend class GLToy_EnvironmentFile;

    typedef GLToy_EnvironmentLeaf GLToy_Parent;

public:

};

class GLToy_Environment_Lightmapped
: public GLToy_Environment
{
    friend class GLToy_EnvironmentFile;

    typedef GLToy_Environment GLToy_Parent;

public:

    GLToy_Environment_Lightmapped()
    : GLToy_Parent()
    , m_xVertices()
    , m_xFaces()
    , m_xLightmapData()
    {
    }

    virtual ~GLToy_Environment_Lightmapped() {}

    // TODO
	virtual void ReadFromBitStream( const GLToy_BitStream& xStream );
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const;

    virtual void Initialise();
    virtual void Shutdown();    
    
    virtual void Render() const;
    virtual void RenderLightmap() const;
    virtual void Update();

    virtual int GetType() const;

protected:

    GLToy_SerialisableArray< GLToy_Vector_3 > m_xVertices;
    GLToy_Array< GLToy_Environment_LightmappedFace > m_xFaces;
    GLToy_SerialisableArray< u_char > m_xLightmapData;

};

#endif