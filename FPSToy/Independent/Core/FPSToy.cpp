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

// This file's header
#include <Core/FPSToy.h>

// GLToy
#include <Core/Console/GLToy_Console.h>
#include <Core/State/GLToy_State_System.h>
#include <Entity/GLToy_Entity_System.h>
#include <Environment/GLToy_Environment_System.h>
#include <Render/GLToy_Camera.h>

// FPSToy
#include <Core/FPSToy_Player.h>
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

    GLToy_State_System::ChangeState( GLToy_Hash_Constant( "Splash" ) );
    GLToy_State_System::SetNextState( GLToy_Hash_Constant( "Editor" ) );

    GLToy::ChangeWindowTitle( "FPSToy" );
    GLToy::ChangeWindowIcon( "Icons/FPSToy_24x24.png" );

    // in case we don't load an environment for whatever reason...
    FPSToy_Player::Spawn( GLToy_Vector_3( 0.0f, 100.0f, 0.0f ), GLToy_Maths::IdentityMatrix3 );

    return true;
}

void FPSToy::Shutdown()
{
    FPSToy_Damage_System::Shutdown();
    FPSToy_Weapon_System::Shutdown();
}

void FPSToy::Update()
{
    FPSToy_Player::Update();
}
