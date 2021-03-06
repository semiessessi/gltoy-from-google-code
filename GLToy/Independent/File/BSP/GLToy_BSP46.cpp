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
#include <Environment/GLToy_Environment.h>
#include <Environment/GLToy_Environment_Lightmapped.h>
#include <Environment/GLToy_Environment_System.h>

// TODO - finish BSP v46 environment geometry loading

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const u_int uBSP46_LUMP_ENTITIES = 0;
static const u_int uBSP46_LUMP_TEXTURES = 1;
static const u_int uBSP46_LUMP_PLANES = 2;
static const u_int uBSP46_LUMP_NODES = 3;
static const u_int uBSP46_LUMP_LEAFS = 4;
static const u_int uBSP46_LUMP_VERTICES = 10;
static const u_int uBSP46_LUMP_FACES = 13;
static const u_int uBSP46_LUMP_VIS = 16;

static const u_int uBSP46_FACE_POLYGON = 1;
static const u_int uBSP46_FACE_PATCH = 2;
static const u_int uBSP46_FACE_MESH = 3;
static const u_int uBSP46_FACE_SPRITE = 4;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_BSP46_Lump
{

public:
    
    void ReadFromBitStream( const GLToy_BitStream& xStream ) { xStream >> m_uOffset; xStream >> m_uSize; }
    void WriteToBitStream( GLToy_BitStream& xStream ) const { xStream << m_uOffset; xStream << m_uSize; }

    u_int m_uOffset;
    u_int m_uSize;

};

class GLToy_BSP46_LumpDirectory
{

public:
    
    void ReadFromBitStream( const GLToy_BitStream& xStream ) { for( u_int u = 0; u < 17; ++u ) { xStream >> m_axLumps[ u ]; } }
    void WriteToBitStream( GLToy_BitStream& xStream ) const { for( u_int u = 0; u < 17; ++u ) { xStream << m_axLumps[ u ]; } }


    GLToy_BSP46_Lump m_axLumps[ 17 ];

};

class GLToy_BSP46_Vertex
{

public:

    void ReadFromBitStream( const GLToy_BitStream& xStream )
    {
        xStream >> m_xVertex;
        xStream >> m_xUV;
        xStream >> m_xLightmapUV;
        xStream >> m_xNormal;
        xStream >> m_uRGBA;
    }

    void WriteToBitStream( GLToy_BitStream& xStream ) const
    {
        xStream << m_xVertex;
        xStream << m_xUV;
        xStream << m_xLightmapUV;
        xStream << m_xNormal;
        xStream << m_uRGBA;
    }

    static size_t GetSize() { return sizeof( GLToy_BSP46_Vertex ) - sizeof( void ( * )() ); }  // subtract vtable pointer size

    GLToy_Vector_3 m_xVertex;
    GLToy_Vector_2 m_xUV;
    GLToy_Vector_2 m_xLightmapUV;
    GLToy_Vector_3 m_xNormal;
    u_int m_uRGBA;

};

class GLToy_BSP46_Face
{

public:

    void ReadFromBitStream( const GLToy_BitStream& xStream )
    {
        xStream >> m_iTextureIndex;
        xStream >> m_iEffect;
        xStream >> m_uType;
        xStream >> m_iFirstVertex;
        xStream >> m_uNumVertices;
        xStream >> m_iFirstMeshVertex;
        xStream >> m_uNumMeshVertices;
        xStream >> m_iLightmapIndex;
        xStream >> m_aiLightmapStart[ 0 ];
        xStream >> m_aiLightmapStart[ 1 ];
        xStream >> m_auLightmapSize[ 0 ];
        xStream >> m_auLightmapSize[ 1 ];
        xStream >> m_xLightmapOrigin;
        xStream >> m_axLightmapST[ 0 ];
        xStream >> m_axLightmapST[ 1 ];
        xStream >> m_xNormal;
        xStream >> m_auPatchSize[ 0 ];
        xStream >> m_auPatchSize[ 1 ];
    }

    void WriteToBitStream( GLToy_BitStream& xStream ) const
    {
        xStream << m_iTextureIndex;
        xStream << m_iEffect;
        xStream << m_uType;
        xStream << m_iFirstVertex;
        xStream << m_uNumVertices;
        xStream << m_iFirstMeshVertex;
        xStream << m_uNumMeshVertices;
        xStream << m_iLightmapIndex;
        xStream << m_aiLightmapStart[ 0 ];
        xStream << m_aiLightmapStart[ 1 ];
        xStream << m_auLightmapSize[ 0 ];
        xStream << m_auLightmapSize[ 1 ];
        xStream << m_xLightmapOrigin;
        xStream << m_axLightmapST[ 0 ];
        xStream << m_axLightmapST[ 1 ];
        xStream << m_xNormal;
        xStream << m_auPatchSize[ 0 ];
        xStream << m_auPatchSize[ 1 ];
    }

    static size_t GetSize() { return sizeof( GLToy_BSP46_Face ) - sizeof( void ( * )() ); }  // subtract vtable pointer size

    int m_iTextureIndex;
    int m_iEffect;
    u_int m_uType;
    int m_iFirstVertex;
    u_int m_uNumVertices;
    int m_iFirstMeshVertex;
    u_int m_uNumMeshVertices;
    int m_iLightmapIndex;
    int m_aiLightmapStart[ 2 ];
    u_int m_auLightmapSize[ 2 ];
    GLToy_Vector_3 m_xLightmapOrigin;
    GLToy_Vector_3 m_axLightmapST[ 2 ];
    GLToy_Vector_3 m_xNormal;
    u_int m_auPatchSize[ 2 ];

};


/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_EnvironmentFile::LoadBSP46( const GLToy_BitStream& xStream ) const
{
    GLToy_Environment_Lightmapped* pxEnv = static_cast< GLToy_Environment_Lightmapped* >( GLToy_Environment_System::CreateEnvironmentFromType( ENV_LIGHTMAPPED ) );

    if( !pxEnv )
    {
        GLToy_Assert( pxEnv != NULL, "Failed to create lightmapped environment" );
        return;
    }

    GLToy_BSP46_LumpDirectory xLumps;
    xStream >> xLumps;

    // vertices
    GLToy_Array< GLToy_BSP46_Vertex > xVertices;
    xVertices.Resize( xLumps.m_axLumps[ uBSP46_LUMP_VERTICES ].m_uSize / GLToy_BSP46_Vertex::GetSize() );
    xStream.SetReadByte( xLumps.m_axLumps[ uBSP46_LUMP_VERTICES ].m_uOffset );
    for( u_int u = 0; u < xVertices.GetCount(); ++u )
    {
        xStream >> xVertices[ u ];
    }

    // faces
    GLToy_Array< GLToy_BSP46_Face > xFaces;
    xFaces.Resize( xLumps.m_axLumps[ uBSP46_LUMP_FACES ].m_uSize / GLToy_BSP46_Face::GetSize() );
    xStream.SetReadByte( xLumps.m_axLumps[ uBSP46_LUMP_FACES ].m_uOffset );

    for( u_int u = 0; u < xFaces.GetCount(); ++u )
    {
        xStream >> xFaces[ u ];
    }

    // set up vertices
    pxEnv->m_xVertices.Resize( xVertices.GetCount() );
    for( u_int u = 0; u < xVertices.GetCount(); ++u )
    {
        pxEnv->m_xVertices[ u ].m_xPosition = GLToy_Vector_3( -( xVertices[ u ].m_xVertex[ 1 ] ), xVertices[ u ].m_xVertex[ 2 ], xVertices[ u ].m_xVertex[ 0 ] );
        pxEnv->m_xVertices[ u ].m_xUV = xVertices[ u ].m_xUV;
        pxEnv->m_xVertices[ u ].m_xLightmapUV = xVertices[ u ].m_xLightmapUV;
        pxEnv->m_xVertices[ u ].m_xNormal = xVertices[ u ].m_xNormal;
        // pxEnv->m_xVertices[ u ].m_xColour = xVertices[ u ].m_uRGBA;
    }

    // set up faces
    pxEnv->m_xFaces.Resize( xFaces.GetCount() );
    for( u_int u = 0; u < xFaces.GetCount(); ++u )
    {
        pxEnv->m_xFaces[ u ].m_uTextureHash = uGLTOY_BAD_HASH;
        
        const u_int uType = xFaces[ u ].m_uType;
        switch( uType )
        {

            case uBSP46_FACE_POLYGON:
            {
                pxEnv->m_xFaces[ u ].m_bVisible = true;
                pxEnv->m_xFaces[ u ].m_xIndices.Resize( xFaces[ u ].m_uNumVertices );
                for( u_int v = 0; v < xFaces[ u ].m_uNumVertices; ++v )
                {
                    pxEnv->m_xFaces[ u ].m_xIndices[ v ] = xFaces[ u ].m_iFirstVertex + v;
                }

                break;
            }

            case uBSP46_FACE_PATCH:
            case uBSP46_FACE_MESH:
            case uBSP46_FACE_SPRITE:
            {
                break;
            }

            default:
            {
                GLToy_Assert( false, "Bad face type in BSP file" );
                return;
            }

        }
    }

    GLToy_Environment_System::SetCurrentEnvironment( pxEnv );
}
