/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <File/GLToy_EnvironmentFile.h>

// GLToy
#include <Environment/GLToy_Environment.h>
#include <Environment/GLToy_Environment_System.h>

// TODO - finish BSP v38 environment geometry loading

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

class GLToy_BSP38_ShortPoint
: public GLToy_Serialisable
{

public:

    virtual void ReadFromBitStream( const GLToy_BitStream& xStream ) { for( u_int u = 0; u < 3; ++u ) { xStream >> m_usPoints[ u ]; } }
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const { for( u_int u = 0; u < 3; ++u ) { xStream << m_usPoints[ u ]; } }

    u_short m_usPoints[ 3 ];

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
        xStream >> m_xBBMin;
        xStream >> m_xBBMax;
        xStream >> m_usFirstFace;
        xStream >> m_usFaceCount;
    }

    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const
    {
        xStream << m_uPlane;
        xStream << m_iFrontChild;
        xStream << m_iBackChild;
        xStream << m_xBBMin;
        xStream << m_xBBMax;
        xStream << m_usFirstFace;
        xStream << m_usFaceCount;
    }

    u_int m_uPlane;
    int m_iFrontChild;
    int m_iBackChild;
    GLToy_BSP38_ShortPoint m_xBBMin;
    GLToy_BSP38_ShortPoint m_xBBMax;
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
        xStream >> m_xBBMin;
        xStream >> m_xBBMax;
        xStream >> m_usFirstLeafBrush;
        xStream >> m_usLeafBrushCount;
    }

    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const
    {
        xStream << m_uBrushOr;
        xStream << m_usCluster;
        xStream << m_usArea;
        xStream << m_xBBMin;
        xStream << m_xBBMax;
        xStream << m_usFirstLeafBrush;
        xStream << m_usLeafBrushCount;
    }

    u_int m_uBrushOr;
    u_short m_usCluster;
    u_short m_usArea;
    GLToy_BSP38_ShortPoint m_xBBMin;
    GLToy_BSP38_ShortPoint m_xBBMax;
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

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_EnvironmentFile::LoadBSP38( const GLToy_BitStream& xStream ) const
{
    GLToy_BSP38_LumpDirectory xLumps;
    xStream >> xLumps;

    // plane lump
    GLToy_Array< GLToy_BSP38_Plane > xPlanes;
    xPlanes.Resize( xLumps.m_axLumps[ uBSP38_LUMP_PLANES ].m_uSize / sizeof( GLToy_BSP38_Plane ) );
    xStream.SetReadByte( xLumps.m_axLumps[ uBSP38_LUMP_PLANES ].m_uOffset );
    for( u_int u = 0; u < xPlanes.GetCount(); ++u )
    {
        xStream >> xPlanes[ u ];
    }

    // vertex lump
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

    // TODO ...
}