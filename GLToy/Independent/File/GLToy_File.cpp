/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009, 2010 Semi Essessi
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

// NOTE: We can not support wchar_t* path names because Linux forbids them

GLToy_File::GLToy_File( const GLToy_String& szFilename )
: m_szFilename( szFilename )
{
    char* szPath = m_szFilename.CreateANSIString();
    FILE* pxFile = fopen( szPath, "rb" );
    delete[] szPath;

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

    char* szPath = m_szFilename.CreateANSIString();
    FILE* pxFile = fopen( szPath, "rb" );
    delete[] szPath;

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
    char* szPath = m_szFilename.CreateANSIString();
    FILE* pxFile = fopen( szPath, "wb" );
    delete[] szPath;

    if( !pxFile )
    {
        return;
    }

    fwrite( xStream.GetData(), sizeof( char ), xStream.GetBytesWritten(), pxFile );
    
    fseek( pxFile, 0, SEEK_END );
    m_uSize = ftell( pxFile );
    
    fclose( pxFile );
}
