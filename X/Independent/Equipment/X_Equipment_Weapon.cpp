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
#include <Equipment/X_Equipment_Weapon.h>

// GLToy
#include <Maths/GLToy_Maths.h>
#include <Core/GLToy_Timer.h>
//#include <Render/GLToy_Render.h>
//#include <Render/GLToy_Texture_System.h>

// X
#include "Entity/Enemy/X_Entity_Enemy.h"
#include "Entity/Player/X_Entity_Player.h"
#include <Equipment/X_Weapon_Types.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

X_Weapon::X_Weapon()
: m_bIsShooting( false )
, m_fShootTimer( 0.0f )
{
}

X_Weapon::~X_Weapon()
{
}

void X_Weapon::Update()
{
	if( IsShooting() )
	{
		m_fShootTimer = m_fShootTimer + GLToy_Timer::GetFrameTime();
	}
}

void X_Weapon::StartShooting()
{
	m_bIsShooting = true;
	m_fShootTimer = 0.0f;
}

void X_Weapon::StopShooting()
{
	m_bIsShooting = false;
	m_fShootTimer = 0.0f;
}

X_Entity_Player* X_Weapon::GetPlayerEntity()
{
	GLToy_Array<X_Entity_Player*> xList = X_Entity_Player::GetList();
	return xList[0];
}


// _________________________________________________________________________________________
// _________________________________________________________________________________________

X_Player_Weapon::X_Player_Weapon() 
: X_Weapon()
, m_iBoost( 0 )
{
}

X_Player_Weapon::~X_Player_Weapon()
{
}

void X_Player_Weapon::Boost()
{
	m_iBoost = GLToy_Maths::Min( m_iBoost + 1, X_WEAPON_MAX_BOOST );
}

// _________________________________________________________________________________________
// _________________________________________________________________________________________

X_Enemy_Weapon::X_Enemy_Weapon( GLToy_Hash uEnemyHash )
: X_Weapon()
, m_uEnemyHash( uEnemyHash )
{

}

X_Enemy_Weapon::~X_Enemy_Weapon()
{
}

X_Entity_Enemy* X_Enemy_Weapon::GetEntity()
{
	GLToy_Array<X_Entity_Enemy*> xList = X_Entity_Enemy::GetList();
	
	for( u_int uEnemy = 0; uEnemy < xList.GetCount(); ++uEnemy )
	{
		if( xList[uEnemy]->GetHash() == m_uEnemyHash )
		{
			return xList[uEnemy];
		}
	}

	return 0;
}

// _________________________________________________________________________________________
// _________________________________________________________________________________________

X_Weapon* X_Weapon_Factory::CreateWeapon( u_int uType, GLToy_Hash uEnemyHash )
{
	switch( uType )
	{
		case ePLAYER_WEAPON_VULCAN: return new X_Player_Weapon_Vulcan;

		case eENEMY_WEAPON_SINGLE: return new X_Enemy_Weapon_Single( uEnemyHash );
	}

	return 0;
}

X_Player_Weapon* X_Weapon_Factory::CreatePlayerWeapon( u_int uType )
{
	return reinterpret_cast< X_Player_Weapon* >( CreateWeapon( uType ) );
}

X_Enemy_Weapon* X_Weapon_Factory::CreateEnemyWeapon( u_int uType, GLToy_Hash uEnemyHash )
{
	return reinterpret_cast< X_Enemy_Weapon* >( CreateWeapon( uType, uEnemyHash ) );
}

//eof
