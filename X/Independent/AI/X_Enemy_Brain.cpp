
#include "Core/GLToy.h"

#include "Core/GLToy_Functor.h"
#include "Core/GLToy_Hash.h"
#include "Core/GLToy_Timer.h"
#include "Maths/GLToy_Maths.h"

#include "X_Enemy_Brain.h"
#include "X_Enemy_Brain_Types.h"

#include "Entity/Enemy/X_Entity_Enemy.h"
#include "Entity/Player/X_Entity_Player.h"

X_Enemy_Brain::X_Enemy_Brain( GLToy_Hash uEnemy )
: m_uEnemyHash( uEnemy ) 
{
}


X_Enemy_Brain::~X_Enemy_Brain()
{
}


X_Entity_Enemy* X_Enemy_Brain::GetEntity()
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


X_Entity_Player* X_Enemy_Brain::GetPlayerEntity()
{
	GLToy_Array<X_Entity_Player*> xList = X_Entity_Player::GetList();
	return xList[0];
}


void X_Enemy_Brain::TurnTowards( GLToy_Vector_2 xPosition, float fTurnSpeed )
{
	X_Entity_Enemy* pxEntity = GetEntity();
	GLToy_Vector_2 xEntityDir = pxEntity->GetDirection();

	xEntityDir.Normalise();
	
	GLToy_Vector_2 xNewDir = xPosition - pxEntity->GetPosition();
	if( xNewDir.ApproximatelyEqual( GLToy_Maths::ZeroVector2 ) )
	{
		return;
	}
	xNewDir.Normalise();
	
	xNewDir = GLToy_Maths::ClampedLerp( xEntityDir, xNewDir, GLToy_Timer::GetFrameTime() * fTurnSpeed );

	pxEntity->SetDirection( xNewDir );
}


void X_Enemy_Brain::Accelerate( float fAccelSpeed, float fTopSpeed )
{
	X_Entity_Enemy* pxEntity = GetEntity();

	float fSpeed = pxEntity->GetSpeed();

	fSpeed = GLToy_Maths::ClampedLerp( fSpeed, fTopSpeed, GLToy_Timer::GetFrameTime() * fAccelSpeed );

	pxEntity->SetSpeed( fSpeed );
}

// ____________________________________________________________________________________________________________________

X_Enemy_Brain* X_Enemy_Brain_Factory::CreateBrain( GLToy_Hash uBrainType, GLToy_Hash uEnemy )
{
	#define CASE(X) case X::uTYPE_HASH: return new X(uEnemy)

	switch( uBrainType )
	{
		CASE( X_Enemy_Brain_Suicide );
	}

	#undef CASE

	return 0;
}


//eof
