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

    while( !xStream.IsDoneReading() )
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
            GLToy_Assert( pxEntity != NULL, "Fatal error whilst loading entity file \"%S\"", GetFilename().GetWideString() );
            return;
        }
    }
}

void GLToy_EntityFile::Save( const GLToy_String& szFilename )
{
    GLToy_BitStream xStream;

    xStream << GLToy_HeaderBytes( "ENTS" );

    GLToy_Entity_System::Traverse( GLToy_EntitySaveFunctor( xStream ) );

    GLToy_EntityFile( szFilename ).WriteFromBitStream( xStream );
}