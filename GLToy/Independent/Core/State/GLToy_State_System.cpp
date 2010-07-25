/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
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
#include <Core/State/GLToy_State_FixedCamera.h>
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
    RegisterState( new GLToy_State_FixedCamera(), GLToy_Hash_Constant( "FixedCamera" ) );
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
