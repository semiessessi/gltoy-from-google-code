
// TY 09.02.11

#ifndef _X_ENEMY_BRAIN_
#define _X_ENEMY_BRAIN_

#include "Core/GLToy.h"

#define X_BRAIN_TYPE(X) public: static const u_int uTYPE_HASH = X

class X_Entity_Enemy;
class X_Entity_Player;

class X_Enemy_Brain
{
	public:

		X_Enemy_Brain( GLToy_Hash uEnemy );
		virtual ~X_Enemy_Brain();

		virtual void Update() = 0;

	protected:

		X_Entity_Enemy* GetEntity();
		X_Entity_Player* GetPlayerEntity();

		void TurnTowards( GLToy_Vector_2 xPosition, float fTurnSpeed = 100.0f );
		void Accelerate( float fAccelSpeed, float fTopSpeed );

	private:

		GLToy_Hash m_uEnemyHash;
};

class X_Enemy_Brain_Factory
{
	public:

		static X_Enemy_Brain* CreateBrain( GLToy_Hash uBrainType, GLToy_Hash uEnemy );

		static u_int GetRandomBrainType();
};

//eof

#endif
