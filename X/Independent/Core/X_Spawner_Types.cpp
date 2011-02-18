
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
	const float fSpawnInterval = 3.0f;

	m_fTimer = m_fTimer + GLToy_Timer::GetFrameTime();
	if( m_fTimer > fSpawnInterval )
	{
		const GLToy_Hash uEnemyHash = GLToy_Random_Hash();
		X_Entity_Enemy* pxEnemy = static_cast< X_Entity_Enemy* >( GLToy_Entity_System::CreateEntity( uEnemyHash, X_ENTITY_ENEMY ) );
		pxEnemy->SetDefinition( m_xDefinition );
		// TODO: Move this start position into the enemy definition
		pxEnemy->SetPosition( GLToy_Vector_3( GLToy_Maths::Random( -1.0f, 1.0f ), 1.5f, 0.0f ) );

		m_fTimer = 0.0f;
	}
}

//eof
