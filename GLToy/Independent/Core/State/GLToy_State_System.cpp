/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Core/State/GLToy_State_System.h>

// GLToy
#include <Core/Data Structures/GLToy_HashTree.h>
#include <Core/State/GLToy_State.h>

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
    return true;
}

void GLToy_State_System::Shutdown()
{
    s_xStates.DeleteAll();
    s_xStates.Clear();
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