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
//#include <Core/GLToy_Timer.h>
//#include <Render/GLToy_Render.h>
//#include <Render/GLToy_Texture_System.h>

// X
//#include <Entity/Enemy/X_Entity_Enemy.h>
//#include <Entity/Player/X_Entity_Player.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const int iX_EQUIP_WEAPON_MAX_PROJECTILES = 5;

static const float fX_EQUIP_WEAPON_MIN_SPEED = 0.01f;
static const float fX_EQUIP_WEAPON_MAX_SPEED = 0.025f;

static const float fX_EQUIP_WEAPON_MIN_RATE = 0.05f;
static const float fX_EQUIP_WEAPON_MAX_RATE = 0.2f;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

X_Equipment_Weapon::X_Equipment_Weapon()
: m_fSize( 0.0f )
, m_fSpread( 0.0f )
, m_iProjectiles( 1 )
, m_bWavey( false )
, m_fBoost( 0.0f )
{
}

X_Equipment_Weapon::~X_Equipment_Weapon()
{
}

void X_Equipment_Weapon::RandomGenerate()
{
    // decide the weapon type - lets have just 4 for now and use magic numbers...
    const u_int uWeaponType = GLToy_Maths::RandomUint( 0, 3 );

	// Base damage
	m_xDamage[1] = ( uWeaponType & 1 ) ? 16.0f : 50.0f;
	m_xDamage[0] = ( uWeaponType > 1 ) ? 1.0f : 8.0f;

	// Use multiple projectiles?
	const bool bMultipleProjectiles = uWeaponType == 3; // Tend to not use multiple
	m_iProjectiles = 1;
	if( bMultipleProjectiles )
	{
		m_iProjectiles = 2;
		m_xDamage *= 1.5f; // Damage boost for multiple projectiles
	}

	// Spread
	m_fSpread = static_cast< float >( uWeaponType ) * 0.333333f;

	// Speed
	m_xSpeed[1] = GLToy_Maths::ClampedLerp( fX_EQUIP_WEAPON_MIN_SPEED, fX_EQUIP_WEAPON_MAX_SPEED, static_cast< float >( uWeaponType ) * 0.333333f );
	m_xSpeed[0] = m_xSpeed[1] * 0.125f;

	// Fire rate
	m_xRate[1] = ( uWeaponType < 2 ) ? fX_EQUIP_WEAPON_MIN_RATE : fX_EQUIP_WEAPON_MAX_RATE;
	m_xRate[0] = ( uWeaponType > 2 ) ? m_xRate[1] * 3.0f : m_xRate[1] * 2.0f;

	// Size
	m_fSize = ( uWeaponType & 1 ) ? fX_EQUIP_WEAPON_MIN_SIZE : fX_EQUIP_WEAPON_MAX_SIZE;

	// Wavey?
	m_bWavey = uWeaponType == 0;
}

float X_Equipment_Weapon::GetDamage()
{
	return GLToy_Maths::Lerp( m_xDamage[0], m_xDamage[1], m_fBoost );
}

float X_Equipment_Weapon::GetSpeed()
{
	return GLToy_Maths::Lerp( m_xSpeed[0], m_xSpeed[1], m_fBoost );
}

float X_Equipment_Weapon::GetRate()
{
	return GLToy_Maths::Lerp( m_xRate[0], m_xRate[1], m_fBoost );
}

float X_Equipment_Weapon::GetSize()
{
	return m_fSize;
}

float X_Equipment_Weapon::GetSpread()
{
	return 0.15f * m_fSpread;
}

int X_Equipment_Weapon::GetNumProjectiles()
{
	return m_iProjectiles + ( ( m_iProjectiles > 1 ) ? static_cast< u_int >( 7.0f * m_fBoost ) : 0 );
}

bool X_Equipment_Weapon::IsWavey()
{
	return m_bWavey;
}

void X_Equipment_Weapon::Boost( float fBoost )
{
	m_fBoost = GLToy_Maths::Min( 1.0f, m_fBoost + fBoost );
}

//eof
