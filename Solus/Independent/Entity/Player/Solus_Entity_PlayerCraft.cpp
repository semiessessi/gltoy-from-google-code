
/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/Solus.h>										// Main header

#include <Entity/Player/Solus_Entity_PlayerCraft.h>			// My header

// GLToy
//#include <Core/GLToy_Timer.h>
//#include <Entity/GLToy_Entity_System.h>
//#include <Material/GLToy_Material_System.h>
#include <Render/GLToy_Render.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

Solus_Entity_PlayerCraft::Solus_Entity_PlayerCraft( const GLToy_Hash uHash, const u_int uType )
: GLToy_Parent( uHash, uType )
{
}

Solus_Entity_PlayerCraft::~Solus_Entity_PlayerCraft()
{
}

void Solus_Entity_PlayerCraft::Update()
{
	GLToy_Parent::Update();
}

void Solus_Entity_PlayerCraft::Render() const
{
	if( GLToy_Render::HasDeferredBuffer() )
	{
		GLToy_Render::RegisterDeferred( this );
		return;
	}
	GLToy_Parent::Render();
}

void Solus_Entity_PlayerCraft::RenderDeferred() const
{
	// TODO: Model rendering doesnt support deffered rendering yet
}

//eof
