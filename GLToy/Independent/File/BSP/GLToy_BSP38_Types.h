/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2011 Semi Essessi
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

#ifndef __GLTOY_BSP38_TYPES_H_
#define __GLTOY_BSP38_TYPES_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/Data Structures/GLToy_BitStream.h>
#include <Core/Data Structures/GLToy_HashMap.h>
#include <Maths/GLToy_Maths.h>
#include <String/GLToy_String.h>

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
#ifdef GLTOY_DEBUG
        const GLToy_Hash uHash = GLToy_String( m_szTextureName ).GetHash();
        if( !s_xTextureNames.FindData( uHash ) )
        {
            s_xTextureNames.AddNode( m_szTextureName, uHash );
        }
#endif
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

#ifdef GLTOY_DEBUG
    static GLToy_HashMap< GLToy_String > s_xTextureNames;
#endif

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

#endif