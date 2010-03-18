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
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const u_int uCURRENT_VERSION = 0;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Inline const bool GLToy_EnvironmentFile_IsFileHeaderValid( const u_int uHeader )
{
    return ( uHeader == GLToy_HeaderBytes( "ENVR" ) || uHeader == GLToy_HeaderBytes( "IBSP" ) );
}

void GLToy_EnvironmentFile::LoadEnvironment() const
{
    GLToy_BitStream xStream;
    ReadToBitStream( xStream );

    u_int uHeader = 0;
    u_int uVersion = 0;
    xStream >> uHeader;
    xStream >> uVersion;

    if( !GLToy_EnvironmentFile_IsFileHeaderValid( uHeader ) )
    {
        GLToy_Assert( GLToy_EnvironmentFile_IsFileHeaderValid( uHeader ), "Wrong header for environment file \"%S\"! Aborting load.", GetFilename().GetWideString() );
        return;
    }

    if( uHeader == GLToy_HeaderBytes( "IBSP" ) )
    {
        switch( uVersion )
        {
            // TODO - it would be nice to support all of these - but maybe a *little* ambitious
            //// Quake
            //case 29:
            //{
            //    return LoadBSP29( xStream );
            //}
            //// Half-Life
            //case 30:
            //{
            //    return LoadBSP30( xStream );
            //}
            //// Quake 2
            //case 38:
            //{
            //    return LoadBSP38( xStream );
            //}
            //// Quake 3
            //case 46:
            //{
            //    return LoadBSP46( xStream );
            //}

            case 0:
            default:
            {
                // TODO - some kind of modal dialog system
                GLToy_Assert( false, "Version %d BSP files are not supported by GLToy.", uVersion );
                break;
            }
        };
    }

    uVersion -= 0x80000000;
}

void GLToy_EnvironmentFile::Save( const GLToy_String& szFilename )
{
    GLToy_BitStream xStream;

    xStream << GLToy_HeaderBytes( "ENVR" );
    xStream << uCURRENT_VERSION + 0x80000000;

    GLToy_EnvironmentFile( szFilename ).WriteFromBitStream( xStream );
}