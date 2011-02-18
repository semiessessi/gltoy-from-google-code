
#include "Core/GLToy.h"

#include "Core/X_Spawner.h"

#include "Core/X_Spawner_Types.h"

X_Enemy_Spawner::X_Enemy_Spawner( const X_Enemy_Definition& xDefinition )
: m_xDefinition( xDefinition )
{
}

X_Enemy_Spawner* X_Enemy_Spawner_Factory::CreateSpawner( GLToy_Hash uType, const X_Enemy_Definition& xDefinition )
{
	#define CASE(X) case X::uTYPE_HASH: return new X(xDefinition)

	switch( uType )
	{
		CASE( X_Spawner_Interval );
	}

	#undef CASE

	return 0;
}
