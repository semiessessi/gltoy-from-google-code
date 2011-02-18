
// TY 18.02.11

#ifndef _X_ENEMY_SPAWNER_
#define _X_ENEMY_SPAWNER_

#include "Core/GLToy.h"

#include "Entity/Enemy/X_Entity_Enemy.h"

#define X_SPAWNER_TYPE(X) public: static const GLToy_Hash uTYPE_HASH = X

class X_Enemy_Spawner
{
	public:

		X_Enemy_Spawner( const X_Enemy_Definition& xDefinition );
		virtual ~X_Enemy_Spawner() {};

		virtual void Update() = 0;

	protected:

		X_Enemy_Definition m_xDefinition;
};


class X_Enemy_Spawner_Factory
{
	public:

		static X_Enemy_Spawner* CreateSpawner( GLToy_Hash uType, const X_Enemy_Definition& xDefinition );
};

#endif

// eof
