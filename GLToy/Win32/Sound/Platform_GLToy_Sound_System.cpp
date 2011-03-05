/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi, Thomas Young
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
#include <Sound/Platform_GLToy_Sound_System.h>

// GLToy
#include <File/GLToy_File_System.h>
#include <File/GLToy_WaveFile.h>
#include <Sound/Platform_GLToy_Sound_Voice.h>

// Xaudio
#include <xaudio2.h>


// Define this to enable xaudio debugging
// #define GLTOY_XAUDIO_DEBUG

/////////////////////////////////////////////////////////////////////////////////////////////
// S T A T I C   M E M B E R   V A R I A B L E S
/////////////////////////////////////////////////////////////////////////////////////////////

IXAudio2* Platform_GLToy_Sound_System::s_pxEngine = 0;
IXAudio2MasteringVoice* Platform_GLToy_Sound_System::s_pxMasteringVoice = 0;

Platform_GLToy_Sound_Voice* Platform_GLToy_Sound_System::s_apxVoices[ uGLTOY_XAUDIO_MAX_VOICES ];
GLToy_HashMap< GLToy_WaveFile* > Platform_GLToy_Sound_System::s_xWaves;

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const unsigned int uGLTOY_XAUDIO_SAMPLE_RATE = 48000;
static const unsigned int uGLTOY_XAUDIO_CHANNELS = 2;         // use XAUDIO2_DEFAULT_CHANNELS to support any number of output channels

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool Platform_GLToy_Sound_System::Initialise()
{
	if( !Initialise_XAudio2() )
	{
		GLToy_DebugOutput( "Failed to initialise XAudio\r\n" );
		return false;
	}

	if( !Initialise_Voices() )
	{
		GLToy_DebugOutput( "Failed to initialise voices\r\n" );
	}

	if( !Initialise_LoadSounds() )
	{
		GLToy_DebugOutput( "Failed to load sounds\r\n" );
		return false;
	}

	return true;
}

bool Platform_GLToy_Sound_System::Initialise_XAudio2()
{
	CoInitializeEx( NULL, COINIT_APARTMENTTHREADED );

	UINT32 uFlags = 0;
	#ifdef GLTOY_XAUDIO_DEBUG
	uFlags |= XAUDIO2_DEBUG_ENGINE;
	#endif

	GLToy_DebugOutput( "Initialising XAudio\r\n" );

	s_pxEngine = NULL;
	HRESULT xResult = XAudio2Create( &s_pxEngine, uFlags );

	if( FAILED( xResult ) || !s_pxEngine )
	{
		return false;
	}

    XAUDIO2_DEBUG_CONFIGURATION xDebugConfig;
    memset( &xDebugConfig, 0, sizeof(xDebugConfig) );
	#ifdef GLTOY_XAUDIO_DEBUG
	xDebugConfig.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
	#endif
    s_pxEngine->SetDebugConfiguration( &xDebugConfig );

	s_pxMasteringVoice = NULL;
	xResult = s_pxEngine->CreateMasteringVoice( &s_pxMasteringVoice, uGLTOY_XAUDIO_CHANNELS, uGLTOY_XAUDIO_SAMPLE_RATE );

	if( FAILED( xResult ) || !s_pxMasteringVoice )
	{
		// We couldn't create a mastering voice with the properties we wanted!
		// Try just letting the sound card work out what it supports
		xResult = s_pxEngine->CreateMasteringVoice( &s_pxMasteringVoice, XAUDIO2_DEFAULT_CHANNELS );
		if( FAILED( xResult ) || !s_pxMasteringVoice )
		{
			s_pxEngine->Release();
			s_pxEngine = 0;
			return false;
		}
	}

	{
		XAUDIO2_DEVICE_DETAILS xDetails;
		s_pxEngine->GetDeviceDetails( 0, &xDetails );

		GLToy_DebugOutput( "XAudio2 device ID: %S\r\n", xDetails.DeviceID );
		GLToy_DebugOutput( "XAudio2 device name: %S\r\n", xDetails.DisplayName );
		GLToy_DebugOutput( "XAudio2 sample rate: %u Hz\r\n", xDetails.OutputFormat.Format.nSamplesPerSec );
		GLToy_DebugOutput( "XAudio2 num channels: %u\r\n", xDetails.OutputFormat.Format.nChannels );
	}

	return true;
}

bool Platform_GLToy_Sound_System::Initialise_Voices()
{
	memset( s_apxVoices, 0, sizeof( Platform_GLToy_Sound_Voice* ) * uGLTOY_XAUDIO_MAX_VOICES );
	return true;
}

bool Platform_GLToy_Sound_System::Initialise_LoadSounds()
{
	// TODO: Probably don't want to load in every sound like this

	GLToy_Array< GLToy_String > xWavePaths = GLToy_File_System::PathsFromFilter( "Sounds/", "*.wav" );

	GLToy_ConstIterate( GLToy_String, szPath, xWavePaths )

		GLToy_String szName = szPath;
		szName.RemoveAt( 0, 7 );      // remove "Sounds/"
		szName.RemoveFromEnd( 4 );    // remove .wav
        
		GLToy_DebugOutput( "   - Found sound \"%S\".\r\n", szName.GetWideString() );

		GLToy_WaveFile* pxWavFile = new GLToy_WaveFile( szPath );
		if( pxWavFile->Load() )
		{
			s_xWaves.AddNode( pxWavFile, szName.GetHash() );
		}
		
	GLToy_Iterate_End;

	return true;
}


void Platform_GLToy_Sound_System::Shutdown()
{
	for( int iVoice = 0; iVoice < uGLTOY_XAUDIO_MAX_VOICES; ++iVoice )
	{
		if( s_apxVoices[ iVoice ] )
		{
			DestroyVoice( iVoice );
		}
	}

	if( s_pxMasteringVoice )
	{
		s_pxMasteringVoice->DestroyVoice();
		s_pxMasteringVoice = NULL;
	}

	if( s_pxEngine )
	{
		s_pxEngine->Release();
	}

	CoUninitialize();
}

void Platform_GLToy_Sound_System::Update()
{
	for( int iVoice = 0; iVoice < uGLTOY_XAUDIO_MAX_VOICES; ++iVoice )
	{
		if( s_apxVoices[ iVoice ] )
		{
			s_apxVoices[ iVoice ]->Update();

			if( !s_apxVoices[ iVoice ]->IsPlaying() && s_apxVoices[ iVoice ]->IsReleased() )
			{
				DestroyVoice( iVoice );
			}
		}
	}
}

GLToy_Handle Platform_GLToy_Sound_System::CreateVoice( const GLToy_Hash uWaveHash )
{
	// Find a free voice slot
	for( int iVoice = 0; iVoice < uGLTOY_XAUDIO_MAX_VOICES; ++iVoice )
	{
		if( !s_apxVoices[ iVoice ] )
		{
			s_apxVoices[ iVoice ] = new Platform_GLToy_Sound_Voice( uWaveHash );
			return iVoice;
		}
	}
	return uGLTOY_INVALID_HANDLE;
}

GLToy_Sound_Voice* Platform_GLToy_Sound_System::GetVoice( const GLToy_Handle iHandle )
{
	if( ( iHandle >= 0 ) && ( iHandle < uGLTOY_XAUDIO_MAX_VOICES ) )
	{
		return s_apxVoices[ iHandle ];
	}
	return 0;
}

void Platform_GLToy_Sound_System::DestroyVoice( const GLToy_Handle iHandle )
{
	if( ( iHandle >= 0 ) && ( iHandle < uGLTOY_XAUDIO_MAX_VOICES ) )
	{
		delete s_apxVoices[ iHandle ];
		s_apxVoices[ iHandle ] = 0;
	}
}

GLToy_WaveFile* Platform_GLToy_Sound_System::GetWave( GLToy_Hash uHash )
{
	return *s_xWaves.FindData( uHash );
}

IXAudio2* Platform_GLToy_Sound_System::GetXAudio2Engine()
{
	return s_pxEngine;
}

//eof

