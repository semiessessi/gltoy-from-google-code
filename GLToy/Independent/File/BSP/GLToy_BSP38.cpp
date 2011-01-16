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
#include <Core/Data Structures/GLToy_Pair.h>
#include <Entity/GLToy_Entity_System.h>
#include <Environment/GLToy_Environment.h>
#include <Environment/GLToy_Environment_Lightmapped.h>
#include <Environment/GLToy_Environment_System.h>
#include <Material/GLToy_Material_System.h>
#include <Render/GLToy_Texture_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const u_int uBSP38_LUMP_ENTITIES = 0;
static const u_int uBSP38_LUMP_PLANES = 1;
static const u_int uBSP38_LUMP_VERTICES = 2;
static const u_int uBSP38_LUMP_VIS = 3;
static const u_int uBSP38_LUMP_NODES = 4;
static const u_int uBSP38_LUMP_TEXTURES = 5;
static const u_int uBSP38_LUMP_FACES = 6;
static const u_int uBSP38_LUMP_LIGHTMAPS = 7;
static const u_int uBSP38_LUMP_LEAVES = 8;
static const u_int uBSP38_LUMP_LEAFFACETABLE = 9;
static const u_int uBSP38_LUMP_LEAFBRUSHTABLE = 10;
static const u_int uBSP38_LUMP_EDGES = 11;
static const u_int uBSP38_LUMP_EDGEFACETABLE = 12;
static const u_int uBSP38_LUMP_MODELS = 13;
static const u_int uBSP38_LUMP_BRUSHES = 14;
static const u_int uBSP38_LUMP_BRUSHSIDES = 15;

static const u_int uBSP38_TCSCALE = 256;

static const u_int uBSP38_FACEFLAG_SKY = 0x4;
static const u_int uBSP38_FACEFLAG_TRANS33 = 0x10;
static const u_int uBSP38_FACEFLAG_TRANS66 = 0x20;
static const u_int uBSP38_FACEFLAG_NODRAW = 0x80;

static const u_int uBSP38_BRUSHFLAGS_SOLID = 0x1;
static const u_int uBSP38_BRUSHFLAGS_PLAYERCLIP = 0x10000;
static const u_int uBSP38_BRUSHFLAGS_AICLIP = 0x20000;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_BSP38_Lump
: public GLToy_SimpleSerialisable< GLToy_BSP38_Lump >
{

public:

    u_int m_uOffset;
    u_int m_uSize;

};

class GLToy_BSP38_LumpDirectory
: public GLToy_SimpleSerialisable< GLToy_BSP38_LumpDirectory >
{

public:

    GLToy_BSP38_Lump m_axLumps[ 19 ];

};


class GLToy_BSP38_Face
: public GLToy_SimpleSerialisable< GLToy_BSP38_Face >
{

public:

    u_short m_usPlane;
    u_short m_usPlaneSide;
    u_int m_uFirstEdge;
    u_short m_usEdgeCount;
    u_short m_usTextureInfo;
    u_char m_aucLightmapStyles[ 4 ];
    u_int m_uLightmapOffset;

};

class GLToy_BSP38_Plane
{

public:

    virtual void ReadFromBitStream( const GLToy_BitStream& xStream )
    {
        GLToy_Vector_3 xNormal;
        float fDistance;
        
        xStream >> xNormal;
        xStream >> fDistance;
        xStream >> m_uType;

        // this flips the normals to GLToy orientation
        // also id BSP files use the reverse convention for the plane distance and have opposite handedness
        m_xPlane = GLToy_Plane( GLToy_Vector_3( -( xNormal[ 1 ] ), xNormal[ 2 ], xNormal[ 0 ] ), -fDistance );
    }
    
    // TODO - do we ever want to write one of these files?
    void WriteToBitStream( GLToy_BitStream& xStream ) const {}

    GLToy_Plane m_xPlane;
    u_int m_uType;

};

class GLToy_BSP38_Node
: public GLToy_SimpleSerialisable< GLToy_BSP38_Node >
{

public:

    u_int m_uPlane;
    int m_iFrontChild;
    int m_iBackChild;
    u_short m_usBBMin[ 3 ];
    u_short m_usBBMax[ 3 ];
    u_short m_usFirstFace;
    u_short m_usFaceCount;

};

class GLToy_BSP38_Leaf
: public GLToy_SimpleSerialisable< GLToy_BSP38_Leaf >
{

public:

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
{

public:

    void ReadFromBitStream( const GLToy_BitStream& xStream )
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

    void WriteToBitStream( GLToy_BitStream& xStream ) const
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
: public GLToy_SimpleSerialisable< GLToy_BSP38_VisOffset >
{

public:

    u_int m_uPVS;
    u_int m_uPHS;

};

class GLToy_BSP38_Edge
: public GLToy_SimpleSerialisable< GLToy_BSP38_Edge >
{

public:

    u_short m_usVertex1;
    u_short m_usVertex2;

};

class GLToy_BSP38_Model
: public GLToy_SimpleSerialisable< GLToy_BSP38_Model >
{

public:

    GLToy_Vector_3 m_xMins;
    GLToy_Vector_3 m_xMaxs;
    GLToy_Vector_3 m_xOrigin;
    int m_iHead;
    u_int m_uFirstFace;
    u_int m_uNumFaces;

};

class GLToy_BSP38_Brush
: public GLToy_SimpleSerialisable< GLToy_BSP38_Brush >
{

public:

    u_int m_uFirstSide;
    u_int m_uNumSides;
    u_int m_uFlags;

};

class GLToy_BSP38_BrushSide
: public GLToy_SimpleSerialisable< GLToy_BSP38_BrushSide >
{

public:

    u_short m_usPlane;
    short m_sTexInfo;

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
    GLToy_Array< GLToy_Vector_3 > xVertices;
    xVertices.Resize( xLumps.m_axLumps[ uBSP38_LUMP_VERTICES ].m_uSize / sizeof( GLToy_Vector_3 ) );
    xStream.SetReadByte( xLumps.m_axLumps[ uBSP38_LUMP_VERTICES ].m_uOffset );
    for( u_int u = 0; u < xVertices.GetCount(); ++u )
    {
        xStream >> xVertices[ u ];
    }

    // brush lump
    GLToy_Array< GLToy_BSP38_Brush > xBrushes;
    xBrushes.Resize( xLumps.m_axLumps[ uBSP38_LUMP_BRUSHES ].m_uSize / 12 );
    xStream.SetReadByte( xLumps.m_axLumps[ uBSP38_LUMP_BRUSHES ].m_uOffset );
    for( u_int u = 0; u < xBrushes.GetCount(); ++u )
    {
        xStream >> xBrushes[ u ];
    }

    // brush side lump
    GLToy_Array< GLToy_BSP38_BrushSide > xBrushSides;
    xBrushSides.Resize( xLumps.m_axLumps[ uBSP38_LUMP_BRUSHSIDES ].m_uSize / 4 );
    xStream.SetReadByte( xLumps.m_axLumps[ uBSP38_LUMP_BRUSHSIDES ].m_uOffset );
    for( u_int u = 0; u < xBrushSides.GetCount(); ++u )
    {
        xStream >> xBrushSides[ u ];
    }

    // model lump
    GLToy_Array< GLToy_BSP38_Model > xModels;
    xModels.Resize( xLumps.m_axLumps[ uBSP38_LUMP_MODELS ].m_uSize / 48 );
    xStream.SetReadByte( xLumps.m_axLumps[ uBSP38_LUMP_MODELS ].m_uOffset );
    for( u_int u = 0; u < xModels.GetCount(); ++u )
    {
        xStream >> xModels[ u ];
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
    GLToy_Iterate( GLToy_BSP38_Face, xBSPFace, xFaces )
        uNumVertices += xBSPFace.m_usEdgeCount;
    GLToy_Iterate_End;

    pxEnv->m_xVertices.Resize( uNumVertices );

    // set up faces
    pxEnv->m_xFaces.Resize( xFaces.GetCount() );
    u_int uCurrentVertex = 0;
    GLToy_Iterate( GLToy_Environment_LightmappedFace, xFace, pxEnv->m_xFaces )
        GLToy_BSP38_Face& xBSPFace = xFaces[ xIterator.Index() ];
        xFace.m_aucLightmapStyles[ 0 ] = xBSPFace.m_aucLightmapStyles[ 0 ];
        xFace.m_aucLightmapStyles[ 1 ] = xBSPFace.m_aucLightmapStyles[ 1 ];
        xFace.m_aucLightmapStyles[ 2 ] = xBSPFace.m_aucLightmapStyles[ 2 ];
        xFace.m_aucLightmapStyles[ 3 ] = xBSPFace.m_aucLightmapStyles[ 3 ];
        xFace.m_aucLightmapStyles[ 4 ] = 0;
        xFace.m_aucLightmapStyles[ 5 ] = 0;
        xFace.m_aucLightmapStyles[ 6 ] = 0;
        xFace.m_aucLightmapStyles[ 7 ] = 0;

        xFace.m_bVisible = ( xTexInfos[ xBSPFace.m_usTextureInfo ].m_uFlags & uBSP38_FACEFLAG_NODRAW ) == 0;

        GLToy_Texture* pxTexture = GLToy_Texture_System::LookUpTextureNoExt( xTexInfos[ xBSPFace.m_usTextureInfo ].m_szTextureName );
        xFace.m_uTextureHash = pxTexture ? pxTexture->GetHash() : uGLTOY_BAD_HASH;

        u_int uTexWidth = pxTexture ? pxTexture->GetWidth() : uBSP38_TCSCALE;
        u_int uTexHeight = pxTexture ? pxTexture->GetHeight() : uBSP38_TCSCALE;

        if( pxTexture )
        {
            pxTexture->Create();
        }
        else
        {
            GLToy_Material* pxMaterial = GLToy_Material_System::LookUpMaterialNoExt( xTexInfos[ xBSPFace.m_usTextureInfo ].m_szTextureName ); 
            if( pxMaterial )
            {
                xFace.m_uTextureHash = pxMaterial->GetHash();
                pxMaterial->Initialise();

                uTexWidth = pxMaterial->GetWidth();
                uTexHeight = pxMaterial->GetHeight();
            }
        }

        xFace.m_xIndices.Resize( xBSPFace.m_usEdgeCount );

        // TODO - this loop can be refactored to not do something to entry zero first, i just can't be bothered right now
        u_int uFaceEdge = xBSPFace.m_uFirstEdge;
        int iEdge = xFaceEdges[ uFaceEdge ];
        xFace.m_xIndices[ 0 ] = uCurrentVertex;

        GLToy_Vector_3 xTangent = xTexInfos[ xBSPFace.m_usTextureInfo ].m_xUAxis;
        xTangent.Normalise();

        GLToy_Vector_3 xNormal = GLToy_Maths::ZeroVector3;
        u_int uFirstEdge = xBSPFace.m_uFirstEdge;
        // this crashes if there are degenerate faces...
        while( xNormal.MagnitudeSquared() < 0.00001f )
        {
            int iEdge1 = xFaceEdges[ uFirstEdge ];
            int iEdge2 = xFaceEdges[ uFirstEdge + 1 ];
            const GLToy_Vector_3& xVertex1 = xVertices[ ( iEdge1 < 0 ) ? xEdges[ -iEdge1 ].m_usVertex2 : xEdges[ iEdge1 ].m_usVertex1 ];
            const GLToy_Vector_3& xVertex2 = xVertices[ ( iEdge2 < 0 ) ? xEdges[ -iEdge2 ].m_usVertex2 : xEdges[ iEdge2 ].m_usVertex1 ];
            const GLToy_Vector_3& xVertex3 = xVertices[ ( iEdge2 < 0 ) ? xEdges[ -iEdge2 ].m_usVertex1 : xEdges[ iEdge2 ].m_usVertex2 ];
            xNormal = ( xVertex1 - xVertex2 ).Cross( xVertex3 - xVertex2 );
            ++uFirstEdge;
        }
        
        xNormal.Normalise();
        
        pxEnv->m_xVertices[ uCurrentVertex ].m_xPosition = xVertices[ ( iEdge < 0 ) ? xEdges[ -iEdge ].m_usVertex2 : xEdges[ iEdge ].m_usVertex1 ];

        // these need adjusting later...
        pxEnv->m_xVertices[ uCurrentVertex ].m_xLightmapUV =
            GLToy_Vector_2(
                xTexInfos[ xBSPFace.m_usTextureInfo ].m_xUAxis * pxEnv->m_xVertices[ uCurrentVertex ].m_xPosition + xTexInfos[ xBSPFace.m_usTextureInfo ].m_fUOffset,
                xTexInfos[ xBSPFace.m_usTextureInfo ].m_xVAxis * pxEnv->m_xVertices[ uCurrentVertex ].m_xPosition + xTexInfos[ xBSPFace.m_usTextureInfo ].m_fVOffset );
        
        pxEnv->m_xVertices[ uCurrentVertex ].m_xUV = 
            GLToy_Vector_2(
                pxEnv->m_xVertices[ uCurrentVertex ].m_xLightmapUV[ 0 ] / static_cast< float >( uTexWidth ),
                -( pxEnv->m_xVertices[ uCurrentVertex ].m_xLightmapUV[ 1 ] / static_cast< float >( uTexHeight ) ) );

        pxEnv->m_xVertices[ uCurrentVertex ].m_xNormal = xNormal;
        pxEnv->m_xVertices[ uCurrentVertex ].m_xTangent = xTangent;
        
        // work out the verts from the edges
        ++uCurrentVertex;
        for( u_int u = 0; ( u + 1 ) < xBSPFace.m_usEdgeCount; ++u )
        {
            uFaceEdge = u + xBSPFace.m_uFirstEdge;
            iEdge = xFaceEdges[ uFaceEdge ];

            xFace.m_xIndices[ u + 1 ] = uCurrentVertex;

            GLToy_Environment_LightmappedFaceVertex& xVertex = pxEnv->m_xVertices[ uCurrentVertex ];

            xVertex.m_xPosition = xVertices[ ( iEdge < 0 ) ? xEdges[ -iEdge ].m_usVertex1 : xEdges[ iEdge ].m_usVertex2 ];
            xVertex.m_xLightmapUV =
                GLToy_Vector_2(
                    xTexInfos[ xBSPFace.m_usTextureInfo ].m_xUAxis * xVertex.m_xPosition + xTexInfos[ xBSPFace.m_usTextureInfo ].m_fUOffset,
                    xTexInfos[ xBSPFace.m_usTextureInfo ].m_xVAxis * xVertex.m_xPosition + xTexInfos[ xBSPFace.m_usTextureInfo ].m_fVOffset );
            
            xVertex.m_xUV = 
                GLToy_Vector_2(
                    pxEnv->m_xVertices[ uCurrentVertex ].m_xLightmapUV[ 0 ] / static_cast< float >( uTexWidth ),
                    -( pxEnv->m_xVertices[ uCurrentVertex ].m_xLightmapUV[ 1 ] / static_cast< float >( uTexHeight ) ) );

            xVertex.m_xNormal = xNormal;
            xVertex.m_xTangent = xTangent;

            ++uCurrentVertex;
        }
    GLToy_Iterate_End;

    // re-orient vertices - this must be done after setting up the faces to ensure UVs are generated correctly
    GLToy_Iterate( GLToy_Environment_LightmappedFaceVertex, xCurrent, pxEnv->m_xVertices )
        xCurrent.m_xPosition = GLToy_Vector_3( -( xCurrent.m_xPosition[ 1 ] ), xCurrent.m_xPosition[ 2 ], xCurrent.m_xPosition[ 0 ] );
    GLToy_Iterate_End;

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
        if( ( xFaces[ u ].m_uLightmapOffset != 0xFFFFFFFF )
            && !( xTexInfos[ xFaces[ u ].m_usTextureInfo ].m_uFlags & uBSP38_FACEFLAG_SKY )
            && !( xTexInfos[ xFaces[ u ].m_usTextureInfo ].m_uFlags & uBSP38_FACEFLAG_TRANS33 )
            && !( xTexInfos[ xFaces[ u ].m_usTextureInfo ].m_uFlags & uBSP38_FACEFLAG_TRANS66 )
            && !( xTexInfos[ xFaces[ u ].m_usTextureInfo ].m_uFlags & uBSP38_FACEFLAG_NODRAW ) )
        {
            // this tries to replicate what Quake 2 does to build the lightmaps and the lightmap UVs
            // but without sticking lightmaps together
            float fUMax = -GLToy_Maths::LargeFloat;
            float fUMin = GLToy_Maths::LargeFloat;
            float fVMax = -GLToy_Maths::LargeFloat;
            float fVMin = GLToy_Maths::LargeFloat;

            GLToy_Iterate( u_int, uIndex, xEnvFace.m_xIndices )
                GLToy_Vector_2& xUV = pxEnv->m_xVertices[ uIndex ].m_xLightmapUV;

                fUMax = GLToy_Maths::Max( xUV[ 0 ], fUMax );
                fUMin = GLToy_Maths::Min( xUV[ 0 ], fUMin );
                fVMax = GLToy_Maths::Max( xUV[ 1 ], fVMax );
                fVMin = GLToy_Maths::Min( xUV[ 1 ], fVMin );
            GLToy_Iterate_End;

            const u_int uWidth = static_cast< u_int >( GLToy_Maths::Ceiling( fUMax / 16.0f ) - GLToy_Maths::Floor( fUMin / 16.0f ) ) + 1;
            const u_int uHeight = static_cast< u_int >( GLToy_Maths::Ceiling( fVMax / 16.0f ) - GLToy_Maths::Floor( fVMin / 16.0f ) ) + 1;

            // sanity checks
            GLToy_Assert( xFaces[ u ].m_uLightmapOffset % 3 == 0, "Lightmap offset should really divide 3" );
            GLToy_Assert( ( uWidth < 18 ) && ( uHeight < 18 ), "Lightmap is too big: %dx%d", uWidth, uHeight );

            xEnvFace.m_uLightmapHash = _GLToy_GetHash( reinterpret_cast< const char* const >( &uHashSource ), 4 );
            GLToy_Texture_System::CreateTextureFromRGBData( xEnvFace.m_uLightmapHash, &( pxEnv->m_xLightmapData[ xFaces[ u ].m_uLightmapOffset ] ), uWidth, uHeight );

            // fix up UVs
            for( u_int v = 0; v < xEnvFace.m_xIndices.GetCount(); ++v )
            {
                GLToy_Environment_LightmappedFaceVertex& xVertex = pxEnv->m_xVertices[ xEnvFace.m_xIndices[ v ] ];

                xVertex.m_xLightmapUV[ 0 ] -= GLToy_Maths::Floor( fUMin / 16.0f ) * 16.0f;
                xVertex.m_xLightmapUV[ 0 ] += 8.0f;
                xVertex.m_xLightmapUV[ 0 ] /= uWidth * 16.0f;

                xVertex.m_xLightmapUV[ 1 ] -= GLToy_Maths::Floor( fVMin / 16.0f ) * 16.0f;
                xVertex.m_xLightmapUV[ 1 ] += 8.0f;
                xVertex.m_xLightmapUV[ 1 ] /= uHeight * 16.0f;
            }
        }
        else
        {
            xEnvFace.m_uLightmapHash = GLToy_Hash_Constant( "White" );
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
                ppxNodes[ u ]->m_pxNegative =
                    ( xNodes[ u ].m_iBackChild >= 0 )
                        ? ppxNodes[ xNodes[ u ].m_iBackChild ]
                        : new GLToy_BSPNode< GLToy_EnvironmentLeaf >( &pxEnv->m_xLeaves[ -1 - xNodes[ u ].m_iBackChild ] );

                ppxNodes[ u ]->m_pxPositive =
                    ( xNodes[ u ].m_iFrontChild >= 0 )
                        ? ppxNodes[ xNodes[ u ].m_iFrontChild ]
                        : new GLToy_BSPNode< GLToy_EnvironmentLeaf >( &pxEnv->m_xLeaves[ -1 - xNodes[ u ].m_iFrontChild ] );
            }

            pxEnv->SetToNodePointer( ppxNodes[ 0 ] );

            GLToy_Assert( pxEnv->ValidateBSPTree(), "BSP tree is not valid! At least some leaves have faces that are on the wrong side of their parent node's plane." );

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

    // create brushes
    pxEnv->m_xBrushes.Resize( xBrushes.GetCount() );
    for( u_int u = 0; u < xBrushes.GetCount(); ++u )
    {
        for( u_int v = 0; v < xBrushes[ u ].m_uNumSides; ++v )
        {
            pxEnv->m_xBrushes[ u ].m_xPlanes.Append( xPlanes[ xBrushSides[ xBrushes[ u ].m_uFirstSide + v ].m_usPlane ].m_xPlane );
        }

        pxEnv->m_xBrushes[ u ].m_bSolid = ( xBrushes[ u ].m_uFlags & uBSP38_BRUSHFLAGS_SOLID ) != 0;
        pxEnv->m_xBrushes[ u ].m_bPlayerClip = ( xBrushes[ u ].m_uFlags & uBSP38_BRUSHFLAGS_PLAYERCLIP ) != 0;
        pxEnv->m_xBrushes[ u ].m_bAIClip = ( xBrushes[ u ].m_uFlags & uBSP38_BRUSHFLAGS_AICLIP ) != 0;
    }

    // entities...
    char* pcBuffer = new char[ xLumps.m_axLumps[ uBSP38_LUMP_ENTITIES ].m_uSize + 1 ];
    xStream.SetReadByte( xLumps.m_axLumps[ uBSP38_LUMP_ENTITIES ].m_uOffset );
    xStream.ByteAlignedWrite( pcBuffer, xLumps.m_axLumps[ uBSP38_LUMP_ENTITIES ].m_uSize );
    pcBuffer[ xLumps.m_axLumps[ uBSP38_LUMP_ENTITIES ].m_uSize ] = 0;
    GLToy_String szEntityData = pcBuffer;
    delete[] pcBuffer;
    
    // first split the data up per entity
    GLToy_Array< GLToy_String > xEntityStrings = szEntityData.Split( L'}' );
    GLToy_Iterate( GLToy_String, xString, xEntityStrings )
        xString.TrimLeadingWhiteSpace();
        xString.RemoveAt( 0 );
    GLToy_Iterate_End;

    // then for each value the entities have
    GLToy_Array< GLToy_Array< GLToy_String > > xEntityValues;
    xEntityValues.Resize( xEntityStrings.GetCount() );
    GLToy_ConstIterate( GLToy_String, szString, xEntityStrings )
        xEntityValues[ xIterator.Index() ] = szString.Split( '"' );
    GLToy_Iterate_End;

    GLToy_Array< GLToy_Array< GLToy_Pair< GLToy_String > > > xKeyValuePairs;
    xKeyValuePairs.Resize( xEntityStrings.GetCount() );
    GLToy_ConstIterate( GLToy_Array< GLToy_String >, xArray, xEntityValues )
        bool bFirst = true;
        const GLToy_ConstIterator< GLToy_Array< GLToy_String > >& xParentIterator = xIterator;
        GLToy_ConstIterate( GLToy_String, szString, xArray )
            if( !( szString.IsWhiteSpace() || szString.IsEmpty() ) )
            {
                if( bFirst )
                {
                    xKeyValuePairs[ xParentIterator.Index() ].Append( GLToy_Pair< GLToy_String >( szString ) );
                }
                else
                {
                    xKeyValuePairs[ xParentIterator.Index() ].End().Second() = szString;
                }

                bFirst = !bFirst;
            }
        GLToy_Iterate_End;

        GLToy_Assert( bFirst, "The number of keys and values is not the same for entity %d!", xIterator.Index() );
    GLToy_Iterate_End;

    // finally, create the entities
    GLToy_ConstIterate( GLToy_Array< GLToy_Pair< GLToy_String > >, xEntityDefinition, xKeyValuePairs )
        GLToy_Entity_System::CreateEntity( xEntityDefinition );
    GLToy_Iterate_End;

    GLToy_DebugOutput_Release( "Loaded BSP v38 (Quake 2/Kingpin) environment file \"%S\" successfully", m_szFilename.GetWideString() );
    GLToy_Environment_System::SetCurrentEnvironment( pxEnv );
}
