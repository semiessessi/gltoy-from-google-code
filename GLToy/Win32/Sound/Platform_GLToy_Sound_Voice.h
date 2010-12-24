
// Thomas Young 13.12.10

#ifndef _PLATFORM_SOUND_VOICE_H_
#define _PLATFORM_SOUND_VOICE_H_

#include <Sound/GLToy_Sound_System.h>

#include <XAudio2.h>

class Platform_GLToy_Sound_Voice : public GLToy_Sound_Voice
{
	friend class Platform_GLToy_Sound_System;

	public:

		void Play();
		bool IsPlaying();

	protected:

		Platform_GLToy_Sound_Voice( GLToy_Hash uSource );
		~Platform_GLToy_Sound_Voice();

		void Update();

	private:

		IXAudio2SourceVoice* m_pxSourceVoice;
};

#endif

//eof
