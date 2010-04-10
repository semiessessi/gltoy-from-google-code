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
//
// MD2 file loader
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// Based the information here: http://www.flipcode.com/archives/Quake_2_BSP_File_Format.shtml
// but with additional details inferred from the Quake 2 source: http://www.idsoftware.com/
//
/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <File/GLToy_EnvironmentFile.h>

// GLToy
#include <Environment/GLToy_Environment.h>
#include <Environment/GLToy_Environment_Lightmapped.h>
#include <Environment/GLToy_Environment_System.h>
#include <Render/GLToy_Texture.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const u_int uBSP38_LUMP_PLANES = 1;
static const u_int uBSP38_LUMP_VERTICES = 2;
static const u_int uBSP38_LUMP_VIS = 3;
static const u_int uBSP38_LUMP_NODES = 4;
static const u_int uBSP38_LUMP_TEXTURES = 5;
static const u_int uBSP38_LUMP_FACES = 6;
static const u_int uBSP38_LUMP_LIGHTMAPS = 7;
static const u_int uBSP38_LUMP_LEAVES = 8;
static const u_int uBSP38_LUMP_LEAFFACETABLE = 9;
static const u_int uBSP38_LUMP_EDGES = 11;
static const u_int uBSP38_LUMP_EDGEFACETABLE = 12;

static const u_int uBSP38_TCSCALE = 256;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_BSP38_Lump
: public GLToy_Serialisable
{

public:
    
    virtual void ReadFromBitStream( const GLToy_BitStream& xStream ) { xStream >> m_uOffset; xStream >> m_uSize; }
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const { xStream << m_uOffset; xStream << m_uSize; }

    u_int m_uOffset;
    u_int m_uSize;

};

class GLToy_BSP38_LumpDirectory
: public GLToy_Serialisable
{

public:
    
    virtual void ReadFromBitStream( const GLToy_BitStream& xStream ) { for( u_int u = 0; u < 19; ++u ) { xStream >> m_axLumps[ u ]; } }
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const { for( u_int u = 0; u < 19; ++u ) { xStream << m_axLumps[ u ]; } }


    GLToy_BSP38_Lump m_axLumps[ 19 ];

};


class GLToy_BSP38_Face
: public GLToy_Serialisable
{

public:

    virtual void ReadFromBitStream( const GLToy_BitStream& xStream )
    {
        xStream >> m_usPlane;
        xStream >> m_usPlaneSide;
        xStream >> m_uFirstEdge;
        xStream >> m_usEdgeCount;
        xStream >> m_usTextureInfo;
        xStream >> m_aucLightmapStyles[ 0 ];
        xStream >> m_aucLightmapStyles[ 1 ];
        xStream >> m_aucLightmapStyles[ 2 ];
        xStream >> m_aucLightmapStyles[ 3 ];
        xStream >> m_uLightmapOffset;
    }

    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const
    {
        xStream << m_usPlane;
        xStream << m_usPlaneSide;
        xStream << m_uFirstEdge;
        xStream << m_usEdgeCount;
        xStream << m_usTextureInfo;
        xStream << m_aucLightmapStyles[ 0 ];
        xStream << m_aucLightmapStyles[ 1 ];
        xStream << m_aucLightmapStyles[ 2 ];
        xStream << m_aucLightmapStyles[ 3 ];
        xStream << m_uLightmapOffset;
    }

    u_short m_usPlane;
    u_short m_usPlaneSide;
    u_int m_uFirstEdge;
    u_short m_usEdgeCount;
    u_short m_usTextureInfo;
    u_char m_aucLightmapStyles[ 4 ];
    u_int m_uLightmapOffset;

};

class GLToy_BSP38_Plane
: public GLToy_Serialisable
{

public:

    virtual void ReadFromBitStream( const GLToy_BitStream& xStream )
    {
        GLToy_Vector_3 xNormal;
        float fDistance;
        
        xStream >> xNormal;
        xStream >> fDistance;
        xStream >> m_uType;

        m_xPlane = GLToy_Plane( GLToy_Vector_3( -xNormal[ 1 ], xNormal[ 2 ], xNormal[ 0 ] ), fDistance );
    }
    
    // TODO - do we ever want to write one of these files?
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const {}

    GLToy_Plane m_xPlane;
    u_int m_uType;

};

class GLToy_BSP38_Node
: public GLToy_Serialisable
{

public:

    virtual void ReadFromBitStream( const GLToy_BitStream& xStream )
    {
        xStream >> m_uPlane;
        xStream >> m_iFrontChild;
        xStream >> m_iBackChild;
        xStream >> m_usBBMin[ 0 ];
        xStream >> m_usBBMin[ 1 ];
        xStream >> m_usBBMin[ 2 ];
        xStream >> m_usBBMax[ 0 ];
        xStream >> m_usBBMax[ 1 ];
        xStream >> m_usBBMax[ 2 ];
        xStream >> m_usFirstFace;
        xStream >> m_usFaceCount;
    }

    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const
    {
        xStream << m_uPlane;
        xStream << m_iFrontChild;
        xStream << m_iBackChild;
        xStream << m_usBBMin[ 0 ];
        xStream << m_usBBMin[ 1 ];
        xStream << m_usBBMin[ 2 ];
        xStream << m_usBBMax[ 0 ];
        xStream << m_usBBMax[ 1 ];
        xStream << m_usBBMax[ 2 ];
        xStream << m_usFirstFace;
        xStream << m_usFaceCount;
    }

    u_int m_uPlane;
    int m_iFrontChild;
    int m_iBackChild;
    u_short m_usBBMin[ 3 ];
    u_short m_usBBMax[ 3 ];
    u_short m_usFirstFace;
    u_short m_usFaceCount;

};

class GLToy_BSP38_Leaf
: public GLToy_Serialisable
{

public:

    virtual void ReadFromBitStream( const GLToy_BitStream& xStream )
    {
        xStream >> m_uBrushOr;
        xStream >> m_usCluster;
        xStream >> m_usArea;
        xStream >> m_usBBMin[ 0 ];
        xStream >> m_usBBMin[ 1 ];
        xStream >> m_usBBMin[ 2 ];
        xStream >> m_usBBMax[ 0 ];
        xStream >> m_usBBMax[ 1 ];
        xStream >> m_usBBMax[ 2 ];
        xStream >> m_usFirstLeafFace;
        xStream >> m_usLeafFaceCount;
        xStream >> m_usFirstLeafBrush;
        xStream >> m_usLeafBrushCount;
    }

    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const
    {
        xStream << m_uBrushOr;
        xStream << m_usCluster;
        xStream << m_usArea;
        xStream << m_usBBMin[ 0 ];
        xStream << m_usBBMin[ 1 ];
        xStream << m_usBBMin[ 2 ];
        xStream << m_usBBMax[ 0 ];
        xStream << m_usBBMax[ 1 ];
        xStream << m_usBBMax[ 2 ];
        xStream << m_usFirstLeafFace;
        xStream << m_usLeafFaceCount;
        xStream << m_usFirstLeafBrush;
        xStream << m_usLeafBrushCount;
    }

    u_int m_uBrushOr;
    u_short m_usCluster;
    u_short m_usArea;
    u_short m_usBBMin[ 3 ];
    u_short m_usBBMax[ 3 ];
    u_short m_usFirstLeafFace;
    u_short m_usLeafFaceCount;
    u_short m_usFirstLeafBrush;
    u_short m_usLeafBrushCount;

};

class GLToy_BSP38_TextureInfo
: public GLToy_Serialisable
{

public:

    virtual void ReadFromBitStream( const GLToy_BitStream& xStream )
    {
        xStream >> m_xUAxis;
        xStream >> m_fUOffset;
        xStream >> m_xVAxis;
        xStream >> m_fVOffset;
        xStream >> m_uFlags;
        xStream >> m_uValue;
        for( u_int u = 0; u < 32; ++u ) { xStream >> m_szTextureName[ u ]; }
        xStream >> m_uNextTextureInfo;
    }

    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const
    {
        xStream << m_xUAxis;
        xStream << m_fUOffset;
        xStream << m_xVAxis;
        xStream << m_fVOffset;
        xStream << m_uFlags;
        xStream << m_uValue;
        for( u_int u = 0; u < 32; ++u ) { xStream << m_szTextureName[ u ]; }
        xStream << m_uNextTextureInfo;
    }

    GLToy_Vector_3 m_xUAxis;
    float m_fUOffset;
    GLToy_Vector_3 m_xVAxis;
    float m_fVOffset;
    u_int m_uFlags;
    u_int m_uValue;
    char m_szTextureName[ 32 ];
    u_int m_uNextTextureInfo;

};

class GLToy_BSP38_VisOffset
: public GLToy_Serialisable
{

public:
    
    virtual void ReadFromBitStream( const GLToy_BitStream& xStream ) { xStream >> m_uPVS; xStream >> m_uPHS; }
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const { xStream << m_uPVS; xStream << m_uPHS; }

    u_int m_uPVS;
    u_int m_uPHS;

};

class GLToy_BSP38_Edge
: public GLToy_Serialisable
{

public:
    
    virtual void ReadFromBitStream( const GLToy_BitStream& xStream ) { xStream >> m_usVertex1; xStream >> m_usVertex2; }
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const { xStream << m_usVertex1; xStream << m_usVertex2; }

    u_short m_usVertex1;
    u_short m_usVertex2;

};

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_EnvironmentFile::LoadBSP38( const GLToy_BitStream& xStream ) const
{
    GLToy_Environment_Lightmapped* pxEnv = static_cast< GLToy_Environment_Lightmapped* >( GLToy_Environment_System::CreateEnvironmentFromType( ENV_LIGHTMAPPED ) );

    if( !pxEnv )
    {
        GLToy_Assert( pxEnv != NULL, "Failed to create lightmapped environment" );
        return;
    }

    // note that the magic numbers are intentional...
    // in hindsight I should have added "GetSize" members to the lump component classes (see BSP v46 loader)
    GLToy_BSP38_LumpDirectory xLumps;
    xStream >> xLumps;

    // plane lump
    GLToy_Array< GLToy_BSP38_Plane > xPlanes;
    xPlanes.Resize( xLumps.m_axLumps[ uBSP38_LUMP_PLANES ].m_uSize / 20 );
    xStream.SetReadByte( xLumps.m_axLumps[ uBSP38_LUMP_PLANES ].m_uOffset );
    for( u_int u = 0; u < xPlanes.GetCount(); ++u )
    {
        xStream >> xPlanes[ u ];
    }

    // vertex lump
    // build a local list of vertices because they don't match what we want in the environment proper
    GLToy_Array< GLToy_Vector_3 > xVertices;
    xVertices.Resize( xLumps.m_axLumps[ uBSP38_LUMP_VERTICES ].m_uSize / sizeof( GLToy_Vector_3 ) );
    xStream.SetReadByte( xLumps.m_axLumps[ uBSP38_LUMP_VERTICES ].m_uOffset );
    for( u_int u = 0; u < xVertices.GetCount(); ++u )
    {
        xStream >> xVertices[ u ];
    }

    // vis lump
    GLToy_Array< GLToy_BSP38_VisOffset > xVisOffsets;
    xStream.SetReadByte( xLumps.m_axLumps[ uBSP38_LUMP_VIS ].m_uOffset );

    u_int uNumClusters = 0;
    xStream >> uNumClusters;

    xVisOffsets.Resize( uNumClusters );
    for( u_int u = 0; u < uNumClusters; ++u )
    {
        xStream >> xVisOffsets[ u ];
    }

    GLToy_SerialisableArray< u_char > xVisData;
    xVisData.Resize( xLumps.m_axLumps[ uBSP38_LUMP_VIS ].m_uSize );
    xStream.SetReadByte( xLumps.m_axLumps[ uBSP38_LUMP_VIS ].m_uOffset );
    xStream.ByteAlignedWrite( reinterpret_cast< char* >( xVisData.GetDataPointer() ), xVisData.GetCount() );

    // nodes
    GLToy_Array< GLToy_BSP38_Node > xNodes;
    xNodes.Resize( xLumps.m_axLumps[ uBSP38_LUMP_NODES ].m_uSize / 28 );
    xStream.SetReadByte( xLumps.m_axLumps[ uBSP38_LUMP_NODES ].m_uOffset );

    for( u_int u = 0; u < xNodes.GetCount(); ++u )
    {
        xStream >> xNodes[ u ];
    }

    // texture info
    GLToy_Array< GLToy_BSP38_TextureInfo > xTexInfos;
    xTexInfos.Resize( xLumps.m_axLumps[ uBSP38_LUMP_TEXTURES ].m_uSize / 76 );
    xStream.SetReadByte( xLumps.m_axLumps[ uBSP38_LUMP_TEXTURES ].m_uOffset );

    for( u_int u = 0; u < xTexInfos.GetCount(); ++u )
    {
        xStream >> xTexInfos[ u ];
    }

    // faces
    GLToy_Array< GLToy_BSP38_Face > xFaces;
    xFaces.Resize( xLumps.m_axLumps[ uBSP38_LUMP_FACES ].m_uSize / 20 );
    xStream.SetReadByte( xLumps.m_axLumps[ uBSP38_LUMP_FACES ].m_uOffset );

    for( u_int u = 0; u < xFaces.GetCount(); ++u )
    {
        xStream >> xFaces[ u ];
    }

    // lightmaps
    // do these later...

    // leaves
    GLToy_Array< GLToy_BSP38_Leaf > xLeaves;
    xLeaves.Resize( xLumps.m_axLumps[ uBSP38_LUMP_LEAVES ].m_uSize / 28 );
    xStream.SetReadByte( xLumps.m_axLumps[ uBSP38_LUMP_LEAVES ].m_uOffset );

    for( u_int u = 0; u < xLeaves.GetCount(); ++u )
    {
        xStream >> xLeaves[ u ];
    }

    // leaf-face table
    GLToy_Array< u_short > xLeafFaces;
    xLeafFaces.Resize( xLumps.m_axLumps[ uBSP38_LUMP_LEAFFACETABLE ].m_uSize / sizeof( u_short ) );
    xStream.SetReadByte( xLumps.m_axLumps[ uBSP38_LUMP_LEAFFACETABLE ].m_uOffset );

    for( u_int u = 0; u < xLeafFaces.GetCount(); ++u )
    {
        xStream >> xLeafFaces[ u ];
    }

    // edges
    GLToy_Array< GLToy_BSP38_Edge > xEdges;
    xEdges.Resize( xLumps.m_axLumps[ uBSP38_LUMP_EDGES ].m_uSize / 4 );
    xStream.SetReadByte( xLumps.m_axLumps[ uBSP38_LUMP_EDGES ].m_uOffset );

    for( u_int u = 0; u < xEdges.GetCount(); ++u )
    {
        xStream >> xEdges[ u ];
    }

    // face edges
    GLToy_Array< int > xFaceEdges;
    xFaceEdges.Resize( xLumps.m_axLumps[ uBSP38_LUMP_EDGEFACETABLE ].m_uSize / 4 );
    xStream.SetReadByte( xLumps.m_axLumps[ uBSP38_LUMP_EDGEFACETABLE ].m_uOffset );

    for( u_int u = 0; u < xFaceEdges.GetCount(); ++u )
    {
        xStream >> xFaceEdges[ u ];
    }

    // initialise vertices
    u_int uNumVertices = 0;
    GLToy_Iterate( GLToy_BSP38_Face, xIterator, &xFaces )
    {
        GLToy_BSP38_Face& xBSPFace = xIterator.Current();
        uNumVertices += xBSPFace.m_usEdgeCount;
    }

    pxEnv->m_xVertices.Resize( uNumVertices );

    // set up faces
    pxEnv->m_xFaces.Resize( xFaces.GetCount() );
    u_int uCurrentVertex = 0;
    GLToy_Iterate( GLToy_Environment_LightmappedFace, xIterator, &( pxEnv->m_xFaces ) )
    {
        GLToy_Environment_LightmappedFace& xFace = xIterator.Current();
        GLToy_BSP38_Face& xBSPFace = xFaces[ xIterator.Index() ];
        xFace.m_aucLightmapStyles[ 0 ] = xBSPFace.m_aucLightmapStyles[ 0 ];
        xFace.m_aucLightmapStyles[ 1 ] = xBSPFace.m_aucLightmapStyles[ 1 ];
        xFace.m_aucLightmapStyles[ 2 ] = xBSPFace.m_aucLightmapStyles[ 2 ];
        xFace.m_aucLightmapStyles[ 3 ] = xBSPFace.m_aucLightmapStyles[ 3 ];
        xFace.m_aucLightmapStyles[ 4 ] = 0;
        xFace.m_aucLightmapStyles[ 5 ] = 0;
        xFace.m_aucLightmapStyles[ 6 ] = 0;
        xFace.m_aucLightmapStyles[ 7 ] = 0;

		GLToy_Texture* pxTexture = GLToy_Texture_System::LookUpTextureNoExt( xTexInfos[ xBSPFace.m_usTextureInfo ].m_szTextureName );
		xFace.m_uTextureHash = pxTexture ? pxTexture->GetHash() : uGLTOY_BAD_HASH;

        if( pxTexture )
        {
            pxTexture->Create();
        }

        const u_int uTexWidth = pxTexture ? pxTexture->GetWidth() : uBSP38_TCSCALE;
        const u_int uTexHeight = pxTexture ? pxTexture->GetHeight() : uBSP38_TCSCALE;

        xFace.m_xIndices.Resize( xBSPFace.m_usEdgeCount );

        // TODO - this loop can be refactored to not do something to entry zero first, i just can't be bothered right now
        u_int uFaceEdge = xBSPFace.m_uFirstEdge;
        int iEdge = xFaceEdges[ uFaceEdge ];
        xFace.m_xIndices[ 0 ] = uCurrentVertex;
        
        pxEnv->m_xVertices[ uCurrentVertex ].m_xVertex = xVertices[ ( iEdge < 0 ) ? xEdges[ -iEdge ].m_usVertex2 : xEdges[ iEdge ].m_usVertex1 ];

        // these need adjusting later...
        pxEnv->m_xVertices[ uCurrentVertex ].m_xLightmapUV =
            GLToy_Vector_2(
                xTexInfos[ xBSPFace.m_usTextureInfo ].m_xUAxis * pxEnv->m_xVertices[ uCurrentVertex ].m_xVertex + xTexInfos[ xBSPFace.m_usTextureInfo ].m_fUOffset,
                xTexInfos[ xBSPFace.m_usTextureInfo ].m_xVAxis * pxEnv->m_xVertices[ uCurrentVertex ].m_xVertex + xTexInfos[ xBSPFace.m_usTextureInfo ].m_fVOffset );
        
        pxEnv->m_xVertices[ uCurrentVertex ].m_xUV = 
            GLToy_Vector_2(
            pxEnv->m_xVertices[ uCurrentVertex ].m_xLightmapUV[ 0 ] / static_cast< float >( uTexWidth ),
            pxEnv->m_xVertices[ uCurrentVertex ].m_xLightmapUV[ 1 ] / static_cast< float >( uTexHeight ) );
        
        // work out the verts from the edges
        ++uCurrentVertex;
        for( u_int u = 0; ( u + 1 ) < xBSPFace.m_usEdgeCount; ++u )
        {
            uFaceEdge = u + xBSPFace.m_uFirstEdge;
            iEdge = xFaceEdges[ uFaceEdge ];

            xFace.m_xIndices[ u + 1 ] = uCurrentVertex;

            GLToy_Environment_LightmappedFaceVertex& xVertex = pxEnv->m_xVertices[ uCurrentVertex ];

            xVertex.m_xVertex = xVertices[ ( iEdge < 0 ) ? xEdges[ -iEdge ].m_usVertex1 : xEdges[ iEdge ].m_usVertex2 ];
            xVertex.m_xLightmapUV =
                GLToy_Vector_2(
                    xTexInfos[ xBSPFace.m_usTextureInfo ].m_xUAxis * xVertex.m_xVertex + xTexInfos[ xBSPFace.m_usTextureInfo ].m_fUOffset,
                    xTexInfos[ xBSPFace.m_usTextureInfo ].m_xVAxis * xVertex.m_xVertex + xTexInfos[ xBSPFace.m_usTextureInfo ].m_fVOffset );
            
            xVertex.m_xUV = 
                GLToy_Vector_2(
                pxEnv->m_xVertices[ uCurrentVertex ].m_xLightmapUV[ 0 ] / static_cast< float >( uTexWidth ),
                pxEnv->m_xVertices[ uCurrentVertex ].m_xLightmapUV[ 1 ] / static_cast< float >( uTexHeight ) );

            ++uCurrentVertex;
        }
    }

    // re-orient vertices - this must be done after setting up the faces to ensure UVs are generated correctly
    GLToy_Iterate( GLToy_Environment_LightmappedFaceVertex, xIterator, &( pxEnv->m_xVertices ) )
    {
        GLToy_Environment_LightmappedFaceVertex& xCurrent = xIterator.Current();
        xCurrent.m_xVertex = GLToy_Vector_3( -( xCurrent.m_xVertex[ 1 ] ), xCurrent.m_xVertex[ 2 ], xCurrent.m_xVertex[ 0 ] );
    }

    // create the lightmap textures
    pxEnv->m_xLightmapData.Resize( xLumps.m_axLumps[ uBSP38_LUMP_LIGHTMAPS ].m_uSize );
    xStream.SetReadByte( xLumps.m_axLumps[ uBSP38_LUMP_LIGHTMAPS ].m_uOffset );
    xStream.ByteAlignedWrite( reinterpret_cast< char* >( pxEnv->m_xLightmapData.GetDataPointer() ), xLumps.m_axLumps[ uBSP38_LUMP_LIGHTMAPS ].m_uSize );

    for( u_int u = 0; u < xFaces.GetCount(); ++u )
    {
        GLToy_Environment_LightmappedFace& xEnvFace = pxEnv->m_xFaces[ u ];

        // this nasty hashing is so we don't screw the tree balance and turn a large part of it into a linked list
        const u_int uHashSource = 1337 * u + 7;

        // create the texture
        // TODO - this needs to be redone on reloads
        if( ( xFaces[ u ].m_uLightmapOffset != 0xFFFFFFFF ) && ( xFaces[ u ].m_uLightmapOffset != 0 ) )
        {
            // this tries to replicate what Quake 2 does to build the lightmaps and the lightmap UVs
            // but without sticking lightmaps together
            float fUMax = -GLToy_Maths::LargeFloat;
            float fUMin = GLToy_Maths::LargeFloat;
            float fVMax = -GLToy_Maths::LargeFloat;
            float fVMin = GLToy_Maths::LargeFloat;

            GLToy_Iterate( u_int, xIterator, &( xEnvFace.m_xIndices ) )
            {
                GLToy_Vector_2& xUV = pxEnv->m_xVertices[ xIterator.Current() ].m_xLightmapUV;

                fUMax = GLToy_Maths::Max( xUV[ 0 ], fUMax );
                fUMin = GLToy_Maths::Min( xUV[ 0 ], fUMin );
                fVMax = GLToy_Maths::Max( xUV[ 1 ], fVMax );
                fVMin = GLToy_Maths::Min( xUV[ 1 ], fVMin );
            }

            const u_int uWidth = static_cast< u_int >( GLToy_Maths::Ceiling( fUMax / 16.0f ) - GLToy_Maths::Floor( fUMin / 16.0f ) ) + 1;
            const u_int uHeight = static_cast< u_int >( GLToy_Maths::Ceiling( fVMax / 16.0f ) - GLToy_Maths::Floor( fVMin / 16.0f ) ) + 1;

            // sanity checks
            GLToy_Assert( xFaces[ u ].m_uLightmapOffset % 3 == 0, "Lightmap offset should really divide 3" );
            // GLToy_Assert( ( uWidth < 17 ) && ( uHeight < 17 ), "Lightmap is too big: %dx%d", uWidth, uHeight );
			GLToy_Assert( ( uWidth < 18 ) && ( uHeight < 18 ), "Lightmap is too big: %dx%d", uWidth, uHeight );

            GLToy_Texture_System::CreateTextureFromRGBData( _GLToy_GetHash( reinterpret_cast< const char* const >( &uHashSource ), 4 ), &( pxEnv->m_xLightmapData[ xFaces[ u ].m_uLightmapOffset ] ), uWidth, uHeight );

            // fix up UVs
            for( u_int v = 0; v < xEnvFace.m_xIndices.GetCount(); ++v )
            {
                GLToy_Environment_LightmappedFaceVertex& xVertex = pxEnv->m_xVertices[ xEnvFace.m_xIndices[ v ] ];

                xVertex.m_xLightmapUV[ 0 ] -= GLToy_Maths::Floor( fUMin / 16.0f ) * 16.0f;
                xVertex.m_xLightmapUV[ 0 ] += 8;
                xVertex.m_xLightmapUV[ 0 ] /= uWidth * 16.0f;

                xVertex.m_xLightmapUV[ 1 ] -= GLToy_Maths::Floor( fVMin / 16.0f ) * 16.0f;
                xVertex.m_xLightmapUV[ 1 ] += 8;
                xVertex.m_xLightmapUV[ 1 ] /= uHeight * 16.0f;
            }
        }
        else
        {
            u_int uData = 0xFFFFFFFF;
            GLToy_Texture_System::CreateTextureFromRGBAData( _GLToy_GetHash( reinterpret_cast< const char* const >( &uHashSource ), 4 ), &uData, 1, 1 );
        }
    }

    // create leaves and clusters
    if( xLeaves.GetCount() > 0 )
    {
        pxEnv->m_xLeaves.Resize( xLeaves.GetCount() );
        pxEnv->m_xClusters.Resize( uNumClusters );
        for( u_int u = 0; u < xLeaves.GetCount(); ++u )
        {
            pxEnv->m_xLeaves[ u ].m_pxParent = pxEnv;
            pxEnv->m_xLeaves[ u ].m_xIndices.Resize( xLeaves[ u ].m_usLeafFaceCount );
            pxEnv->m_xLeaves[ u ].m_uCluster = xLeaves[ u ].m_usCluster;
            if( xLeaves[ u ].m_usCluster != 0xFFFF )
            {
                pxEnv->m_xClusters[ xLeaves[ u ].m_usCluster ].m_xIndices.Append( u );
            }

            for( u_int v = 0; v < xLeaves[ u ].m_usLeafFaceCount; ++v )
            {
                pxEnv->m_xLeaves[ u ].m_xIndices[ v ] = xLeafFaces[ xLeaves[ u ].m_usFirstLeafFace + v ];
            }
        }

        if( xNodes.GetCount() > 0 )
        {
            // create the BSP nodes
            GLToy_BSPNode< GLToy_EnvironmentLeaf >** ppxNodes = new GLToy_BSPNode< GLToy_EnvironmentLeaf >*[ xNodes.GetCount() ];

            for( u_int u = 0 ; u < xNodes.GetCount(); ++u )
            {
                ppxNodes[ u ] =
                    new GLToy_BSPNode< GLToy_EnvironmentLeaf >(
                        xPlanes[ xNodes[ u ].m_uPlane ].m_xPlane.GetNormal(),
                        xPlanes[ xNodes[ u ].m_uPlane ].m_xPlane.GetDistance() );
            }

            for( u_int u = 0 ; u < xNodes.GetCount(); ++u )
            {
                ppxNodes[ u ]->m_pxPositive =
                    ( xNodes[ u ].m_iBackChild >= 0 )
                        ? ppxNodes[ xNodes[ u ].m_iBackChild ]
                        : new GLToy_BSPNode< GLToy_EnvironmentLeaf >( &pxEnv->m_xLeaves[ -1 - xNodes[ u ].m_iBackChild ] );

                ppxNodes[ u ]->m_pxNegative =
                    ( xNodes[ u ].m_iFrontChild >= 0 )
                        ? ppxNodes[ xNodes[ u ].m_iFrontChild ]
                        : new GLToy_BSPNode< GLToy_EnvironmentLeaf >( &pxEnv->m_xLeaves[ -1 - xNodes[ u ].m_iFrontChild ] );
            }

            pxEnv->SetToNodePointer( ppxNodes[ 0 ] );

            delete[] ppxNodes;
        }

        // decompress the PVSs
        for( u_int w = 0; w < uNumClusters; ++w )
        {
            for( u_int u = 0, v = xVisOffsets[ w ].m_uPVS; u < uNumClusters; ++v )
            {
               if( xVisData[ v ] == 0 )
               {
                  ++v;     
                  u += 8 * xVisData[ v ];
               }
               else
               {
                  for( u_char ucBit = 1; ucBit != 0; ucBit <<= 1, ++u )
                  {
                     if( xVisData[ v ] & ucBit )
                     {
                        pxEnv->m_xClusters[ w ].m_xPVS.Append( u );
                     }
                  }
               }   
            }
        }
    }

    GLToy_DebugOutput_Release( "Loaded BSP v38 (Quake 2/Kingpin) environment file \"%S\" successfully", m_szFilename.GetWideString() );
    GLToy_Environment_System::SetCurrentEnvironment( pxEnv );
}
