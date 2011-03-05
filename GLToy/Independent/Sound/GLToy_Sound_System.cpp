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

// This file's headers
#include <Sound/GLToy_Sound_System.h>
#include <Sound/Platform_GLToy_Sound_System.h>

#include "Maths/GLToy_Maths.h"
#include "Render/GLToy_Camera.h"
#include "Core/Console/GLToy_Console.h"

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

static void SpawnSound()
{
	GLToy_Handle xVoice = GLToy_Sound_System::CreateVoice( GLToy_Hash_Constant( "Explode" ) );
	GLToy_Sound_Voice* pxVoice = GLToy_Sound_System::GetVoice( xVoice );
	if( pxVoice )
	{
		pxVoice->SetSpeakerMapping( GLToy_Sound_Voice::SM_SPATIAL );
		pxVoice->SetPosition( GLToy_Camera::GetPosition() );
		pxVoice->SetRadius( 200.0f );
		pxVoice->Play();
		pxVoice->Release();
	}
}

bool GLToy_Sound_System::Initialise()
{
	GLToy_Console::RegisterCommand( "spawnsound", SpawnSound );

	return Platform_GLToy_Sound_System::Initialise();
}

void GLToy_Sound_System::Update()
{
	Platform_GLToy_Sound_System::Update();
}

void GLToy_Sound_System::Shutdown()
{
	Platform_GLToy_Sound_System::Shutdown();
}

GLToy_Handle GLToy_Sound_System::CreateVoice( const GLToy_Hash uWaveHash )
{
	return Platform_GLToy_Sound_System::CreateVoice( uWaveHash );
}

GLToy_Sound_Voice* GLToy_Sound_System::GetVoice( const GLToy_Handle iHandle )
{
	return Platform_GLToy_Sound_System::GetVoice( iHandle );
}

void GLToy_Sound_System::DestroyVoice( const GLToy_Handle iHandle )
{
	return Platform_GLToy_Sound_System::DestroyVoice( iHandle );
}

// __________________________________________________________________________________________________

GLToy_Sound_Voice::GLToy_Sound_Voice( GLToy_Hash uWave )
: m_uWave( uWave )
, m_xPosition( GLToy_Maths::ZeroVector2 )
, m_fRadius( 1.0f )
, m_eSpeakerMapping( SM_DIRECT )
, m_bReleased( false )
, m_bIsPositional( false )
, m_fAmplitude( 1.0f )
{
}

float ANGLY = 0.0f;

float GLToy_Sound_Voice::GetSpeakerAmplitude( u_int uSpeaker )
{
	GLToy_Vector_3 xToSound = GLToy_Camera::GetPosition() - m_xPosition;

	const float fDistance = xToSound.Magnitude();
	if( IsPositional() && fDistance > m_fRadius )
	{
		return 0.0f;
	}

	float fDistanceAmplitude = 1.0f - ( fDistance + 0.01f ) / m_fRadius;

	if( m_eSpeakerMapping == SM_DIRECT )
	{
		if( IsPositional() )
		{
			return m_fAmplitude * fDistanceAmplitude; 
		}
		else
		{
			return m_fAmplitude;
		}
	}
	else if( m_eSpeakerMapping == SM_SPATIAL )
	{
		if( xToSound == GLToy_Maths::ZeroVector3 )
		{
			xToSound.z += 1.0f;
		}
		xToSound.Normalise();
		GLToy_Vector_2 xCameraDir( GLToy_Camera::GetDirection().x, GLToy_Camera::GetDirection().z );
		xCameraDir.Normalise();
		
		switch( 1 << uSpeaker )
		{
			case SPEAKER_FRONT_LEFT:  xCameraDir = GLToy_Maths::Rotate_2D( xCameraDir, + 1.0f );  break;
			case SPEAKER_FRONT_RIGHT: xCameraDir = GLToy_Maths::Rotate_2D( xCameraDir, - 1.0f );  break;
		}
		
		float fAngle = GLToy_Maths::ACos( xToSound.x * xCameraDir.x + xToSound.z * xCameraDir.y ) / GLToy_Maths::Pi;

		fAngle = GLToy_Maths::Pow( fAngle, 2.5f );

		return fAngle * m_fAmplitude * fDistanceAmplitude;
	}

	return 0.0f;
}

//eof
