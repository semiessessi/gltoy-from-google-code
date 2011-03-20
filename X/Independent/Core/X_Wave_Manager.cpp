
/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2011 Thomas Young
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

#include "Core/X.h"

// This file's header
#include "X_Wave_Manager.h"

// GLToy
#include <Core/GLToy_Timer.h>
#include <Core/GLToy_Hash.h>
//#include <Entity/GLToy_Entity_System.h>
//#include <Material/GLToy_Material_System.h>
//#include <Render/GLToy_Render.h>
//#include <Render/GLToy_Texture_System.h>

// X
#include "AI/X_Enemy_Brain_Types.h"
#include "Core/X_Spawner.h"
//#include <Core/X_Cheats.h>
//#include <Entity/X_EntityTypes.h>
//#include <Entity/Collectible/X_Entity_Collectible.h>
#include "X_Spawner_Types.h"
#include <Entity/Enemy/X_Entity_Enemy.h>
#include "Equipment/X_Weapon_Types.h"

static const float fX_WAVE_MANAGER_WAVE_DURATION = 30.0f;
static const float fX_WAVE_MANAGER_REST_DURATION = 5.0f;

static const float fX_WM_ENEMY_SPEED_EXTRA_SLOW = 0.2f;
static const float fX_WM_ENEMY_SPEED_SLOW = 0.5f;
static const float fX_WM_ENEMY_SPEED_NORMAL = 0.75f;
static const float fX_WM_ENEMY_SPEED_FAST = 1.0f;
static const float fX_WM_ENEMY_SPEED_EXTA_FAST = 1.3f;

static X_Enemy_Definition xEnemy_Def_Template_Diver;
static X_Enemy_Definition xEnemy_Def_Template_Hunter;

// Classes

class X_Timed_Spawner
{
	public:

		X_Timed_Spawner( SPAWNER_TYPES eSpawnerType, const X_Enemy_Definition& xDefinition, float fStartTime = 0.0f, float fEndTime = fX_WAVE_MANAGER_WAVE_DURATION );
		~X_Timed_Spawner();

		void Update();

	private:

		float m_fStartTime;
		float m_fEndTime;
		X_Enemy_Spawner* m_pxSpawner;
		float m_fTimer;
};

// Functions

// ________________ X_Timed_Spawner ________________


X_Timed_Spawner::X_Timed_Spawner( SPAWNER_TYPES eSpawnerType, const X_Enemy_Definition& xDefinition, float fStartTime, float fEndTime )
: m_fStartTime( fStartTime )
, m_fEndTime( fEndTime )
, m_pxSpawner( 0 )
, m_fTimer( 0.0f )
{
	m_pxSpawner = X_Enemy_Spawner_Factory::CreateSpawner( eSpawnerType, xDefinition );
}

X_Timed_Spawner::~X_Timed_Spawner()
{
	delete m_pxSpawner;
}

void X_Timed_Spawner::Update()
{
	if( !m_pxSpawner )
	{
		return;
	}

	m_fTimer += GLToy_Timer::GetFrameTime();

	if( ( m_fTimer > m_fStartTime ) && ( m_fTimer < m_fEndTime ) )
	{
		m_pxSpawner->SetActive( true );
	}
	else
	{
		m_pxSpawner->SetActive( false );
	}

	m_pxSpawner->Update();
}


// ________________ X_Wave_Manager ________________


X_Timed_Spawner* X_Wave_Manager::s_apxTimedSpawners[ X_WAVE_MANAGER_MAX_TIMED_SPAWNERS ];
u_int X_Wave_Manager::s_uWave = 0;
float X_Wave_Manager::s_fWaveTimer = 0.0f;
bool X_Wave_Manager::s_bRestPeriod = true;


void X_Wave_Manager::Inititalise()
{
	for( u_int u = 0; u < X_WAVE_MANAGER_MAX_TIMED_SPAWNERS; ++u )
	{
		s_apxTimedSpawners[ u ] = 0;
	}

	// Fill out the templates

	xEnemy_Def_Template_Diver.m_fHealth = 10.0f;
	xEnemy_Def_Template_Diver.m_fSize = 0.075f;
	xEnemy_Def_Template_Diver.m_uBrain = eENEMY_BRAIN_DIVE;
	xEnemy_Def_Template_Diver.m_uMaterial = GLToy_Hash_Constant( "Enemy/Enemy1" );
	xEnemy_Def_Template_Diver.m_fSpeed = fX_WM_ENEMY_SPEED_NORMAL;
	xEnemy_Def_Template_Diver.m_fScore = 1.0f;

	xEnemy_Def_Template_Hunter.m_fHealth = 50.0f;
	xEnemy_Def_Template_Hunter.m_fSize = 0.1f;
	xEnemy_Def_Template_Hunter.m_uBrain = eENEMY_BRAIN_SUICIDE;
	xEnemy_Def_Template_Hunter.m_uMaterial = GLToy_Hash_Constant( "Enemy/Enemy2" );
	xEnemy_Def_Template_Hunter.m_fSpeed = fX_WM_ENEMY_SPEED_NORMAL;
	xEnemy_Def_Template_Hunter.m_fScore = 5.0f;
	xEnemy_Def_Template_Hunter.m_uWeapon = eENEMY_WEAPON_SINGLE;
}

void X_Wave_Manager::Shutdown()
{
	for( u_int u = 0; u < X_WAVE_MANAGER_MAX_TIMED_SPAWNERS; ++u )
	{
		delete s_apxTimedSpawners[ u ];
		s_apxTimedSpawners[ u ] = 0;
	}
}

void X_Wave_Manager::Update()
{
	if( s_bRestPeriod )
	{
		// We are resting, wait for the enemy count to be zero, then pause before the next wave

		if( X_Entity_Enemy::GetList().GetCount() == 0 )
		{
			s_fWaveTimer += GLToy_Timer::GetFrameTime();
			if( s_fWaveTimer > fX_WAVE_MANAGER_REST_DURATION )
			{
				s_bRestPeriod = false;
				s_fWaveTimer = 0.0f;
				NextWave();
			}
		}
	}
	else
	{
		// We are spawning

		s_fWaveTimer += GLToy_Timer::GetFrameTime();

		for( u_int u = 0; u < X_WAVE_MANAGER_MAX_TIMED_SPAWNERS; ++u )
		{
			if( s_apxTimedSpawners[ u ] )
			{
				s_apxTimedSpawners[ u ]->Update();
			}
		}
	
		if( s_fWaveTimer > fX_WAVE_MANAGER_WAVE_DURATION )
		{
			for( u_int u = 0; u < X_WAVE_MANAGER_MAX_TIMED_SPAWNERS; ++u )
 			{
				delete s_apxTimedSpawners[ u ];
				s_apxTimedSpawners[ u ] = 0;
			}

			s_bRestPeriod = true;
			s_fWaveTimer = 0.0f;
		}
	}
}

void X_Wave_Manager::Render()
{
}

void X_Wave_Manager::NextWave()
{
	++s_uWave;

	X_Enemy_Definition xEnemy;
	u_int uSpawner = 0;

	switch( s_uWave )
	{
		case 1:
		{
			xEnemy = xEnemy_Def_Template_Diver;
			s_apxTimedSpawners[ uSpawner++ ] = new X_Timed_Spawner( eENEMY_SPAWNER_SCATTER, xEnemy, fX_WAVE_MANAGER_WAVE_DURATION * 0.00f, fX_WAVE_MANAGER_WAVE_DURATION * 0.20f );
			xEnemy.m_fSpeed = fX_WM_ENEMY_SPEED_SLOW;
			s_apxTimedSpawners[ uSpawner++ ] = new X_Timed_Spawner( eENEMY_SPAWNER_SWEEP,   xEnemy, fX_WAVE_MANAGER_WAVE_DURATION * 0.20f, fX_WAVE_MANAGER_WAVE_DURATION * 0.50f );
			xEnemy.m_fSpeed = fX_WM_ENEMY_SPEED_NORMAL;
			s_apxTimedSpawners[ uSpawner++ ] = new X_Timed_Spawner( eENEMY_SPAWNER_SCATTER, xEnemy, fX_WAVE_MANAGER_WAVE_DURATION * 0.60f, fX_WAVE_MANAGER_WAVE_DURATION * 0.90f );
			
			xEnemy = xEnemy_Def_Template_Hunter;
			xEnemy.m_fHealth = 80.0f;
			s_apxTimedSpawners[ uSpawner++ ] = new X_Timed_Spawner( eENEMY_SPAWNER_ONE, xEnemy, fX_WAVE_MANAGER_WAVE_DURATION * 0.2f );
			s_apxTimedSpawners[ uSpawner++ ] = new X_Timed_Spawner( eENEMY_SPAWNER_ONE, xEnemy, fX_WAVE_MANAGER_WAVE_DURATION * 0.3f );
			s_apxTimedSpawners[ uSpawner++ ] = new X_Timed_Spawner( eENEMY_SPAWNER_ONE, xEnemy, fX_WAVE_MANAGER_WAVE_DURATION * 0.4f );
			s_apxTimedSpawners[ uSpawner++ ] = new X_Timed_Spawner( eENEMY_SPAWNER_ONE, xEnemy, fX_WAVE_MANAGER_WAVE_DURATION * 0.5f );
			s_apxTimedSpawners[ uSpawner++ ] = new X_Timed_Spawner( eENEMY_SPAWNER_ONE, xEnemy, fX_WAVE_MANAGER_WAVE_DURATION * 0.6f );
			s_apxTimedSpawners[ uSpawner++ ] = new X_Timed_Spawner( eENEMY_SPAWNER_ONE, xEnemy, fX_WAVE_MANAGER_WAVE_DURATION * 0.7f );
			xEnemy.m_fHealth = 30.0f;
			xEnemy.m_uBrain = eENEMY_BRAIN_DIVE;
			s_apxTimedSpawners[ uSpawner++ ] = new X_Timed_Spawner( eENEMY_SPAWNER_SCATTER, xEnemy, fX_WAVE_MANAGER_WAVE_DURATION * 0.9f );
		}
		break;
	}
}

// eof
