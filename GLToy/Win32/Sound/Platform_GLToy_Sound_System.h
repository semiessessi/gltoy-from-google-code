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

#ifndef __PLATFORM_GLTOY_SOUND_SYSTEM_H_
#define __PLATFORM_GLTOY_SOUND_SYSTEM_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/Data Structures/GLToy_Array.h>
#include <Core/Data Structures/GLToy_HashMap.h>

#include <Sound/Platform_GLToy_Sound_Voice.h>

#include <XAudio2.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// L I B R A R I E S
/////////////////////////////////////////////////////////////////////////////////////////////

//#pragma comment( lib, "xaudio2" )
//#pragma comment( lib, "alut" )

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S 
/////////////////////////////////////////////////////////////////////////////////////////////

/*struct IUnknown;
struct IXAPO;
struct IXAudio2;
struct IXAudio2MasteringVoice;
struct IXAudio2SourceVoice;
struct IXAudio2SubmixVoice;
struct IXAudio2Voice;
struct IXAudio2VoiceCallback;
struct tWAVEFORMATEX;
typedef tWAVEFORMATEX WAVEFORMATEX;
struct XAUDIO2FX_REVERB_PARAMETERS;*/

class GLToy_Sound_Voice;
class Platform_GLToy_Sound_Voice;
class GLToy_WaveFile;

struct IXAudio2MasteringVoice;
struct IXAudio2;

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const unsigned int uGLTOY_XAUDIO_MAX_VOICES = 64;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class Platform_GLToy_Sound_System
{
	friend class Platform_GLToy_Sound_Voice;

	public:

		static bool Initialise();
		static void Shutdown();
		static void Update();

		static GLToy_Handle CreateVoice( const GLToy_Hash uWaveHash );
		static GLToy_Sound_Voice* GetVoice( const GLToy_Handle iHandle );
		static void DestroyVoice( const GLToy_Handle iHandle );

	protected:

		static GLToy_WaveFile* GetWave( GLToy_Hash uHash );
		static IXAudio2* GetXAudio2Engine();

	private:

		static bool Initialise_XAudio2();
		static bool Initialise_Voices();
		static bool Initialise_LoadSounds();

		static IXAudio2* s_pxEngine;
		static IXAudio2MasteringVoice* s_pxMasteringVoice;

		static Platform_GLToy_Sound_Voice* s_apxVoices[ uGLTOY_XAUDIO_MAX_VOICES ];
		static GLToy_HashMap< GLToy_WaveFile* > s_xWaves;
};

#endif
