/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Core/State/GLToy_State_EditorFrontEnd.h>

// GLToy
#include <Entity/GLToy_Entity_System.h>
#include <Environment/GLToy_Environment_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_State_EditorFrontEnd::Initialise()
{
    GLToy_Entity_System::DestroyEntities();
    GLToy_Environment_System::SwitchEnvironment( NULL );
}

void GLToy_State_EditorFrontEnd::Shutdown()
{
}

void GLToy_State_EditorFrontEnd::Render() const
{
}

void GLToy_State_EditorFrontEnd::Update()
{
}
