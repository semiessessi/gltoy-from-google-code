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
#include <File/GLToy_EntityFile.h>

// GLToy
#include <Entity/GLToy_Entity.h>
#include <Entity/GLToy_Entity_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_EntitySaveFunctor
: public GLToy_ConstFunctor< GLToy_Entity* >
{

public:

    GLToy_EntitySaveFunctor( GLToy_BitStream& xBitStream )
    : m_xStream( xBitStream )
    {
    }

    void operator()( GLToy_Entity* const* const pxEntity )
    {
        ( *pxEntity )->WriteToBitStream( m_xStream );
    }

protected:

    GLToy_BitStream& m_xStream;

};

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_EntityFile::LoadEntities() const
{
    GLToy_BitStream xStream;
    ReadToBitStream( xStream );

    if( xStream.GetBitsWritten() == 0 )
    {
        GLToy_Assert( xStream.GetBitsWritten() != 0, "No data in entity file \"%S\"! Aborting load.", GetFilename().GetWideString() );
        return;
    }

    u_int uHeader;
    xStream >> uHeader;

    if( uHeader != GLToy_HeaderBytes( "ENTS" ) )
    {
        GLToy_Assert( uHeader == GLToy_HeaderBytes( "ENTS" ), "Wrong header for entity file \"%S\"! Aborting load.", GetFilename().GetWideString() );
        return;
    }

    GLToy_Entity_System::DestroyEntities();

    while( xStream.HasNMoreBytes( 5 ) )
    {
        u_char ucType;
        GLToy_Hash uHash;

        xStream >> ucType;
        xStream >> uHash;

        GLToy_Entity* pxEntity = GLToy_Entity_System::CreateEntity( uHash, static_cast< GLToy_EntityType >( ucType ) );

        if( pxEntity )
        {
            pxEntity->ReadFromBitStream( xStream );
        }
        else
        {
            GLToy_Assert( pxEntity != NULL, "Fatal error whilst loading entity file \"%S\" - can not continue processing stream", GetFilename().GetWideString() );
            return;
        }
    }

    GLToy_DebugOutput_Release( "Loaded entity file \"%S\" successfully", m_szFilename.GetWideString() );
}

void GLToy_EntityFile::Save( const GLToy_String& szFilename )
{
    GLToy_BitStream xStream;

    xStream << GLToy_HeaderBytes( "ENTS" );

    GLToy_Entity_System::Traverse( GLToy_EntitySaveFunctor( xStream ) );

    GLToy_EntityFile( szFilename ).WriteFromBitStream( xStream );

    GLToy_DebugOutput_Release( "Saved entity file \"%S\" successfully", szFilename.GetWideString() );
}
