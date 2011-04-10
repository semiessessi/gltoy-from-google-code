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

#include <Core/GLToy.h>

// This file's header
#include <Equipment/X_Weapon_Types.h>

// GLToy
#include <Maths/GLToy_Maths.h>
#include <Core/GLToy_Timer.h>
#include "Entity/GLToy_Entity_System.h"
//#include <Render/GLToy_Render.h>
//#include <Render/GLToy_Texture_System.h>

// X
#include "Entity/Enemy/X_Entity_Enemy.h"
#include "Entity/Player/X_Entity_Player.h"
#include <Entity/Projectile/X_Entity_Projectile.h>
#include <Entity/X_EntityTypes.h>
#include "Sound/X_Sound_System.h"

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

// ______________________________  Player Weapon Types  ____________________________________
// _________________________________________________________________________________________

X_Player_Weapon_Vulcan::X_Player_Weapon_Vulcan()
: X_Player_Weapon()
, m_fShootTimer( 0.0f )
{
}

X_Player_Weapon_Vulcan::~X_Player_Weapon_Vulcan()
{

}

void X_Player_Weapon_Vulcan::StartShooting()
{
	X_Player_Weapon::StartShooting();

	m_fShootTimer = 100.0f;
}

void X_Player_Weapon_Vulcan::Update()
{
	X_Player_Weapon::Update();
	
	X_Entity_Player* pxPlayer = GetPlayerEntity();

	if( pxPlayer && IsShooting() )
	{
		const float fShootResetMod = GetBoost() > 0 ? 0.05f : 0.1f;
		const float fShootReset = GLToy_Maths::Max( 3.0f - GetTimeShooting(), 1.0f ) * fShootResetMod;

		m_fShootTimer += GLToy_Timer::GetFrameTime();
		if( m_fShootTimer > fShootReset )
		{
			for(int i=0; i<2; ++i)
			{ 
				X_Entity_Projectile* pxProjectile = static_cast< X_Entity_Projectile* >( GLToy_Entity_System::CreateEntity( GLToy_Random_Hash(), X_ENTITY_PROJECTILE ) );
				
				GLToy_Vector_3 xPos = pxPlayer->GetPosition();
				xPos.x += GLToy_Maths::Cos( GetTimeShooting() * 6.0f ) * 0.05f * ( i ? 1.0f : -1.0f );
				pxProjectile->SetPosition( xPos );
			
				const float fSpread = GLToy_Maths::Max( 1.0f - GetTimeShooting() * 3.0f, 0.0f );
			
				GLToy_Vector_2 xDirection( 0.0f, 1.0f );
			
				if( i )
				{
					xDirection.x = -xDirection.x;
				}

				xDirection.Normalise();

				pxProjectile->SetDirection( GLToy_Vector_3( xDirection.x, xDirection.y, 0.0f ) );
				pxProjectile->SetRadius( 0.015f );
				pxProjectile->SetTexture( 1 );
				pxProjectile->SetIsFromPlayer( true );
				pxProjectile->SetSpeed( 3.0f );
			}

			X_Sound_System::PlayOneShotSound( GLToy_Hash_Constant( "PlayerShoot1" ), pxPlayer->GetPosition() );

			m_fShootTimer = 0.0f;
		}
	}
}

// ______________________________  Enemy Weapon Types  ____________________________________
// _________________________________________________________________________________________

X_Enemy_Weapon_Single::X_Enemy_Weapon_Single( GLToy_Hash uEnemyHash )
: X_Enemy_Weapon( uEnemyHash )
, m_fShootTimer( 0.0f )
{
}
X_Enemy_Weapon_Single::~X_Enemy_Weapon_Single()
{
}

void X_Enemy_Weapon_Single::Update()
{
	X_Enemy_Weapon::Update();
	
	m_fShootTimer -= GLToy_Timer::GetFrameTime();

	const float fShootInterval = 2.0f; 

	if( m_fShootTimer < 0.0f )
	{
		X_Entity_Player* pxPlayer = GetPlayerEntity();
		X_Entity_Enemy* pxEnemy = GetEntity();

		if( pxPlayer && pxEnemy && IsShooting() )
		{
			X_Entity_Projectile* pxProjectile = static_cast< X_Entity_Projectile* >( GLToy_Entity_System::CreateEntity( GLToy_Random_Hash(), X_ENTITY_PROJECTILE ) );
			GLToy_Vector_3 xDirection = ( pxPlayer->GetPosition() - pxEnemy->GetPosition() );
			xDirection.x += GLToy_Maths::Random( - 0.1f, 0.1f );
			xDirection.Normalise();
			pxProjectile->SetDirection( xDirection );
			pxProjectile->SetPosition( pxEnemy->GetPosition() );
			pxProjectile->SetRadius( 0.008f );
			pxProjectile->SetTexture( 0 );
			pxProjectile->SetIsFromPlayer( false );
			pxProjectile->SetSpeed( 1.2f );

			X_Sound_System::PlayOneShotSound( GLToy_Hash_Constant( "EnemyShoot1" ), pxEnemy->GetPosition() );

			m_fShootTimer = fShootInterval;
		}
	}
}

//eof
