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

// TODO - finish environment creation - verify loading

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

    virtual void ReadFromBitStream( const GLToy_BitStream& xStream ) { xStream >> m_xPlane; xStream >> m_uType; }
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const { xStream << m_xPlane; xStream << m_uType; }

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
        xStream << m_usFirstLeafBrush;
        xStream << m_usLeafBrushCount;
    }

    u_int m_uBrushOr;
    u_short m_usCluster;
    u_short m_usArea;
    u_short m_usBBMin[ 3 ];
    u_short m_usBBMax[ 3 ];
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
        // TODO - some kind of user feedback
        return;
    }

    // note that the magic numbers are intentional...
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
    pxEnv->m_xVertices.Resize( xLumps.m_axLumps[ uBSP38_LUMP_VERTICES ].m_uSize / sizeof( GLToy_Vector_3 ) );
    xStream.SetReadByte( xLumps.m_axLumps[ uBSP38_LUMP_VERTICES ].m_uOffset );
    for( u_int u = 0; u < pxEnv->m_xVertices.GetCount(); ++u )
    {
        xStream >> pxEnv->m_xVertices[ u ];
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

    // set up faces
    pxEnv->m_xFaces.Resize( xFaces.GetCount() );
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
        const u_int uTexWidth = pxTexture ? pxTexture->GetWidth() : 64;
        const u_int uTexHeight = pxTexture ? pxTexture->GetHeight() : 64;

        if( pxTexture )
        {
            pxTexture->Create();
        }

        // work out the verts from the elaborate edge data
        for( u_int u = 0; u < xBSPFace.m_usEdgeCount; ++u )
        {
            const u_int uFaceEdge = u + xBSPFace.m_uFirstEdge;
            const int iEdge = xFaceEdges[ uFaceEdge ];

            if( iEdge < 0 )
            {
                if( u == 0 )
                {
                    xFace.m_xVertexIndices.Append( xEdges[ -iEdge ].m_usVertex2 );
                }
                xFace.m_xVertexIndices.Append( xEdges[ -iEdge ].m_usVertex1 );
            }
            else
            {
                if( u == 0 )
                {
                    xFace.m_xVertexIndices.Append( xEdges[ iEdge ].m_usVertex1 );
                }
                xFace.m_xVertexIndices.Append( xEdges[ iEdge ].m_usVertex2 );
            }
            
            if( u == 0 )
            {
                xFace.m_xTexCoords.Append(
                    GLToy_Vector_3(
                        ( xTexInfos[ xBSPFace.m_usTextureInfo ].m_xUAxis * pxEnv->m_xVertices[ xFace.m_xVertexIndices[ 0 ] ] + xTexInfos[ xBSPFace.m_usTextureInfo ].m_fUOffset ) / static_cast< float >( uTexWidth ),
                        ( xTexInfos[ xBSPFace.m_usTextureInfo ].m_xVAxis * pxEnv->m_xVertices[ xFace.m_xVertexIndices[ 0 ] ] + xTexInfos[ xBSPFace.m_usTextureInfo ].m_fVOffset ) / static_cast< float >( uTexHeight ),
                        0.0f ) );
            }
            xFace.m_xTexCoords.Append(
                GLToy_Vector_3(
                    ( xTexInfos[ xBSPFace.m_usTextureInfo ].m_xUAxis * pxEnv->m_xVertices[ xFace.m_xVertexIndices.End() ] + xTexInfos[ xBSPFace.m_usTextureInfo ].m_fUOffset ) / static_cast< float >( uTexWidth ),
                    ( xTexInfos[ xBSPFace.m_usTextureInfo ].m_xVAxis * pxEnv->m_xVertices[ xFace.m_xVertexIndices.End() ] + xTexInfos[ xBSPFace.m_usTextureInfo ].m_fVOffset ) / static_cast< float >( uTexHeight ),
                    0.0f ) );
                    
        }
    }

    GLToy_DebugOutput_Release( "Loaded environment file \"%S\" successfully", m_szFilename.GetWideString() );
    GLToy_Environment_System::SwitchEnvironment( pxEnv );
}
