

#include "Core/GLToy.h"

#include "X_Enemy_Brain_Types.h"

#include "Core/GLToy_Hash.h"

#include "Entity/Player/X_Entity_Player.h"

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

	TurnTowards( pxPlayer->GetPosition(), 0.5f );
	Accelerate( 0.1f, 2.0f );
}