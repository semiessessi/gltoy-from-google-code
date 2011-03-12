/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
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

#ifndef GLTOY_DEMO

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <File/GLToy_WaveFile.h>

// GLToy
#include <Sound/GLToy_Sound_System.h>


/////////////////////////////////////////////////////////////////////////////////////////////
// D E F I N E S
/////////////////////////////////////////////////////////////////////////////////////////////

#define GLTOY_FOURCC(ch0, ch1, ch2, ch3) \
                    ((unsigned int)(unsigned char)(ch0) | ((unsigned int)(unsigned char)(ch1) << 8) | \
                    ((unsigned int)(unsigned char)(ch2) << 16) | ((unsigned int)(unsigned char)(ch3) << 24 ))

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

const u_int uGLTOY_4CC_DATA = GLTOY_FOURCC( 'd', 'a', 't', 'a' );
const u_int uGLTOY_4CC_FMT  = GLTOY_FOURCC( 'f', 'm', 't', ' ' );
const u_int uGLTOY_4CC_RIFF = GLTOY_FOURCC( 'R', 'I', 'F', 'F' );
const u_int uGLTOY_4CC_WAVE = GLTOY_FOURCC( 'W', 'A', 'V', 'E' );

const u_int uGLTOY_WAVE_READ_SIZE = 1024 * 16;

/////////////////////////////////////////////////////////////////////////////////////////////
// S T R U C T U R E S
/////////////////////////////////////////////////////////////////////////////////////////////

struct Riff_Chunk
{
	u_int uChunkType;
	u_int uChunkSize;
};

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_WaveFile::GLToy_WaveFile( const GLToy_String& szFilename ) : GLToy_File( szFilename )
{
	m_uBufferSize = 0;
	m_pBuffer = 0;
	memset( &m_xFormat, 0, sizeof( WAVEFORMATEX ) );
}

GLToy_WaveFile::~GLToy_WaveFile()
{
	delete []m_pBuffer;
}

const WAVEFORMATEX& GLToy_WaveFile::GetWaveFormat()
{
	return m_xFormat;
}

void GLToy_WaveFile::GetBuffer( void*& pBuffer, unsigned int& uBufferSize )
{
	uBufferSize = m_uBufferSize;
	pBuffer = m_pBuffer;
}

bool GLToy_WaveFile::Load()
{
	FILE* pxFile = _wfopen( GetFilename().GetDataPointer(), L"rb" );
	
	if( !pxFile )
	{
		return false;
	}

	unsigned int uFileSize = 0;

	// Read RIFF header
	{
		Riff_Chunk xRiff;

		if( fread( &xRiff, sizeof( Riff_Chunk ), 1, pxFile ) == 0 )
		{
			fclose( pxFile );
			return false;
		}

		if( xRiff.uChunkType != uGLTOY_4CC_RIFF ) 
		{
			fclose( pxFile );
			return false;
		}

		// File size is RIFF chunk size + the above chunk header
		uFileSize = xRiff.uChunkSize + 8;
	}

	// RIFF chunk contents are the subtype FOURCC then subtype-dependent data

	unsigned int uSubType;
	if( fread( &uSubType, sizeof( unsigned int ), 1, pxFile ) == 0 )
	{
		fclose( pxFile );
		return false;
	}

	if( uSubType != uGLTOY_4CC_WAVE ) 
	{
		fclose( pxFile );
		return false;
	}

	while( ftell( pxFile ) < static_cast<int>( uFileSize ) )
	{
		if( !ParseChunk( pxFile ) )
		{
			fclose( pxFile );
			return false;
		}
	}

	fclose( pxFile );
	return true;
}

bool GLToy_WaveFile::ParseChunk( FILE* pxFile )
{
	Riff_Chunk xRiff;

	if( fread( &xRiff, sizeof( Riff_Chunk ), 1, pxFile ) == 0 )
	{
		return false;
	}

	switch( xRiff.uChunkType )
	{
		case uGLTOY_4CC_DATA:
		{
            m_uBufferSize = xRiff.uChunkSize;
			m_pBuffer = new char[ m_uBufferSize ];

			unsigned int uToRead = m_uBufferSize;

			while( uToRead > 0 )
			{
				u_int uReadSize = uGLTOY_WAVE_READ_SIZE;
				if( uToRead < uGLTOY_WAVE_READ_SIZE )
				{
					uReadSize = uToRead;
				}

				if( fread( ( reinterpret_cast<char*>(m_pBuffer) + ( m_uBufferSize - uToRead ) ), uReadSize, 1, pxFile ) == 0 )
				{
					return false;
				}

				uToRead -= uReadSize;
			}

			return true;
		}
		break;

		case uGLTOY_4CC_FMT:
		{
			char acBuffer[512];

			if( fread( &acBuffer, xRiff.uChunkSize, 1, pxFile ) == 0 )
			{
				return false;
			}

			memcpy( &m_xFormat, &acBuffer[0], sizeof( WAVEFORMATEX ) );
			
			// Must be 16 bit PCM
			if( ( m_xFormat.wFormatTag != WAVE_FORMAT_PCM ) || ( m_xFormat.wBitsPerSample != 16 ) )
			{
				return false;
			}

			return true;
		}
		break;

		default:
		{
			// Unknown chunk, skip it
			fseek( pxFile, xRiff.uChunkSize, SEEK_CUR );
			return true;
		}
	}

	// shouldnt get here
	return false;
}

#endif
