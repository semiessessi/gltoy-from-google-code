/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010 Semi Essessi
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
#include <File/GLToy_EnvironmentFile.h>

// GLToy
#include <Environment/GLToy_Environment_System.h>
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const u_int uVBSP19_LUMP_ENTITIES = 0;
static const u_int uVBSP19_LUMP_PLANES = 1;
static const u_int uVBSP19_LUMP_TEXTUREDATA = 2;
static const u_int uVBSP19_LUMP_VERTICES = 3;
static const u_int uVBSP19_LUMP_VISIBILITY = 4;
static const u_int uVBSP19_LUMP_NODES = 5;
static const u_int uVBSP19_LUMP_TEXTUREINFO = 6;
static const u_int uVBSP19_LUMP_FACES = 7;
static const u_int uVBSP19_LUMP_LIGHTING = 8;
static const u_int uVBSP19_LUMP_OCCLUSION = 9;
static const u_int uVBSP19_LUMP_LEAVES = 10;
static const u_int uVBSP19_LUMP_FACEIDS = 11;
static const u_int uVBSP19_LUMP_EDGES = 12;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_VBSP19_Lump
{

public:
    
    void ReadFromBitStream( const GLToy_BitStream& xStream ) { xStream >> m_uOffset; xStream >> m_uSize; xStream >> m_uVersion; xStream >> m_uFourCC; }
    void WriteToBitStream( GLToy_BitStream& xStream ) const { xStream << m_uOffset; xStream << m_uSize; xStream << m_uVersion; xStream << m_uFourCC; }

    u_int m_uOffset;
    u_int m_uSize;
    u_int m_uVersion;
    u_int m_uFourCC;

};

class GLToy_VBSP19_LumpDirectory
{

public:
    
    void ReadFromBitStream( const GLToy_BitStream& xStream ) { for( u_int u = 0; u < 64; ++u ) { xStream >> m_axLumps[ u ]; } }
    void WriteToBitStream( GLToy_BitStream& xStream ) const { for( u_int u = 0; u < 64; ++u ) { xStream << m_axLumps[ u ]; } }


    GLToy_VBSP19_Lump m_axLumps[ 64 ];

};

class GLToy_VBSP19_Plane
{

public:
    
    void ReadFromBitStream( const GLToy_BitStream& xStream ) { xStream >> m_xPlane; xStream >> m_uType; }
    void WriteToBitStream( GLToy_BitStream& xStream ) const { xStream << m_xPlane; xStream << m_uType; }


    GLToy_Plane m_xPlane;
    u_int m_uType;

};

class GLToy_VBSP19_Edge
{

public:
    
    void ReadFromBitStream( const GLToy_BitStream& xStream ) { xStream >> m_ausVertexIndices[ 0 ]; xStream >> m_ausVertexIndices[ 1 ]; }
    void WriteToBitStream( GLToy_BitStream& xStream ) const { xStream << m_ausVertexIndices[ 0 ]; xStream << m_ausVertexIndices[ 1 ]; }


    u_short m_ausVertexIndices[ 2 ];

};

class GLToy_VBSP19_Face
{

public:
    
    void ReadFromBitStream( const GLToy_BitStream& xStream )
    {
        xStream >> m_usPlane;
        xStream >> m_cSide;
        xStream >> m_uIsNode;
        xStream >> m_iFirstEdge;
        xStream >> m_sEdgeCount;
        xStream >> m_sTextureInfo;
        xStream >> m_sDisplacementInfo;
        xStream >> m_sSurfaceFogVolumeID;
        xStream >> m_aucLightmapStyles[ 0 ];
        xStream >> m_aucLightmapStyles[ 1 ];
        xStream >> m_aucLightmapStyles[ 2 ];
        xStream >> m_aucLightmapStyles[ 3 ];
        xStream >> m_iLightmapOffset;
        xStream >> m_fArea;
        xStream >> m_iLightmapTextureMins[ 0 ];
        xStream >> m_iLightmapTextureMins[ 1 ];
        xStream >> m_iLightmapTextureSize[ 0 ];
        xStream >> m_iLightmapTextureSize[ 1 ];
        xStream >> m_iOriginalFaceID;
        xStream >> m_usPrimitiveCount;
        xStream >> m_usFirstPrimitiveID;
        xStream >> m_uSmoothingGroups;
    }

    void WriteToBitStream( GLToy_BitStream& xStream ) const
    {
        xStream << m_usPlane;
        xStream << m_cSide;
        xStream << m_uIsNode;
        xStream << m_iFirstEdge;
        xStream << m_sEdgeCount;
        xStream << m_sTextureInfo;
        xStream << m_sDisplacementInfo;
        xStream << m_sSurfaceFogVolumeID;
        xStream << m_aucLightmapStyles[ 0 ];
        xStream << m_aucLightmapStyles[ 1 ];
        xStream << m_aucLightmapStyles[ 2 ];
        xStream << m_aucLightmapStyles[ 3 ];
        xStream << m_iLightmapOffset;
        xStream << m_fArea;
        xStream << m_iLightmapTextureMins[ 0 ];
        xStream << m_iLightmapTextureMins[ 1 ];
        xStream << m_iLightmapTextureSize[ 0 ];
        xStream << m_iLightmapTextureSize[ 1 ];
        xStream << m_iOriginalFaceID;
        xStream << m_usPrimitiveCount;
        xStream << m_usFirstPrimitiveID;
        xStream << m_uSmoothingGroups;
    }

    u_short    m_usPlane;
    char m_cSide;
    u_char m_uIsNode;
    int m_iFirstEdge;
    short m_sEdgeCount;
    short m_sTextureInfo;
    short m_sDisplacementInfo;
    short m_sSurfaceFogVolumeID;
    u_char m_aucLightmapStyles[ 4 ];
    int m_iLightmapOffset;
    float m_fArea;
    int m_iLightmapTextureMins[ 2 ];
    int m_iLightmapTextureSize[ 2 ];
    int m_iOriginalFaceID;
    u_short m_usPrimitiveCount;
    u_short m_usFirstPrimitiveID;
    u_int m_uSmoothingGroups;

};

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_EnvironmentFile::LoadVBSP19( const GLToy_BitStream& xStream ) const
{
}
