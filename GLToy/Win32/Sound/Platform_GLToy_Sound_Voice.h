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
		bool IsPlaying() const;

	protected:

		Platform_GLToy_Sound_Voice( GLToy_Hash uSource );
		~Platform_GLToy_Sound_Voice();

		void Update();

	private:

		IXAudio2SourceVoice* m_pxSourceVoice;
};

#endif

//eof
