
#ifndef __SOLUS_ENTITY_PLAYER_CRAFT_H_
#define __SOLUS_ENTITY_PLAYER_CRAFT_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Entity/Model/GLToy_Entity_ModelStatic.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class Solus_Entity_PlayerCraft : public GLToy_Entity_ModelStatic
{

    typedef GLToy_Entity_ModelStatic GLToy_Parent;

	public:

		Solus_Entity_PlayerCraft( const GLToy_Hash uHash, const u_int uType );
		virtual ~Solus_Entity_PlayerCraft();

		virtual void Render() const;
		virtual void RenderDeferred() const;
		virtual void Update();
};

#endif

// eof
