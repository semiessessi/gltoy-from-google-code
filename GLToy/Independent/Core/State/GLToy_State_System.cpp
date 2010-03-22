/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Core/State/GLToy_State_System.h>

// GLToy
#include <Core/Console/GLToy_Console.h>
#include <Core/Data Structures/GLToy_HashTree.h>
#include <Core/State/GLToy_State.h>
#include <Core/State/GLToy_State_Editor.h>
#include <Core/State/GLToy_State_EditorFrontEnd.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_HashTree< GLToy_State* > GLToy_State_System::s_xStates;
GLToy_State* GLToy_State_System::s_pxCurrentState = 0;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_State_System::Initialise()
{
    s_xStates.Clear();

    RegisterState( new GLToy_State_EditorFrontEnd(), GLToy_Hash_Constant( "EditorFrontEnd" ) );
    RegisterState( new GLToy_State_Editor(), GLToy_Hash_Constant( "Editor" ) );

    GLToy_Console::RegisterCommand( "changestate", ChangeState_Console );

    return true;
}

void GLToy_State_System::Shutdown()
{
    s_xStates.DeleteAll();
    s_xStates.Clear();
}

void GLToy_State_System::Render()
{
    if( s_pxCurrentState )
    {
        s_pxCurrentState->Render();
    }
}

void GLToy_State_System::Render2D()
{
    if( s_pxCurrentState )
    {
        s_pxCurrentState->Render2D();
    }
}

void GLToy_State_System::Update()
{
    if( s_pxCurrentState )
    {
        s_pxCurrentState->Update();
    }
}

void GLToy_State_System::RegisterState( GLToy_State* const pxState, const GLToy_Hash uStateHash )
{
    if( !pxState )
    {
        GLToy_Assert( pxState != NULL, "Passing null pointer into GLToy_State_System::RegisterState!" );
        return;
    }

    s_xStates.AddNode( pxState, uStateHash );
}

void GLToy_State_System::ChangeState( const GLToy_Hash uStateHash )
{
    if( s_pxCurrentState )
    {
        s_pxCurrentState->Shutdown();
        s_pxCurrentState = NULL;
    }

    GLToy_State** ppxState = s_xStates.FindData( uStateHash );

    if( ppxState )
    {
        s_pxCurrentState = *ppxState;
        s_pxCurrentState->Initialise();
    }
}

void GLToy_State_System::ChangeState_Console( const GLToy_String& szName )
{
	if( GLToy_Console::IsDown() )
	{
		GLToy_Console::Toggle();
	}

	ChangeState( szName.GetHash() );
}