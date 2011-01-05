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
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////
//
// Based on the information at http://www.martinreddy.net/gfx/3d/3DS.spec
// and some other stuff here: http://gpwiki.org/index.php/C:Load3DS
// and even more here: http://www.flipcode.com/archives/3DS_File_Loader.shtml
//
/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <File/GLToy_3DSFile.h>

// GLToy
#include <Maths/GLToy_Maths.h>
#include <Model/GLToy_Model_3DS.h>
#include <Render/GLToy_Texture_System.h>
#include <UI/GLToy_UI_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const u_int u3DS_CHUNK_VERSION = 0x0002;
static const u_int u3DS_CHUNK_EDIT3DS = 0x3D3D;
static const u_int u3DS_CHUNK_KEYF3DS = 0xB000;

static const u_int u3DS_CHUNK_EDIT_MATERIAL = 0xAFFF;
static const u_int u3DS_CHUNK_EDIT_OBJECT = 0x4000;

static const u_int u3DS_CHUNK_OBJ_TRIMESH = 0x4100;

static const u_int u3DS_CHUNK_TRI_VERTS = 0x4110;
static const u_int u3DS_CHUNK_TRI_MOREFACES = 0x4111;
static const u_int u3DS_CHUNK_TRI_FACES = 0x4120;
static const u_int u3DS_CHUNK_TRI_MATERIAL = 0x4130;

//static const u_int u3DS_CHUNK_COLOUR_RGB = 0x0010;
//static const u_int u3DS_CHUNK_COLOUR_TRU = 0x0011;
//static const u_int u3DS_CHUNK_COLOUR_UNK = 0x0013;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

// It seems like overkill and a bad fit to use serialisables here
// #pragma pack is rubbish, but works well enough...
// I assume the lack of considerate alignment is a leftover from 16-bit days... :/
#pragma pack( push )
#pragma pack( 2 )  

struct GLToy_3DS_ChunkHeader
{

    u_short m_usID;
    u_int m_uSize;

};

// this struct is a little heavy with constructors etc.
// maybe it should be a struct for consistency...
// this should align, but I feel like leaving it inside the pragma for emphasis
struct GLToy_3DS_Triangle
{

    GLToy_3DS_Triangle()
    {
        m_auIDs[ 0 ] = m_auIDs[ 1 ] = m_auIDs[ 2 ] = 0xFFFF;
    }

    GLToy_3DS_Triangle( const GLToy_3DS_Triangle& xTriangle )
    {
        for( u_int u = 0; u < 3; ++u )
        {
            m_auIDs[ u ] = xTriangle.m_auIDs[ u ];
        }
    }

    GLToy_3DS_Triangle& operator =( const GLToy_3DS_Triangle& xTriangle )
    {
        for( u_int u = 0; u < 3; ++u )
        {
            m_auIDs[ u ] = xTriangle.m_auIDs[ u ];
        }

        return *this;
    }

    u_short m_auIDs[ 3 ];
    u_short m_uFlags;

};

#pragma pack( pop )

typedef GLToy_Vector_3 GLToy_3DS_Vertex;
typedef GLToy_Vector_2 GLToy_3DS_UV;

struct GLToy_3DS_TempObject
{

    GLToy_3DS_TempObject()
    : m_xVertices()
    , m_xUVs()
    , m_xFaces()
    {
    }

    GLToy_Array< GLToy_3DS_Vertex > m_xVertices;
    GLToy_Array< GLToy_3DS_UV > m_xUVs;
    GLToy_Array< GLToy_3DS_Triangle > m_xFaces;

};

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Model* GLToy_3DSFile::LoadModel() const
{
    if( m_pxModel )
    {
        return m_pxModel;
    }

    char* pcData = new char[ GetSize() ];
    GetAllData( pcData );

    char* pcCurrent = pcData;
    GLToy_3DS_ChunkHeader* pxChunkHeader = reinterpret_cast< GLToy_3DS_ChunkHeader* >( pcCurrent );

    if( pxChunkHeader->m_usID != 0x4D4D )
    {
        GLToy_UI_System::ShowErrorDialog( "Failed to load .3DS file - unrecognised chunk at start of file" );
        delete[] pcData;
        return NULL;
    }

    // then the next chunk is immediately after the 2-byte header...
    // pcCurrent += 2; // NOTE - some people say this is correct, looking at files tells me otherwise
    pcCurrent += sizeof( GLToy_3DS_ChunkHeader );

    // check version - apparently its at byte 29
    if( pcData[ 28 ] < 3 )
    {
        GLToy_DebugOutput_Release( "Failed to load .3DS file - the file uses an old version (%d) which is not supported.", static_cast< u_int >( pcData[ 28 ] ) );
        delete[] pcData;
        return NULL;
    }

    GLToy_Array< GLToy_3DS_TempObject > xObjects;

    // load chunks...
    while( static_cast< u_int >( pcCurrent - pcData ) < GetSize() )
    {
        pxChunkHeader = reinterpret_cast< GLToy_3DS_ChunkHeader* >( pcCurrent ); 
        
        if( pxChunkHeader->m_uSize > GetSize() )
        {
            GLToy_Assert( pxChunkHeader->m_uSize <= GetSize(), "Error: Chunks should not be larger than the whole file - aborting load" );
            delete[] pcData;
            return NULL;
        }

        switch( pxChunkHeader->m_usID )
        {
            case u3DS_CHUNK_VERSION:
            {
                // version we already have...
                pcCurrent += pxChunkHeader->m_uSize;
                break;
            }
            
            case u3DS_CHUNK_EDIT3DS:
            {
                pcCurrent += sizeof( GLToy_3DS_ChunkHeader );
                // after this we load sub-chunks of this one...
                break;
            }

            case u3DS_CHUNK_EDIT_OBJECT:
            {
                // object
                // skip over the name string, which has a max of 20 chars
                pcCurrent += sizeof( GLToy_3DS_ChunkHeader );
                u_int u = 0;
                while( pcCurrent[ u ] && ( u < 20 ) )
                {
                    ++u;
                }

                pcCurrent += u + 1;
                xObjects.Resize( xObjects.GetCount() + 1 ); // not using Append to avoid the copy constructor - a bit lazy
                break;
            }

            case u3DS_CHUNK_OBJ_TRIMESH:
            {
                // mesh
                // this is just a chunk containing other chunks...
                pcCurrent += sizeof( GLToy_3DS_ChunkHeader );
                break;
            }

            case u3DS_CHUNK_TRI_VERTS:
            {
                // vertices
                pcCurrent += sizeof( GLToy_3DS_ChunkHeader );
                u_short usNumVertices = *reinterpret_cast< u_short* >( pcCurrent );
                pcCurrent += sizeof( u_short );

                xObjects.End().m_xVertices.Resize( usNumVertices );

                for( u_short u = 0; u < usNumVertices; ++u )
                {
                    xObjects.End().m_xVertices[ u ] = ( reinterpret_cast< GLToy_3DS_Vertex* >( pcCurrent ) )[ u ];
                }

                pcCurrent += usNumVertices * sizeof( GLToy_3DS_Vertex );
                break;
            }

            case u3DS_CHUNK_TRI_FACES:
            {
                // primary face list - there may be a second one, but lets pretend it doesn't exist
                pcCurrent += sizeof( GLToy_3DS_ChunkHeader );
                u_short usNumFaces = *reinterpret_cast< u_short* >( pcCurrent );
                pcCurrent += sizeof( u_short );

                xObjects.End().m_xFaces.Resize( usNumFaces );

                for( u_short u = 0; u < usNumFaces; ++u )
                {
                    xObjects.End().m_xFaces[ u ] = ( reinterpret_cast< GLToy_3DS_Triangle* >( pcCurrent ) )[ u ];
                }

                pcCurrent += usNumFaces * sizeof( GLToy_3DS_Triangle );

                break;
            }

            case u3DS_CHUNK_TRI_MOREFACES:
            {
                // second face list
                // TODO - work out what this is actually for
                // GLToy_DebugOutput_Release( "Warning: Unhandled chunk id in 3DS file: 0x%X.", pxChunkHeader->m_usID );
                pcCurrent += pxChunkHeader->m_uSize;
                break;
            }

            case u3DS_CHUNK_KEYF3DS:
            {
                // we don't care about keyframes for now... we might care in future
                // ( will we ever /not/ want to use a lightweight "for games" animation format? )
                pcCurrent += pxChunkHeader->m_uSize;
                break;
            }
            
            case 0x4D4D:
            {
                GLToy_Assert( pxChunkHeader->m_usID != 0x4D4D, "Error: There should only be one main chunk in a 3DS file, but we have found at least two - aborting load." );
                delete[] pcData;
                return NULL;
            }

            default:
            {
                GLToy_DebugOutput_Release( "Warning: Unhandled chunk id in 3DS file: 0x%X.", pxChunkHeader->m_usID );
                pcCurrent += pxChunkHeader->m_uSize;
                break;
            }
        };
    }

    delete[] pcData;

    // TODO - finish
    // for now just create a temporary coloured model
    // this wastes loads of time building a vertex list (stupidly) though
    GLToy_Model_3DS* pxModel = new GLToy_Model_3DS();

    pxModel->m_xObjects.Resize( xObjects.GetCount() );

    for( u_int u = 0; u < xObjects.GetCount(); ++u )
    {
        GLToy_3DS_Object& xModelObject = pxModel->m_xObjects[ u ];

        xModelObject.m_xIndices.Resize( xObjects[ u ].m_xFaces.GetCount() * 3 );
        for( u_int v = 0; v < xObjects[ u ].m_xFaces.GetCount(); ++v )
        {
            xModelObject.m_xIndices[ 3 * v ] = xObjects[ u ].m_xFaces[ v ].m_auIDs[ 0 ];
            xModelObject.m_xIndices[ 3 * v + 1 ] = xObjects[ u ].m_xFaces[ v ].m_auIDs[ 1 ];
            xModelObject.m_xIndices[ 3 * v + 2 ] = xObjects[ u ].m_xFaces[ v ].m_auIDs[ 2 ];
        }

        xModelObject.m_xVertices.Resize( xObjects[ u ].m_xVertices.GetCount() );
        for( u_int v = 0; v < xObjects[ u ].m_xVertices.GetCount(); ++v )
        {
            xModelObject.m_xVertices[ v ] =
                GLToy_Vector_3( -( xObjects[ u ].m_xVertices[ v ][ 1 ] ), xObjects[ u ].m_xVertices[ v ][ 2 ], xObjects[ u ].m_xVertices[ v ][ 0 ] );
        }
    }

    GLToy_DebugOutput_Release( "Loaded 3DS model file \"%S\" successfully", m_szFilename.GetWideString() );

    return pxModel;
}
