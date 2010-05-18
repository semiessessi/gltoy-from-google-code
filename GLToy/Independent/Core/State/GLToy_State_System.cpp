/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of GLToy.
//
// GLToy is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// GLToy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with GLToy.
// If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////////////////

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
#include <Core/State/GLToy_State_Splash.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_HashTree< GLToy_State* > GLToy_State_System::s_xStates;
GLToy_State* GLToy_State_System::s_pxCurrentState = 0;
GLToy_Hash GLToy_State_System::s_uNextState = uGLTOY_BAD_HASH;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_State_System::Initialise()
{
    s_xStates.Clear();

    RegisterState( new GLToy_State_EditorFrontEnd(), GLToy_Hash_Constant( "EditorFrontEnd" ) );
    RegisterState( new GLToy_State_Editor(), GLToy_Hash_Constant( "Editor" ) );
    RegisterState( new GLToy_State_Splash(), GLToy_Hash_Constant( "Splash" ) );

    GLToy_Console::RegisterCommand( "changestate", ChangeState_Console );

    return true;
}

void GLToy_State_System::Shutdown()
{
    s_xStates.DeleteAll();
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
        if( s_pxCurrentState->IsDone() )
        {
            ChangeState( s_uNextState );
            s_uNextState = uGLTOY_BAD_HASH;
        }

        if( s_pxCurrentState )
        {
            s_pxCurrentState->Update();
        }
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

bool GLToy_State_System::ChangeState( const GLToy_Hash uStateHash )
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
        return true;
    }

    return false;
}

void GLToy_State_System::ChangeState_Console( const GLToy_String& szName )
{
    if( GLToy_Console::IsDown() )
    {
        GLToy_Console::Toggle();
    }

    ChangeState( szName.GetHash() );
}
