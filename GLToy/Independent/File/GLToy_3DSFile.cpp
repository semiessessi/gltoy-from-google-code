/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010 Semi Essessi
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
// 3DS file loader
//
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
//#include <Model/GLToy_Model_3DS.h>
#include <Model/GLToy_Model_Coloured.h> // TODO - remove this once done with it
#include <Render/GLToy_Texture.h>

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

    // TODO - proper error if the file is crap

    if( pxChunkHeader->m_usID != 0x4D4D )
    {
        GLToy_DebugOutput_Release( "Failed to load .3DS file - unrecognised chunk at start of file" );
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
                // TODO - load this instead of ignoring it
                GLToy_DebugOutput_Release( "Warning: Unhandled chunk id in 3DS file: 0x%X. This chunk contains extra faces for the object, which will be missing.", pxChunkHeader->m_usID );
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
    GLToy_Model_Coloured* pxModel = new GLToy_Model_Coloured();
    pxModel->SetBBToPoint( xObjects[ 0 ].m_xVertices[ xObjects[ 0 ].m_xFaces[ 0 ].m_auIDs[ 0 ] ] );

    GLToy_ConstIterate( GLToy_3DS_TempObject, xObjectIterator, &xObjects )
    {
        const GLToy_3DS_TempObject& xObject = xObjectIterator.Current();
        GLToy_ConstIterate( GLToy_3DS_Triangle, xIterator, &( xObject.m_xFaces ) )
        {
            const u_int uIndex1 = xIterator.Current().m_auIDs[ 0 ];
            const u_int uIndex2 = xIterator.Current().m_auIDs[ 1 ];
            const u_int uIndex3 = xIterator.Current().m_auIDs[ 2 ];
            const GLToy_Vector_3 xV1 = xObject.m_xVertices[ uIndex1 ];
            const GLToy_Vector_3 xV2 = xObject.m_xVertices[ uIndex2 ];
            const GLToy_Vector_3 xV3 = xObject.m_xVertices[ uIndex3 ];
            pxModel->AddStripFromTriangle( xV1, xV2, xV3, GLToy_Vector_3( 1.0f, 1.0f, 1.0f ) );
            pxModel->GrowBBByPoint( xV1 );
            pxModel->GrowBBByPoint( xV2 );
            pxModel->GrowBBByPoint( xV3 );
        }
    }

    // fix up positions so they are relative to the bb position, and the bb is centered
    for( u_int u = 0; u < pxModel->m_xVertices.GetCount(); ++u )
    {
        pxModel->m_xVertices[ u ] -= pxModel->GetBB().GetPosition();
    }

    pxModel->SetBB( pxModel->GetBB() - pxModel->GetBB().GetPosition() );

    GLToy_DebugOutput_Release( "Loaded 3DS model file \"%S\" successfully", m_szFilename.GetWideString() );

    return pxModel;
}