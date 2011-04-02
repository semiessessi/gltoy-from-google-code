/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010, 2011 Semi Essessi, Thomas Young
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

#include <Core/X.h>

// This file's header
#include <Entity/Enemy/X_Entity_Enemy.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Material/GLToy_Material_System.h>
#include <Particle/GLToy_PFX_System.h>
#include <Sound/GLToy_Sound_System.h>
#include <Render/GLToy_Light_System.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Render_Metrics.h>
#include <Render/GLToy_Texture_System.h>

// X
#include "AI/X_Enemy_Brain.h"
#include "Entity/Player/X_Entity_Player.h"
#include "Equipment/X_Equipment_Weapon.h"
#include "Equipment/X_Weapon_Types.h"
#include <Core/State/X_State_Game.h>
#include "Core/X_Score.h"
#include "Core/X_Effects.h"

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

const float fX_ENTITY_ENEMY_EXPLODE_RANGE = 0.5f;

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Array< X_Entity_Enemy* > X_Entity_Enemy::s_xList;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

X_Entity_Enemy::X_Entity_Enemy( const GLToy_Hash uHash, const u_int uType )
: GLToy_Parent( uHash, uType )
, m_uLight( GLToy_Random_Hash() )
, m_pxBrain( 0 )
, m_pxWeapon( 0 )
, m_xImpulse( GLToy_Maths::ZeroVector2 )
{
	m_xDirection = GLToy_Maths::ZeroVector2;
	m_xDirection.y = -1.0f;

    s_xList.Append( this );

    //GLToy_Light_PointProperties xProperties;
    //xProperties.m_fSphereRadius = 0.35f;
    //xProperties.m_xColour = GLToy_Vector_3( 1.5f, 0.5f, 0.3f );
    //GLToy_Light_System::AddPointLight( m_uLight, xProperties );
}

X_Entity_Enemy::~X_Entity_Enemy()
{
    //GLToy_Light_System::DestroyLight( m_uLight );

    s_xList.RemoveByValue( this );

	delete m_pxBrain;
	delete m_pxWeapon;
}

void X_Entity_Enemy::SetDefinition( const X_Enemy_Definition& xDefinition )
{
	m_xDefinition = xDefinition;

	delete m_pxBrain;
	m_pxBrain = X_Enemy_Brain_Factory::CreateBrain( xDefinition.m_uBrain, GetHash() );

    BoundsFromMaterial( xDefinition.m_uMaterial, xDefinition.m_fSize );

	SetHealth( xDefinition.m_fHealth );

	if( xDefinition.m_uWeapon )
	{
		m_pxWeapon = X_Weapon_Factory::CreateWeapon( xDefinition.m_uWeapon, GetHash() );
	}
}


void X_Entity_Enemy::Update()
{
	if( m_pxBrain )
	{
		m_pxBrain->Update();
	}

	GLToy_Vector_3 xPosition = GetPosition();
	xPosition.x += m_xDirection.x * m_xDefinition.m_fSpeed * GLToy_Timer::GetFrameTime();
	xPosition.y += m_xDirection.y * m_xDefinition.m_fSpeed * GLToy_Timer::GetFrameTime();

	// TODO: Make FPS independent
	m_xImpulse.x *= 0.95f;
	m_xImpulse.y *= 0.95f;
	
	xPosition.x += m_xImpulse.x * m_xDefinition.m_fKnockBack * GLToy_Timer::GetFrameTime();
	xPosition.y += m_xImpulse.y * m_xDefinition.m_fKnockBack * GLToy_Timer::GetFrameTime();

	SetPosition( xPosition );

    if( ( xPosition.y < -1.5f ) ||
		( xPosition.y > 1.5f ) ||
		( xPosition.x < -1.5f ) ||
		( xPosition.x > 1.5f ) )
    {
        Destroy();
		GLToy_Parent::Update();
		return;
    }

	GLToy_Array< X_Entity_Player* > xPlayerList = X_Entity_Player::GetList();
	X_Entity_Player* pxPlayer = xPlayerList[0];

	if( m_pxWeapon )
	{
		if( ( xPosition.y > -0.95f ) &&
			 ( xPosition.y < 0.95f ) &&
			 ( xPosition.x > -0.95f ) &&
			 ( xPosition.x < 0.95f ) &&
			 pxPlayer && 
			 ( pxPlayer->GetPosition().y < ( GetPosition().y - 0.1f ) ) )
		{
			if( !m_pxWeapon->IsShooting() )
			{
				m_pxWeapon->StartShooting();
			}
		}
		else
		{
			if( m_pxWeapon->IsShooting() )
			{
				m_pxWeapon->StopShooting();
			}
		}
		m_pxWeapon->Update();
	}

    /*GLToy_Light* const pxLight = GLToy_Light_System::FindLight( m_uLight );
    if( pxLight )
    {
        pxLight->SetPosition( xPosition + GLToy_Vector_3( 0.0f, 0.5f * fSIZE, -0.1f ) );
    }*/

    if( IsDead() )
    {
		X_Effect_System::CreateShockwave( xPosition, 0.5f );

        GLToy_PFX_System::CreatePFX( GLToy_Hash_Constant( "Explosion1" ), xPosition, GetVelocity() );
        
		GLToy_Handle xVoice = GLToy_Sound_System::CreateVoice( GLToy_Hash_Constant( "Explode" ) );
		GLToy_Sound_Voice* pxVoice = GLToy_Sound_System::GetVoice( xVoice );
		if( pxVoice )
		{
			pxVoice->SetSpeakerMapping( GLToy_Sound_Voice::SM_SPATIAL );
			pxVoice->SetPosition( xPosition );
			pxVoice->SetRadius( 600.0f );
			pxVoice->SetAmplitude( 0.5f );
			pxVoice->Play();
			pxVoice->Release();
		}
		
		X_Score::Add( m_xDefinition.m_fScore, xPosition );
        Destroy();
    }

    GLToy_Parent::Update();
}

void X_Entity_Enemy::Render() const
{
    GLToy_Render_Metrics::IncrementTriangleCount( 2 );

    if( GLToy_Render::HasDeferredBuffer() )
    {
        GLToy_Render::RegisterDeferred( this );
        return;
    }

	const GLToy_Vector_3& xPosition = GetPosition();

    GLToy_Render::EnableBlending();
    GLToy_Render::SetBlendFunction( BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA );

    GLToy_Material_System::BindMaterial( m_xDefinition.m_uMaterial );

	const float fSize = m_xDefinition.m_fSize;

    GLToy_Render::StartSubmittingQuads();

    GLToy_Render::SubmitColour( GLToy_Vector_3( 1.0f, 1.0f, 1.0f ) );
	GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 0.0f ) );
	GLToy_Render::SubmitVertex( xPosition[ 0 ] - fSize, xPosition[ 1 ] + fSize, xPosition[ 2 ] ); 
    GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 0.0f ) );
	GLToy_Render::SubmitVertex( xPosition[ 0 ] + fSize, xPosition[ 1 ] + fSize, xPosition[ 2 ] ); 
	GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 1.0f ) );
    GLToy_Render::SubmitVertex( xPosition[ 0 ] + fSize, xPosition[ 1 ] - fSize, xPosition[ 2 ] );
	GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 1.0f ) );
    GLToy_Render::SubmitVertex( xPosition[ 0 ] - fSize, xPosition[ 1 ] - fSize, xPosition[ 2 ] );
	
    GLToy_Render::EndSubmit();

	GLToy_Render::DisableBlending();
}

void X_Entity_Enemy::RenderDeferred() const
{
    GLToy_Vector_3 xPosition = GetPosition();

    GLToy_Material* const pxMaterial = GLToy_Material_System::FindMaterial( m_xDefinition.m_uMaterial );
    if( pxMaterial )
    {
        pxMaterial->Bind();
    }

	const float fSize = m_xDefinition.m_fSize;

	GLToy_Vector_2 xVertex1( - fSize, + fSize );
	GLToy_Vector_2 xVertex2( + fSize, + fSize );
	GLToy_Vector_2 xVertex3( + fSize, - fSize );
	GLToy_Vector_2 xVertex4( - fSize, - fSize );

	// TY - Sorry for the mess, this was me experimenting with rotating the sprite

	xVertex1.x += xPosition.x;	xVertex1.y += xPosition.y;
	xVertex2.x += xPosition.x;	xVertex2.y += xPosition.y;
	xVertex3.x += xPosition.x;	xVertex3.y += xPosition.y;
	xVertex4.x += xPosition.x;	xVertex4.y += xPosition.y;

    GLToy_Render::StartSubmittingQuads();

	GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 0.0f ) );
	GLToy_Render::SubmitUV( xCOMPRESSED_NORMAL_BASIS, 1 );
	GLToy_Render::SubmitVertex( xVertex1.x, xVertex1.y, xPosition[ 2 ] ); 
    GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 0.0f ) );
	GLToy_Render::SubmitUV( xCOMPRESSED_NORMAL_BASIS, 1 );
	GLToy_Render::SubmitVertex( xVertex2.x, xVertex2.y, xPosition[ 2 ] ); 
	GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 1.0f ) );
	GLToy_Render::SubmitUV( xCOMPRESSED_NORMAL_BASIS, 1 );
    GLToy_Render::SubmitVertex( xVertex3.x, xVertex3.y, xPosition[ 2 ] );
	GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 1.0f ) );
	GLToy_Render::SubmitUV( xCOMPRESSED_NORMAL_BASIS, 1 );
    GLToy_Render::SubmitVertex( xVertex4.x, xVertex4.y, xPosition[ 2 ] );
	
    GLToy_Render::EndSubmit();
}

GLToy_Vector_3 X_Entity_Enemy::GetVelocity() const 
{ 
	return GLToy_Vector_3( m_xDirection * m_xDefinition.m_fSpeed, 0.0f ); 
}