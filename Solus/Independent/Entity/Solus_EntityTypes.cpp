
/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include "Core/Solus.h"									// Main header

#include <Entity/Solus_EntityTypes.h>					// My header

#include <Entity/GLToy_Entity.h>						// GLToy
#include <Entity/GLToy_Entity_System.h>

#include <Entity/Player/Solus_Entity_PlayerCraft.h>		// Solus

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Entity* Solus_CreateEntity( const GLToy_Hash uHash, const u_int uType )
{
	switch( uType )
	{
	
		case SOLUS_ENTITY_PLAYER_CRAFT: return new Solus_Entity_PlayerCraft( uHash, uType );
	}

	return 0;
}

// eof
