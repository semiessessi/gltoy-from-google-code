/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <File/GLToy_EnvironmentFile.h>

// GLToy
#include <Environment/GLToy_Environment.h>
#include <Environment/GLToy_Environment_System.h>


/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_EnvironmentFile::LoadEnvironment() const
{
    GLToy_BitStream xStream;
    ReadToBitStream( xStream );

    u_int uHeader;
    xStream >> uHeader;

    if( uHeader != GLToy_HeaderBytes( "ENVR" ) )
    {
        GLToy_Assert( uHeader == GLToy_HeaderBytes( "ENVR" ), "Wrong header for environment file \"%S\"! Aborting load.", GetFilename().GetWideString() );
        return;
    }
}

void GLToy_EnvironmentFile::Save( const GLToy_String& szFilename )
{
    GLToy_BitStream xStream;

    xStream << GLToy_HeaderBytes( "ENVR" );

    GLToy_EnvironmentFile( szFilename ).WriteFromBitStream( xStream );
}