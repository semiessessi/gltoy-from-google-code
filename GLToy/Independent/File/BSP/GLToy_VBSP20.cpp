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

static const u_int uVBSP20_LUMP_ENTITIES = 0;
static const u_int uVBSP20_LUMP_PLANES = 1;
static const u_int uVBSP20_LUMP_TEXTUREDATA = 2;
static const u_int uVBSP20_LUMP_VERTICES = 3;
static const u_int uVBSP20_LUMP_VISIBILITY = 4;
static const u_int uVBSP20_LUMP_NODES = 5;
static const u_int uVBSP20_LUMP_TEXTUREINFO = 6;
static const u_int uVBSP20_LUMP_FACES = 7;
static const u_int uVBSP20_LUMP_LIGHTING = 8;
static const u_int uVBSP20_LUMP_OCCLUSION = 9;
static const u_int uVBSP20_LUMP_LEAVES = 10;
static const u_int uVBSP20_LUMP_FACEIDS = 11;
static const u_int uVBSP20_LUMP_EDGES = 12;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_VBSP20_Lump
: public GLToy_Serialisable
{

public:
    
    virtual void ReadFromBitStream( const GLToy_BitStream& xStream ) { xStream >> m_uOffset; xStream >> m_uSize; xStream >> m_uVersion; xStream >> m_uFourCC; }
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const { xStream << m_uOffset; xStream << m_uSize; xStream << m_uVersion; xStream << m_uFourCC; }

    u_int m_uOffset;
    u_int m_uSize;
    u_int m_uVersion;
    u_int m_uFourCC;

};

class GLToy_VBSP20_LumpDirectory
: public GLToy_Serialisable
{

public:
    
    virtual void ReadFromBitStream( const GLToy_BitStream& xStream ) { for( u_int u = 0; u < 64; ++u ) { xStream >> m_axLumps[ u ]; } }
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const { for( u_int u = 0; u < 64; ++u ) { xStream << m_axLumps[ u ]; } }


    GLToy_VBSP20_Lump m_axLumps[ 64 ];

};

class GLToy_VBSP20_Plane
: public GLToy_Serialisable
{

public:
    
    virtual void ReadFromBitStream( const GLToy_BitStream& xStream ) { xStream >> m_xPlane; xStream >> m_uType; }
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const { xStream << m_xPlane; xStream << m_uType; }


    GLToy_Plane m_xPlane;
    u_int m_uType;

};

class GLToy_VBSP20_Edge
: public GLToy_Serialisable
{

public:
    
    virtual void ReadFromBitStream( const GLToy_BitStream& xStream ) { xStream >> m_ausVertexIndices[ 0 ]; xStream >> m_ausVertexIndices[ 1 ]; }
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const { xStream << m_ausVertexIndices[ 0 ]; xStream << m_ausVertexIndices[ 1 ]; }


    u_short m_ausVertexIndices[ 2 ];

};

class GLToy_VBSP20_Face
: public GLToy_Serialisable
{

public:
    
    virtual void ReadFromBitStream( const GLToy_BitStream& xStream )
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

    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const
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

	u_short	m_usPlane;
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

void GLToy_EnvironmentFile::LoadVBSP20( const GLToy_BitStream& xStream ) const
{
}
