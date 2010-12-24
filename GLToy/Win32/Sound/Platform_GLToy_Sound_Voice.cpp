
// Thomas Young 23.12.10

#include <Core/GLToy.h>

#include <File/GLToy_WaveFile.h>

#include <Sound/Platform_GLToy_Sound_System.h>
#include <Sound/Platform_GLToy_Sound_Voice.h>


Platform_GLToy_Sound_Voice::Platform_GLToy_Sound_Voice( GLToy_Hash uSource ) 
: GLToy_Sound_Voice( uSource )
, m_pxSourceVoice( 0 )
{
}

Platform_GLToy_Sound_Voice::~Platform_GLToy_Sound_Voice()
{
	if( m_pxSourceVoice )
	{
		m_pxSourceVoice->DestroyVoice();
	}
}

void Platform_GLToy_Sound_Voice::Play()
{
	// Find the source file

	GLToy_WaveFile* pxWav = Platform_GLToy_Sound_System::GetWave( m_uWave );
	if( !pxWav )
	{
		return;
	}
	const WAVEFORMATEX& xWaveFormat = pxWav->GetWaveFormat();

	// Check for duff wave formats. XAudio2 doesn't gracefully handle this, it just crashes
	if( xWaveFormat.nChannels == 0 || xWaveFormat.nSamplesPerSec == 0 )
	{
		return;
	}

	IXAudio2* pxEngine = Platform_GLToy_Sound_System::GetXAudio2Engine();
	if( !pxEngine )
	{
		return;
	}

	pxEngine->CreateSourceVoice( &m_pxSourceVoice, &xWaveFormat );
	
	if( !m_pxSourceVoice )
	{
		return;
	}

	XAUDIO2_BUFFER xBuffer;
	memset( &xBuffer, 0, sizeof( XAUDIO2_BUFFER ) );
	
	void* pBuffer;
	unsigned int uBufferSize;
	pxWav->GetBuffer( pBuffer, uBufferSize );

	xBuffer.AudioBytes = uBufferSize;
	xBuffer.pAudioData = (BYTE*)pBuffer;
	xBuffer.Flags = XAUDIO2_END_OF_STREAM;

	m_pxSourceVoice->SubmitSourceBuffer( &xBuffer );
	m_pxSourceVoice->Start();
}

bool Platform_GLToy_Sound_Voice::IsPlaying()
{
	if( m_pxSourceVoice )
	{
		XAUDIO2_VOICE_STATE xState;
		m_pxSourceVoice->GetState( &xState );

		if( xState.BuffersQueued > 0 )
		{
			return true;
		}
	}
	return false;
}

void Platform_GLToy_Sound_Voice::Update()
{
}

// eof
