
// TY - 10.04.11

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

#ifndef __X_SOUND_SYSTEM_H_
#define __X_SOUND_SYSTEM_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include "Sound/GLToy_Sound_System.h"

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const float fX_SOUND_DEFAULT_SPATIAL_AMPLITUDE = 0.75f;
static const float fX_SOUND_DEFAULT_DIRECT_AMPLITUDE = 0.95f;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class X_Sound_System
{
	public:

		static void PlayOneShotSound( GLToy_Hash uHash, float fAmplitude = fX_SOUND_DEFAULT_DIRECT_AMPLITUDE )
		{
			GLToy_Handle xVoice = GLToy_Sound_System::CreateVoice( uHash );
			GLToy_Sound_Voice* pxVoice = GLToy_Sound_System::GetVoice( xVoice );
			if( pxVoice )
			{
				pxVoice->SetSpeakerMapping( GLToy_Sound_Voice::SM_DIRECT );
				pxVoice->SetAmplitude( fAmplitude );
				pxVoice->Play();
				pxVoice->Release();
			}
		}

		static void PlayOneShotSound( GLToy_Hash uHash, const GLToy_Vector_3& xPosition, float fAmplitude = fX_SOUND_DEFAULT_SPATIAL_AMPLITUDE )
		{
			GLToy_Handle xVoice = GLToy_Sound_System::CreateVoice( uHash );
			GLToy_Sound_Voice* pxVoice = GLToy_Sound_System::GetVoice( xVoice );
			if( pxVoice )
			{
				pxVoice->SetSpeakerMapping( GLToy_Sound_Voice::SM_SPATIAL );
				pxVoice->SetPosition( xPosition );
				pxVoice->SetRadius( 600.0f );
				pxVoice->SetPitch( GLToy_Maths::Random( 0.8f, 1.2f ) );
				pxVoice->SetAmplitude( fAmplitude );
				pxVoice->Play();
				pxVoice->Release();
			}
		}
};

#endif

// eof
