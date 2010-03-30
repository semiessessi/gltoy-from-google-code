/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of Physics.
//
// Physics is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// Physics is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with Physics.
// If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// This file's header
#include <Core/Physics.h>

// GLToy
#include <Core/State/GLToy_State_System.h>
#include <Entity/GLToy_Entity_System.h>
#include <Environment/GLToy_Environment_System.h>
#include <Input/GLToy_Input.h>

// Physics
#include <Entity/Physics_EntityTypes.h>
#include <Physics/Physics_Physics_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool Physics::Initialise()
{
    GLTOY_INITIALISER_CALL( Physics_Physics_System );

    GLToy_Entity_System::SetProjectEntityCreateCallback( Physics_CreateEntity );
    GLToy_Environment_System::CreateTestEnvironment();

    GLToy_State_System::ChangeState( GLToy_Hash_Constant( "Editor" ) );

    return true;
}

void Physics::Shutdown()
{
    Physics_Physics_System::Shutdown();
}

void Physics::Update()
{
    static bool s_bMouseDown = false;
    static bool s_bOldMouseDown = false;

    s_bMouseDown = GLToy_Input_System::IsMouseLeftButtonDown();

    if( !s_bOldMouseDown && s_bMouseDown )
    {
        Physics_Physics_System::TestBox_Console();
    }

    s_bOldMouseDown = s_bMouseDown;
    Physics_Physics_System::Update();
}