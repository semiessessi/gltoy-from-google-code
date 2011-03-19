
// TY 18.02.11

#ifndef _X_ENEMY_SPAWNER_
#define _X_ENEMY_SPAWNER_

#include "Core/GLToy.h"

#include "Entity/Enemy/X_Entity_Enemy.h"

#define X_SPAWNER_TYPE(X) public: static const u_int uTYPE_HASH = X

class X_Enemy_Spawner
{
	public:

		X_Enemy_Spawner( const X_Enemy_Definition& xDefinition );
		virtual ~X_Enemy_Spawner() {};

		virtual void Update() = 0;

		void SetActive( bool bActive ) { m_bIsActive = bActive; }
		bool IsActive() { return m_bIsActive; }

	protected:

		void CreateEnemy( const GLToy_Vector_2& xPosition );

		X_Enemy_Definition m_xDefinition;

		bool m_bIsActive;
};


class X_Enemy_Spawner_Factory
{
	public:

		static X_Enemy_Spawner* CreateSpawner( GLToy_Hash uType, const X_Enemy_Definition& xDefinition );

		static u_int GetRandomSpawnerType();
};

#endif

// eof
