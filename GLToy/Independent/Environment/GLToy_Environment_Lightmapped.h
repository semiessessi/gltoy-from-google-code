/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010, 2011 Semi Essessi
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

#ifndef __GLTOY_ENVIRONMENT_LIGHTMAPPED_H_
#define __GLTOY_ENVIRONMENT_LIGHTMAPPED_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parent
#include <Environment/GLToy_Environment.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_EnvironmentFile;
class GLToy_Environment_Lightmapped;
class GLToy_IndexBuffer;
class GLToy_VertexBuffer_Deferred;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Environment_LightmappedBrush
{

public:

    GLToy_Environment_LightmappedBrush()
    : m_xPlanes()
    , m_uFlags( 0 )
    {
    }

    bool IsCollidable() const { return m_bSolid && !m_bEntityBrush; }
    bool IsAICollidable() const { return IsCollidable() || m_bAIClip; }
    bool IsPlayerCollidable() const { return IsCollidable() || m_bPlayerClip; }

    GLToy_Array< GLToy_Plane > m_xPlanes;

    union
    {
        struct
        {
            u_int m_bSolid : 1;
            u_int m_bPlayerClip : 1;
            u_int m_bAIClip : 1;
            u_int m_bEntityBrush : 1;
        };

        u_int m_uFlags;
    };
};

class GLToy_Environment_LightmappedFaceVertex
{

public:

    GLToy_Environment_LightmappedFaceVertex()
    : m_xPosition()
    , m_xUV()
    , m_xLightmapUV()
    , m_xNormal()
    , m_xTangent()
    , m_xColour()
    {
    }

    GLToy_Environment_LightmappedFaceVertex( const GLToy_Environment_LightmappedFaceVertex& xVertex )
    : m_xPosition( xVertex.m_xPosition )
    , m_xUV( xVertex.m_xUV )
    , m_xLightmapUV( xVertex.m_xLightmapUV )
    , m_xNormal( xVertex.m_xNormal )
    , m_xTangent( xVertex.m_xTangent )
    , m_xColour( xVertex.m_xColour )
    {
    }

    GLToy_Environment_LightmappedFaceVertex& operator =( const GLToy_Environment_LightmappedFaceVertex& xVertex )
    {
        m_xPosition = xVertex.m_xPosition;
        m_xUV = xVertex.m_xUV;
        m_xLightmapUV = xVertex.m_xLightmapUV;
        m_xNormal = xVertex.m_xNormal;
        m_xColour = xVertex.m_xColour;

        return *this;
    }

    bool operator ==( const GLToy_Environment_LightmappedFaceVertex& xVertex ) const
    {
        return
            ( m_xPosition == xVertex.m_xPosition )
            && ( m_xUV == xVertex.m_xUV )
            && ( m_xLightmapUV == xVertex.m_xLightmapUV )
            && ( m_xNormal == xVertex.m_xNormal )
            && ( m_xTangent == xVertex.m_xTangent )
            && ( m_xColour == xVertex.m_xColour );
    }

    GLToy_Vector_3 m_xPosition;
    GLToy_Vector_2 m_xUV;
    GLToy_Vector_2 m_xLightmapUV;
    GLToy_Vector_3 m_xNormal;
    GLToy_Vector_3 m_xTangent;
    GLToy_Vector_4 m_xColour;

};

class GLToy_Environment_LightmappedFace
{

public:

    GLToy_Environment_LightmappedFace()
    : m_uTextureHash( uGLTOY_BAD_HASH )
    , m_xIndices()
    , m_uFlags( 0 )
    , m_uRenderFlags( 0 )
    , m_ullVBD( 0 )
    {
    }

    u_int GetVertexCount() const { return m_xIndices.GetCount(); }

    u_char m_aucLightmapStyles[ 8 ]; // inherited from quakes
    GLToy_Hash m_uTextureHash;
    GLToy_Hash m_uLightmapHash;
    GLToy_Array< u_int > m_xIndices;

    union
    {
        struct
        {
            u_short m_usMaxIndex;
            u_short m_usMinIndex;
            u_short m_usOffset;
            u_short m_usCount;
        } m_xVertexBufferData;

        unsigned long long m_ullVBD;
    };

    union
    {

        struct
        {

            u_int m_bVisible : 1;

        };

        u_int m_uFlags;

    };

    union
    {

        struct
        {

            mutable u_int m_bRendered : 1;
            mutable u_int m_bRenderedLightmap : 1;

        };

        mutable u_int m_uRenderFlags;

    };

};

class GLToy_Environment_LightmappedCluster
{

    friend class GLToy_EnvironmentFile;
    friend class GLToy_Environment_Lightmapped;

public:

    GLToy_Environment_LightmappedCluster()
    : m_xPVS()
    , m_xIndices()
    {
    }

protected:

    GLToy_Array< u_int > m_xPVS;
    GLToy_Array< u_int > m_xIndices;
};

class GLToy_EnvironmentLeaf_Lightmapped
: public GLToy_EnvironmentLeaf
{

    friend class GLToy_EnvironmentFile;
    friend class GLToy_Environment_Lightmapped;

    typedef GLToy_EnvironmentLeaf GLToy_Parent;

public:

    GLToy_EnvironmentLeaf_Lightmapped( const GLToy_Environment_Lightmapped* const pxParent = NULL )
    : m_uCluster( 0xFFFFFFFF )
    , m_xIndices()
    , m_pxParent( pxParent )
    , m_xBatches()
    , m_xBoundingSphere()
    {
    }

    virtual void Render() const;
    virtual void RenderDeferred() const;
    virtual void RenderLightmap() const;
    virtual void RenderLighting() const;
    virtual void RenderDebugFaceInfo() const;
    virtual void RenderDebugNormals() const;

    const GLToy_Array< u_int >& GetIndices() const;
    const u_int GetFaceCount() const { return m_xIndices.GetCount(); }
    const GLToy_Environment_LightmappedFace& GetFace( const u_int uFace ) const;
    const GLToy_Environment_LightmappedFaceVertex& GetFaceVertex( const u_int uFace, const u_int uVertex ) const;


protected:
    
    u_int m_uCluster;
    GLToy_Array< u_int > m_xIndices;
    const GLToy_Environment_Lightmapped* m_pxParent;

    class MaterialBatch
    {
    public:

        MaterialBatch()
        : m_uHash( uGLTOY_BAD_HASH )
        , m_xFaces()
        {
        }
        
        MaterialBatch& operator= ( const MaterialBatch& xMaterialBatch )
        {
            m_uHash = xMaterialBatch.m_uHash;
            m_xFaces = xMaterialBatch.m_xFaces;
            return *this;
        }

        GLToy_Hash m_uHash;
        GLToy_Array< u_short > m_xFaces;
    };

    GLToy_Array< MaterialBatch > m_xBatches;
    GLToy_Sphere m_xBoundingSphere;

};

class GLToy_Environment_Lightmapped
: public GLToy_Environment
{
    friend class GLToy_EnvironmentFile;
    friend class GLToy_EnvironmentLeaf_Lightmapped;

    typedef GLToy_Environment GLToy_Parent;

public:

    GLToy_Environment_Lightmapped()
    : GLToy_Parent()
    , m_xVertices()
    , m_xFaces()
    , m_xLeaves()
    , m_xClusters()
    , m_xBrushes()
    , m_xLightmapData()
    , m_pxVertexBuffer( NULL )
    , m_pxIndexBuffer( NULL )
    {
    }

    virtual ~GLToy_Environment_Lightmapped() {}

    // TODO
    virtual void ReadFromBitStream( const GLToy_BitStream& xStream );
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const;

    virtual void Initialise();
    virtual void Shutdown();    
    
    virtual void Render() const;
    virtual void RenderDeferred() const;
    virtual void RenderLightmap() const;
    virtual void RenderLighting() const;
    virtual void Update();

    //virtual GLToy_Hash GetShaderHash() const { return GLToy_Hash_Constant( "Deferred_DiffuseOnly" ); }

    virtual void Render2D() const;

    virtual u_int GetType() const;

    virtual float Trace( const GLToy_Ray& xRay, const float fLimitingDistance = -1.0f ) const;

    virtual const GLToy_Environment_LightmappedBrush& GetBrush( const u_int uBrush ) const { return m_xBrushes[ uBrush ]; }
    virtual u_int GetVertexIndex( const GLToy_Environment_LightmappedFaceVertex& xVertex );
    
    virtual u_int GetBrushCount() const { return m_xBrushes.GetCount(); }
    virtual u_int GetFaceCount() const { return m_xFaces.GetCount(); }
    virtual u_int GetVertexCount( const u_int uFace ) const { return m_xFaces[ uFace ].m_xIndices.GetCount(); }
    virtual const GLToy_Vector_3& GetFaceVertexPosition( const u_int uFace, const u_int uVertex ) const;

    virtual bool ValidateBSPTree() const;

protected:

    GLToy_Array< GLToy_Environment_LightmappedFaceVertex > m_xVertices;
    GLToy_Array< GLToy_Environment_LightmappedFace > m_xFaces;
    GLToy_Array< GLToy_EnvironmentLeaf_Lightmapped > m_xLeaves;
    GLToy_Array< GLToy_Environment_LightmappedCluster > m_xClusters;
    GLToy_Array< GLToy_Environment_LightmappedBrush > m_xBrushes;
    GLToy_SerialisableArray< u_char > m_xLightmapData;
    GLToy_VertexBuffer_Deferred* m_pxVertexBuffer;
    GLToy_IndexBuffer* m_pxIndexBuffer;

};

#endif
