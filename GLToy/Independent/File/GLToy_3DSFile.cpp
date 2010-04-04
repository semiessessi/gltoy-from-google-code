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
#include <File/GLToy_3DSFile.h>

// GLToy
//#include <Model/GLToy_Model_3DS.h>
#include <Render/GLToy_Texture.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

struct GLToy_3DS_ChunkHeader
{

    u_short m_usID;
    u_int m_uSize;

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

    GLToy_3DS_ChunkHeader* pxFirstChunk = reinterpret_cast< GLToy_3DS_ChunkHeader* >( pcData );

    // TODO - proper error if the file is crap

    if( pxFirstChunk->m_usID != 0x4D4D )
    {
        GLToy_DebugOutput_Release( "Failed to load .3DS file - unrecognised chunk at start of file" );
        delete[] pcData;
        return NULL;
    }

    if( pxFirstChunk->m_uSize != GetSize() )
    {
        GLToy_DebugOutput_Release( "Failed to load .3DS file - the size of the top level chunk (%d) does not reflect the file size (%d).", pxFirstChunk->m_uSize, GetSize() );
        delete[] pcData;
        return NULL;
    }

    delete[] pcData;

    // TODO - finish

    return NULL;
}