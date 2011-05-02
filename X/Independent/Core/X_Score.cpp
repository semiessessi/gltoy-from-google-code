/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2011 Semi Essessi, Thomas Young
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

#include "Core/GLToy.h"

#include "X_Score.h"

#include "Maths/GLToy_Maths.h"
#include "Core/GLToy_Timer.h"
#include "String/GLToy_String.h"
#include "Render/Font/GLToy_Font.h"
#include "Render/GLToy_Render.h"

static float fX_SCORE_CHAIN_RESET_TIME = 1.0f;
static float fX_MAX_MULTIPLIER = 50.0f;


float X_Score::s_fMultiplier = 0.0f;
float X_Score::s_fScore = 0.0f;
float X_Score::s_fChainTimer = 0.0f;
X_Score_Graphic X_Score::s_xNotifications[ X_SCORE_MAX_NOTIFICATIONS ];
u_int X_Score::s_uCurrentNotification = 0;

// _____________________________________________________

X_Score_Graphic::X_Score_Graphic()
: m_xPosition( GLToy_Maths::ZeroVector2 )
, m_fScore( 0.0f )
, m_fLifeTime( 0.0f )
, m_uHash( uGLTOY_BAD_HASH )
, m_bActive( false )
{
}
		
void X_Score_Graphic::Update()
{
	if( !m_bActive )
	{
		return;
	}

	m_xPosition = GLToy_Maths::Lerp( m_xPosition, m_xDestPosition, GLToy_Timer::GetFrameTime() * 4.0f );

	m_fLifeTime -= GLToy_Timer::GetFrameTime();
	if( m_fLifeTime < 0.0f )
	{
		m_bActive = false;
	}
}

void X_Score_Graphic::Render()
{
	if( !m_bActive )
	{
		return;
	}

	if( m_fScore < 1.0f )
	{
		return;
	}

	static GLToy_String szString;

    szString.SetToFormatString( "%.0f", m_fScore );

	float fOpacity = GLToy_Maths::Min( m_fLifeTime * 2.0f, 1.0f ) * 0.8f;
	
	GLToy_Vector_4 xColour;
	xColour[0] = 0.5f + GLToy_Maths::Min( m_fScore / 100.0f, 1.0f ) * 0.5f;
	xColour[1] = 0.5f - GLToy_Maths::Min( m_fScore / 50.0f, 1.0f ) * 0.5f;
	xColour[2] = 0.5f - GLToy_Maths::Min( m_fScore / 50.0f, 1.0f ) * 0.5f;
	xColour[3] = fOpacity;  // TODO: Make the text rendering support this

    GLToy_Font_System::RenderString( szString, "FrontEnd", m_xPosition.x, m_xPosition.y, xColour );
}

void X_Score_Graphic::Activate( float fScore, const GLToy_Vector_2& xPosition, GLToy_Hash uHash )
{
	m_bActive = true;
	m_xPosition = xPosition;
	m_xDestPosition = xPosition;
	m_uHash = uHash;
		
	m_fLifeTime = 1.0f;

	m_fScore = fScore;
}

void X_Score_Graphic::Renew( float fAddScore, GLToy_Vector_2 xPosition )
{
	m_xDestPosition = xPosition;
	m_fScore += fAddScore;
	m_fLifeTime = 1.0f;
}

// _____________________________________________________

void X_Score::Initialise()
{
	s_fMultiplier = 1.0f;
	s_fScore = 0.0f;
	s_fChainTimer = 0.0f;
}

void X_Score::Shutdown()
{
}

void X_Score::Update()
{
	if( s_fChainTimer > 0.0f )
	{
		s_fChainTimer = GLToy_Maths::Max( 0.0f, s_fChainTimer - GLToy_Timer::GetFrameTime() );
		if( s_fChainTimer == 0.0f )
		{
			s_fMultiplier = 1.0f;
		}
	}

	for( u_int uNotification = 0; uNotification < X_SCORE_MAX_NOTIFICATIONS; ++uNotification )
	{
		s_xNotifications[ uNotification ].Update();
	}
}

void X_Score::Render()
{
	for( u_int uNotification = 0; uNotification < X_SCORE_MAX_NOTIFICATIONS; ++uNotification )
	{
		s_xNotifications[ uNotification ].Render();
	}
}

void X_Score::Add( float fScore, bool bIgnoreMultiplier )
{
	if( bIgnoreMultiplier )
	{
		s_fScore += fScore;
	}
	else
	{
		s_fScore += fScore * GLToy_Maths::Floor( s_fMultiplier );
		s_fMultiplier = GLToy_Maths::Min( fX_MAX_MULTIPLIER, s_fMultiplier + fScore * 0.01f );
		s_fChainTimer = fX_SCORE_CHAIN_RESET_TIME;
	}
}

void X_Score::Add( float fScore, const GLToy_Vector_2& xPosition, GLToy_Hash uHash, bool bIgnoreMultiplier )
{
	float fMultipliedScore = 0.0f;

	if( bIgnoreMultiplier )
	{
		s_fScore += fScore;
		fMultipliedScore = fScore;
	}
	else
	{
		fMultipliedScore = fScore * GLToy_Maths::Floor( s_fMultiplier );
		s_fScore += fMultipliedScore;

		s_fMultiplier = GLToy_Maths::Min( fX_MAX_MULTIPLIER, s_fMultiplier + fScore * 0.1f );
		s_fChainTimer = fX_SCORE_CHAIN_RESET_TIME;
	}

	AddNotification( fMultipliedScore, xPosition );
}

void X_Score::AddNotification( float fScore, GLToy_Vector_2 xPosition, GLToy_Hash uHash )
{
	const float fMinDistToReUse = 0.2f;
    const float fMinDistToReUseSquared = fMinDistToReUse * fMinDistToReUse;
	bool bReuse = false;
	u_int uNotificationIndex = 0;

	if( uHash != uGLTOY_BAD_HASH )
	{
		// Is there already a notification with this hash?
		for( u_int uNotification = 0; uNotification < X_SCORE_MAX_NOTIFICATIONS; ++uNotification )
		{
			if( s_xNotifications[ uNotification ].IsActive() && s_xNotifications[ uNotification ].GetHash() == uHash )
			{
				bReuse = true;
				uNotificationIndex = uNotification;
				break;
			}
		}
	}

	if( bReuse )
	{
		s_xNotifications[ uNotificationIndex ].Renew( fScore, xPosition );
	}
	else
	{
		s_xNotifications[ s_uCurrentNotification ].Activate( fScore, xPosition, uHash );
        // SE - this will break if X_SCORE_MAX_NOTIFICATIONS is not a power of 2
		// TY - Then why did you change it? The original code had no such restriction!
		s_uCurrentNotification = ( s_uCurrentNotification + 1 ) & ( X_SCORE_MAX_NOTIFICATIONS - 1 );//% X_SCORE_MAX_NOTIFICATIONS;
	}
}

// eof

