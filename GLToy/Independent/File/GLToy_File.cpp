/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <File/GLToy_File.h>

// GLToy
#include <Core/Data Structures/GLToy_BitStream.h>

// C/C++ headers
#include <stdio.h>
#include <string.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_File::GLToy_File( const GLToy_String& szFilename )
: m_szFilename( szFilename )
{
    FILE* pxFile = _wfopen( m_szFilename.GetWideString(), L"rb" );
    m_uSize = 0;
    if( pxFile )
    {
        fseek( pxFile, 0, SEEK_END );
        m_uSize = ftell( pxFile );
        fclose( pxFile );
    }
}

void GLToy_File::GetAllData( void* const pPointer ) const
{
    char* const pData = static_cast< char* const >( pPointer );

    FILE* pxFile = _wfopen( m_szFilename.GetWideString(), L"rb" );

    if( !pxFile )
    {
        return;
    }

    fread( pData, sizeof( char ), m_uSize, pxFile );

    fclose( pxFile );
}

void GLToy_File::ReadToBitStream( GLToy_BitStream &xStream ) const
{
    char* pcData = new char[ m_uSize ];
    GetAllData( pcData );
    xStream.SetFromByteArray( pcData, m_uSize );
}

void GLToy_File::WriteFromBitStream( const GLToy_BitStream &xStream )
{
    FILE* pxFile = _wfopen( m_szFilename.GetWideString(), L"wb" );

    if( !pxFile )
    {
        return;
    }

    fwrite( xStream.GetData(), sizeof( char ), xStream.GetBytesWritten(), pxFile );
    
    fseek( pxFile, 0, SEEK_END );
    m_uSize = ftell( pxFile );
    
    fclose( pxFile );
}