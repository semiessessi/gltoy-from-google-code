

#include "Core/GLToy.h"

#include "X_Enemy_Brain_Types.h"

#include "Core/GLToy_Hash.h"

#include "Entity/Enemy/X_Entity_Enemy.h"
#include "Entity/Player/X_Entity_Player.h"

// _________________________________________________________________________________________________________

X_Enemy_Brain_Suicide::X_Enemy_Brain_Suicide( GLToy_Hash uEnemy ) : X_Enemy_Brain( uEnemy )
{
}

X_Enemy_Brain_Suicide::~X_Enemy_Brain_Suicide()
{
}

void X_Enemy_Brain_Suicide::Update()
{
	X_Entity_Enemy* pxEntity = GetEntity();
	X_Entity_Player* pxPlayer = GetPlayerEntity();

	const float fTurnSpeed = ( pxPlayer->GetPosition() - pxEntity->GetPosition() ).Magnitude() * 2.5f;
	if( pxEntity->GetPosition().y > pxPlayer->GetPosition().y )
	{
		TurnTowards( pxPlayer->GetPosition(), fTurnSpeed );
		Accelerate( 0.5f, 1.0f );
	}
	else
	{
		GLToy_Vector_2 xPos( 0.0f, -2.0f );
		TurnTowards( xPos, 2.5f );
	}
}

// _________________________________________________________________________________________________________

X_Enemy_Brain_Swarm::X_Enemy_Brain_Swarm( GLToy_Hash uEnemy ) : X_Enemy_Brain( uEnemy )
{
}

X_Enemy_Brain_Swarm::~X_Enemy_Brain_Swarm()
{
}

void X_Enemy_Brain_Swarm::Update()
{
	/*X_Entity_Enemy* pxEntity = GetEntity();
	X_Entity_Player* pxPlayer = GetPlayerEntity();

	GLToy_Array<X_Entity_Enemy*> xList = X_Entity_Enemy::GetList();
	
	GLToy_Random::
	xList.GetCount()

	const float fTurnSpeed = ( pxPlayer->GetPosition() - pxEntity->GetPosition() ).Magnitude() * 2.5f;
	if( pxEntity->GetPosition().y > pxPlayer->GetPosition().y )
	{
		TurnTowards( pxPlayer->GetPosition(), fTurnSpeed );
		Accelerate( 0.5f, 1.0f );
	}
	else
	{
		GLToy_Vector_2 xPos( 0.0f, -2.0f );
		TurnTowards( xPos, 2.5f );
	}*/
}