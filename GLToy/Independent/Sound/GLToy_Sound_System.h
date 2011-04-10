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

#ifndef __GLTOY_SOUND_SYSTEM_H_
#define __GLTOY_SOUND_SYSTEM_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Core/GLToy_Hash.h>
#include "Maths/GLToy_Maths.h"
#include <String/GLToy_String.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

//template < class T > class GLToy_Array;
//template < class T > class GLToy_HashMap;
class GLToy_Vector_3;
//class GLToy_Sound_Source;
//class GLToy_SoundFile;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

// ______________________________ GLToy_Sound_Voice _________________________________________

class GLToy_Sound_Voice
{  
	public:

		enum SPEAKER_MAPPING
		{
			SM_DIRECT,
			SM_SPATIAL
		};

		GLToy_Sound_Voice( GLToy_Hash uWave );
		virtual ~GLToy_Sound_Voice() {};

		virtual void Play() = 0;
		virtual bool IsPlaying() const = 0;

		void Release()    { m_bReleased = true; }
		bool IsReleased() const { return m_bReleased; }

		void SetSpeakerMapping( SPEAKER_MAPPING eMapping ) { m_eSpeakerMapping = eMapping; }
		void SetPosition( const GLToy_Vector_3& xPosition ) { m_xPosition = xPosition; m_bIsPositional = true; }
		void SetRadius( const float fRadius ) { m_fRadius = fRadius; }
		void SetAmplitude( const float fAmplitude ) { m_fAmplitude = fAmplitude; }
		void SetPitch( const float fPitch ) { m_fPitch = fPitch; }

		bool IsPositional() const { return m_bIsPositional; }

	protected:

		float GetSpeakerAmplitude( u_int uSpeaker );

		GLToy_Vector_3 m_xPosition;

		GLToy_Hash m_uWave;
		SPEAKER_MAPPING m_eSpeakerMapping;

		float m_fRadius;
		float m_fAmplitude;
		float m_fPitch;

		bool m_bIsPositional;
		bool m_bReleased;
		
};

// ______________________________ GLToy_Sound_System _________________________________________

class GLToy_Sound_System
{

    //friend class GLToy_Sound;
    //friend class GLToy_WaveFile;

	public:

		static bool Initialise();
		static void Update();
		static void Shutdown();

		static GLToy_Handle CreateVoice( const GLToy_Hash uWaveHash );
		static GLToy_Sound_Voice* GetVoice( const GLToy_Handle iHandle );
		static void DestroyVoice( const GLToy_Handle iHandle );
};

#endif
