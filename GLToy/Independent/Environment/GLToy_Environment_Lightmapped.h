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
    : m_xVertex()
    , m_xUV()
    , m_xLightmapUV()
    , m_xNormal()
    , m_xColour()
    {
    }

    GLToy_Environment_LightmappedFaceVertex( const GLToy_Environment_LightmappedFaceVertex& xVertex )
    : m_xVertex( xVertex.m_xVertex )
    , m_xUV( xVertex.m_xUV )
    , m_xLightmapUV( xVertex.m_xLightmapUV )
    , m_xNormal( xVertex.m_xNormal )
    , m_xColour( xVertex.m_xColour )
    {
    }

    GLToy_Environment_LightmappedFaceVertex& operator =( const GLToy_Environment_LightmappedFaceVertex& xVertex )
    {
        m_xVertex = xVertex.m_xVertex;
        m_xUV = xVertex.m_xUV;
        m_xLightmapUV = xVertex.m_xLightmapUV;
        m_xNormal = xVertex.m_xNormal;
        m_xColour = xVertex.m_xColour;

        return *this;
    }

    bool operator ==( const GLToy_Environment_LightmappedFaceVertex& xVertex ) const
    {
        return
            ( m_xVertex == xVertex.m_xVertex )
            && ( m_xUV == xVertex.m_xUV )
            && ( m_xLightmapUV == xVertex.m_xLightmapUV )
            && ( m_xNormal == xVertex.m_xNormal )
            && ( m_xColour == xVertex.m_xColour );
    }

    GLToy_Vector_3 m_xVertex;
    GLToy_Vector_2 m_xUV;
    GLToy_Vector_2 m_xLightmapUV;
    GLToy_Vector_3 m_xNormal;
    GLToy_Vector_4 m_xColour;

};

class GLToy_Environment_LightmappedFace
{

public:

    GLToy_Environment_LightmappedFace()
    : m_uTextureHash( uGLTOY_BAD_HASH )
    , m_xIndices()
    {
    }

    u_char m_aucLightmapStyles[ 8 ]; // inherited from quakes
    GLToy_Hash m_uTextureHash;
    GLToy_Array< u_int > m_xIndices;

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

    virtual float Trace( const GLToy_Ray& xRay, const float fLimitingDistance = -1.0f ) const;

    virtual u_int GetVertexIndex( const GLToy_Environment_LightmappedFaceVertex& xVertex );

protected:

    // GLToy_SerialisableArray< GLToy_Vector_3 > m_xVertices;
    GLToy_Array< GLToy_Environment_LightmappedFaceVertex > m_xVertices;
    GLToy_Array< GLToy_Environment_LightmappedFace > m_xFaces;
    GLToy_SerialisableArray< u_char > m_xLightmapData;

};

#endif