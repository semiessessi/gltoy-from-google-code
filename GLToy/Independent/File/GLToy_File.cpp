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

GLToy_File::GLToy_File( const char* const szFilename )
{
    // TODO: GLToy_String
    u_int uLength = strlen( szFilename );
    m_szFilename = new char[uLength + 1];
    strncpy_s( m_szFilename, uLength + 1, szFilename, uLength + 1 );

    FILE* pxFile = fopen( m_szFilename, "rb" );
    m_uSize = 0;
    if( pxFile )
    {
        fseek( pxFile, 0, SEEK_END );
        m_uSize = ftell( pxFile );
        fclose( pxFile );
    }
}

GLToy_File::~GLToy_File()
{
    delete m_szFilename;
}

void GLToy_File::GetAllData( void* const pPointer ) const
{
    char* const pData = static_cast< char* const >( pPointer );

    FILE* pxFile = fopen( m_szFilename, "rb" );

    if( !pxFile )
    {
        return;
    }

    fread( pData, sizeof( char ), m_uSize, pxFile );

    fclose( pxFile );
}

void GLToy_File::ReadBitStream( GLToy_BitStream &xStream ) const
{
    // TODO - this does a redundant copy that could be cleaned up by adding a function
    // to bitstream which provides a pointer to its internal data to write n bytes to
    char* pcData = new char[ m_uSize ];
    GetAllData( pcData );
    xStream.WriteData( pcData, m_uSize );
    delete[] pcData;
}

void GLToy_File::WriteBitStream( const GLToy_BitStream &xStream )
{
    FILE* pxFile = fopen( m_szFilename, "rb" );

    if( !pxFile )
    {
        return;
    }

    fwrite( xStream.GetData(), sizeof( char ), xStream.GetBytesWritten(), pxFile );
    
    fseek( pxFile, 0, SEEK_END );
    m_uSize = ftell( pxFile );
    
    fclose( pxFile );
}