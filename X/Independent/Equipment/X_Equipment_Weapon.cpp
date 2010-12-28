
// Thomas Young 22.12.10

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
//#include <Render/GLToy_Texture.h>

// X
//#include <Entity/Enemy/X_Entity_Enemy.h>
//#include <Entity/Player/X_Entity_Player.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const int iX_EQUIP_WEAPON_MAX_PROJECTILES = 5;

static const float fX_EQUIP_WEAPON_MIN_SPEED = 0.01f;
static const float fX_EQUIP_WEAPON_MAX_SPEED = 0.025f;

static const float fX_EQUIP_WEAPON_MIN_RATE = 0.03f;
static const float fX_EQUIP_WEAPON_MAX_RATE = 0.5f;

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

void X_Equipment_Weapon::RandomGenerate( float fDamageMax )
{
	m_fBoost = 0.0f;

	// Base damage

	m_xDamage[1] = GLToy_Maths::Random( 1.0f, fDamageMax );
	m_xDamage[0] = GLToy_Maths::Random( m_xDamage[1] * 0.5f, m_xDamage[1] );

	const float fRelativeDamage = m_xDamage[0] / fDamageMax;

	// Use multiple projectiles?
	
	const bool bMultipleProjectiles = GLToy_Maths::Random() < 0.4f; // Tend to not use multiple

	m_iProjectiles = 1;
	if( bMultipleProjectiles )
	{
		m_iProjectiles += static_cast<int>( GLToy_Maths::Floor( GLToy_Maths::Random() * static_cast<float>( iX_EQUIP_WEAPON_MAX_PROJECTILES ) ) );
		m_xDamage *= 1.5f; // Damage boost for multiple projectiles
	}

	// Spread

	m_fSpread = GLToy_Maths::Random( 0.1f * static_cast<float>( m_iProjectiles - 1 ), 0.785f );

	// Speed

	float fSeed = ( 1.0f - fRelativeDamage ) + GLToy_Maths::Random( -0.25f, 0.25f ); // More damaging bullets tend to go slower

	m_xSpeed[1] = GLToy_Maths::ClampedLerp( fX_EQUIP_WEAPON_MIN_SPEED, fX_EQUIP_WEAPON_MAX_SPEED, fSeed );
	m_xSpeed[0] = GLToy_Maths::Random( m_xSpeed[1] * 0.5f, m_xSpeed[1] );

	// Fire rate

	fSeed = fRelativeDamage + GLToy_Maths::Random( -0.25f, 0.25f ); // More damaging bullets are fired less often
	if( bMultipleProjectiles )
	{
		fSeed += 0.1f;
	}

	m_xRate[1] = GLToy_Maths::ClampedLerp( fX_EQUIP_WEAPON_MIN_RATE, fX_EQUIP_WEAPON_MAX_RATE, fSeed );
	m_xRate[0] = GLToy_Maths::Random( m_xRate[1] * 0.5f, m_xRate[1] );

	// Size

	fSeed = fRelativeDamage + GLToy_Maths::Random( -0.25f, 0.25f ); // More damaging bullets are bigger
	m_fSize = GLToy_Maths::ClampedLerp( fX_EQUIP_WEAPON_MIN_SIZE, fX_EQUIP_WEAPON_MAX_SIZE, fSeed );

	// Wavey?

	m_bWavey = bMultipleProjectiles ? false : ( GLToy_Maths::Random() > 0.5f );
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
	return m_fSpread;
}

int X_Equipment_Weapon::GetNumProjectiles()
{
	return m_iProjectiles;
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
