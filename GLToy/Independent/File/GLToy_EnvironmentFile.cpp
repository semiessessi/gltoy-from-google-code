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
    return ( uHeader == GLToy_HeaderBytes( "ENVR" ) || uHeader == GLToy_HeaderBytes( "IBSP" ) || uHeader == GLToy_HeaderBytes( "VBSP" ) );
}

GLToy_String GLToy_EnvironmentFile::GetName() const
{
    GLToy_String szName = GetFilename();
    szName.RemoveAt( 0, 13 ); // remove "Environments/"
    szName.RemoveFromEnd( 4 ); // remove .env

    return szName;
}

void GLToy_EnvironmentFile::LoadEnvironment() const
{
    GLToy_Environment_System::ClearCurrentEnvironment();

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
            //// Quake / Half-Life
            //case 30:
            //{
            //    return LoadBSP30( xStream );
            //}
            // Quake 2 / Kingpin
            case 38:
            {
                return LoadBSP38( xStream );
            }
            // Quake 3
            case 46:
            {
                return LoadBSP46( xStream );
            }

            case 0:
            default:
            {
                // TODO - some kind of modal dialog system
                GLToy_Assert( false, "Version %d id BSP files are not supported by GLToy.", uVersion );
                break;
            }
        };
    }

    if( uHeader == GLToy_HeaderBytes( "VBSP" ) )
    {
        switch( uVersion )
        {
            // TODO - it would be nice to support all of these - but maybe a *little* ambitious
            //// Source (pre-HDR)
            //case 19:
            //{
            //    return LoadVBSP19( xStream );
            //}
            //// Source
            //case 20:
            //{
            //    return LoadVBSP20( xStream );
            //}
            //// L4D2
            //case 21:
            //{
            //    return LoadVBSP21( xStream );
            //}
            case 0:
            default:
            {
                // TODO - some kind of modal dialog system
                GLToy_Assert( false, "Version %d Valve BSP files are not supported by GLToy.", uVersion );
                break;
            }
        };
    }

    uVersion -= 0x80000000;

    // version handling would go here

    u_int uType;

    xStream.ReadBits( uType, 3 );

    GLToy_Environment* pxEnv = GLToy_Environment_System::CreateEnvironmentFromType( static_cast< GLToy_EnvironmentType >( uType ) );

    if( !pxEnv )
    {
        // TODO - some kind of user feedback
        return;
    }

    pxEnv->ReadFromBitStream( xStream );

    GLToy_DebugOutput_Release( "Loaded environment file \"%S\" successfully", m_szFilename.GetWideString() );
    GLToy_Environment_System::SetCurrentEnvironment( pxEnv );
}

void GLToy_EnvironmentFile::Save( const GLToy_String& szFilename )
{
    const GLToy_Environment* const pxEnv = GLToy_Environment_System::GetCurrentEnvironment();

    if( !pxEnv )
    {
        return;
    }

    GLToy_BitStream xStream;

    xStream << GLToy_HeaderBytes( "ENVR" );
    xStream << uCURRENT_VERSION + 0x80000000;

    xStream.WriteBits( pxEnv->GetType(), 3 );
    pxEnv->WriteToBitStream( xStream );

    GLToy_EnvironmentFile( szFilename ).WriteFromBitStream( xStream );

    GLToy_DebugOutput_Release( "Saved environment file \"%S\" successfully", szFilename.GetWideString() );
}