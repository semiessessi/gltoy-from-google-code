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

class GLToy_Environment_LightmappedFace
{

public:

    u_char m_aucLightmapStyles[ 8 ]; // inherited from quakes
    GLToy_Hash m_uTextureHash;
    GLToy_Array< u_int > m_xVertexIndices;
    GLToy_Array< GLToy_Vector_3 > m_xTexCoords;

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
    virtual void RenderTransparent() const;
    virtual void Update();

    virtual int GetType() const;

protected:

    GLToy_SerialisableArray< GLToy_Vector_3 > m_xVertices;
    GLToy_Array< GLToy_Environment_LightmappedFace > m_xFaces;
    GLToy_SerialisableArray< u_char > m_xLightmapData;

};

#endif