
// TY 18.2.11

#include "Core/GLToy.h"					// Main header

#include "X_Spawner_Types.h"			// My header

#include "Core/GLToy_Timer.h"			// GLToy
#include "Entity/GLToy_Entity_System.h"

#include "Entity/X_EntityTypes.h"		// X

X_Spawner_Interval::X_Spawner_Interval( const X_Enemy_Definition& xDefinition )
: X_Enemy_Spawner( xDefinition )
, m_fTimer( 0.0f )
{

}

void X_Spawner_Interval::Update()
{
	const float fSpawnInterval = 2.0f;

	m_fTimer += GLToy_Timer::GetFrameTime();
	if( m_fTimer > fSpawnInterval )
	{
		GLToy_Vector_2 xPosition( GLToy_Maths::Random( -0.8f, 0.8f ), 1.1f );
		CreateEnemy( xPosition );
		
		m_fTimer = 0.0f;
	}
}

// _____________________________________________________________

X_Spawner_Sweep::X_Spawner_Sweep( const X_Enemy_Definition& xDefinition )
: X_Enemy_Spawner( xDefinition )
, m_fTimer( 0.0f )
, m_fSpawnTimer( 0.0f )
, m_bSpawning( false )
, m_bReverse( true )
{
}


void X_Spawner_Sweep::Update()
{
	const float fSpawnInterval = 0.2f;
	const float fSpawnReset = 2.0f;

	m_fTimer += GLToy_Timer::GetFrameTime();
	m_fSpawnTimer += GLToy_Timer::GetFrameTime();

	if( m_bSpawning && m_fTimer > fSpawnInterval )
	{
		GLToy_Vector_2 xPosition( ( m_fSpawnTimer / fSpawnReset * 2.0f - 1.0f ) * ( m_bReverse ? -1.0f : 1.0f ), 1.1f );
		CreateEnemy( xPosition );
		m_fTimer = 0.0f;
	}
	if( m_fSpawnTimer > fSpawnReset )
	{
		m_bSpawning = !m_bSpawning;
		m_fSpawnTimer = 0.0f;
		if( m_bSpawning )
		{
			m_bReverse = !m_bReverse;
		}
	}
}

X_Spawner_Scatter::X_Spawner_Scatter( const X_Enemy_Definition& xDefinition )
: X_Enemy_Spawner( xDefinition )
, m_fTimer( 0.0f )
, m_fSpawnTimer( 0.0f )
{
}


void X_Spawner_Scatter::Update()
{
	static float fSpawnReset = 2.0f;

	m_fSpawnTimer += GLToy_Timer::GetFrameTime();

	if( m_fSpawnTimer > fSpawnReset )
	{
		m_fSpawnTimer = 0.0f;
        fSpawnReset = GLToy_Maths::Random( 0.5f, 2.0f );
        GLToy_Vector_2 xPosition( GLToy_Maths::Random( -1.0f, 1.0f ), 1.1f );
		CreateEnemy( xPosition );
	}
}


//eof
