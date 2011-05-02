/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include "Core/Solus.h"							// My header

#include "Core/State/GLToy_State_System.h"		// GLToy
#include "Entity/GLToy_Entity_System.h"
#include "String/GLToy_String.h"

#include "Entity/Solus_EntityTypes.h"			// Solus

#include <stdio.h>								// CRT

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool Solus::Initialise()
{
	char szWindowTitle[64];
	sprintf( szWindowTitle, "Solus - v%d.%d", iSOLUS_MAJOR_VERSION, iSOLUS_MAJOR_VERSION );
	GLToy::ChangeWindowTitle( szWindowTitle );

	GLToy_Entity_System::SetProjectEntityCreateCallback( Solus_CreateEntity );

	// TODO: Initialise various systems

	return true;
}

void Solus::Shutdown()
{
}

void Solus::Update()
{
}

// eof
