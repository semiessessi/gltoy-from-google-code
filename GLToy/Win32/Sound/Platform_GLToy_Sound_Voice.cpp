/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Thomas Young
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
	#ifndef GLTOY_DEMO // TY - huh? soundless demo?
	
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

	m_pxSourceVoice->SetFrequencyRatio( m_fPitch );
	m_pxSourceVoice->SubmitSourceBuffer( &xBuffer );
	m_pxSourceVoice->Start();
	#endif
}

bool Platform_GLToy_Sound_Voice::IsPlaying() const
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
	if( !IsPlaying() )
	{
		return;
	}

	XAUDIO2_VOICE_DETAILS xVoiceDetails;
	m_pxSourceVoice->GetVoiceDetails( &xVoiceDetails );
	const u_int uNumInChans = xVoiceDetails.InputChannels;

	float afMatrix[32];
	memset( afMatrix, 0, sizeof( float ) * 32 );

	for( u_int uInChan = 0; uInChan < 2; ++uInChan )
	{
		afMatrix[ uInChan ] = GetSpeakerAmplitude( uInChan );
	}

  	m_pxSourceVoice->SetOutputMatrix( 0, uNumInChans, 2, afMatrix );
	m_pxSourceVoice->SetFrequencyRatio( m_fPitch );
}

// eof
