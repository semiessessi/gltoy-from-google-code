/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of FPSToy.
//
// FPSToy is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// FPSToy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with FPSToy.
// If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// This file's header
#include <Core/FPSToy.h>

// GLToy
#include <Core/State/GLToy_State_System.h>
#include <Entity/GLToy_Entity_System.h>
#include <Environment/GLToy_Environment_System.h>
#include <Input/GLToy_Input.h>
#include <Physics/GLToy_Physics_System.h>
#include <Render/GLToy_Camera.h>

// FPSToy
#include <Damage/FPSToy_Damage_System.h>
#include <Entity/FPSToy_EntityTypes.h>
#include <Weapon/FPSToy_Weapon_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool FPSToy::Initialise()
{
    GLToy_InitialiserCall( FPSToy_Damage_System );
    GLToy_InitialiserCall( FPSToy_Weapon_System );

    GLToy_Entity_System::SetProjectEntityCreateCallback( FPSToy_CreateEntity );

    GLToy_Environment_System::CreateTestEnvironment();

    GLToy_State_System::ChangeState( GLToy_Hash_Constant( "Splash" ) );
    GLToy_State_System::SetNextState( GLToy_Hash_Constant( "Editor" ) );

    GLToy::ChangeWindowTitle( "FPSToy" );
    GLToy::ChangeWindowIcon( "Icons/FPSToy_24x24.png" );

    return true;
}

void FPSToy::Shutdown()
{
    FPSToy_Damage_System::Shutdown();
    FPSToy_Weapon_System::Shutdown();
}

void FPSToy::Update()
{
    static bool s_bMouseDown = false;
    static bool s_bOldMouseDown = false;

    s_bMouseDown = GLToy_Input_System::IsMouseLeftButtonDown();

    if( !s_bOldMouseDown && s_bMouseDown )
    {
        GLToy_Physics_System::TestBox_Console();
    }

    s_bOldMouseDown = s_bMouseDown;
}