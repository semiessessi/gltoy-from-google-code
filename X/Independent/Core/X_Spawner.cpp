
#include "Core/GLToy.h"

#include "Core/X_Spawner.h"

#include "Entity/GLToy_Entity_System.h"		// GLToy

#include "Core/X_Spawner_Types.h"
#include "Entity/X_EntityTypes.h"

X_Enemy_Spawner::X_Enemy_Spawner( const X_Enemy_Definition& xDefinition )
: m_xDefinition( xDefinition )
{
}

void X_Enemy_Spawner::CreateEnemy( const GLToy_Vector_2& xPosition )
{
	GLToy_Vector_3 xFinalPosition;
	xFinalPosition.x = xPosition.x;
	xFinalPosition.y = xPosition.y;
	xFinalPosition.z = 0.0f;

	const GLToy_Hash uEnemyHash = GLToy_Random_Hash();
	X_Entity_Enemy* pxEnemy = static_cast< X_Entity_Enemy* >( GLToy_Entity_System::CreateEntity( uEnemyHash, X_ENTITY_ENEMY ) );
	pxEnemy->SetPosition( xFinalPosition );
	pxEnemy->SetDefinition( m_xDefinition );
}

// ___________________________________________________________________

X_Enemy_Spawner* X_Enemy_Spawner_Factory::CreateSpawner( GLToy_Hash uType, const X_Enemy_Definition& xDefinition )
{
	#define CASE(X) case X::uTYPE_HASH: return new X(xDefinition)

	switch( uType )
	{
		CASE( X_Spawner_Interval );
		CASE( X_Spawner_Sweep );
        CASE( X_Spawner_Scatter );
	}

	#undef CASE

	return 0;
}

u_int X_Enemy_Spawner_Factory::GetRandomSpawnerType()
{
	return static_cast<u_int>( GLToy_Maths::Random() * static_cast<float>( eENEMY_SPAWNER_NUM_TYPES ) );
}

//eof
