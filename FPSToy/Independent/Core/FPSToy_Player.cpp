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

#include <Core/FPSToy.h>

// This file's header
#include <Core/FPSToy_Player.h>

// GLToy
#include <Core/GLToy_Hash.h>
#include <Input/GLToy_Input.h>
#include <Physics/GLToy_Physics_System.h>
#include <Render/GLToy_Camera.h>

// FPSToy
#include <Weapon/FPSToy_Weapon.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

FPSToy_WeaponInventory FPSToy_Player::s_xWeaponInventory( FPSToy_Player::GetHash() );

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void FPSToy_Player::Spawn( const GLToy_Vector_3& xPosition, const GLToy_Matrix_3& xOrientation )
{
    s_xWeaponInventory.Reset();
    // TODO: something better than a magic constant here
    s_xWeaponInventory.AddWeapon( GLToy_Hash_Constant( "TestWeapon1" ) );

    GLToy_Camera::SetPosition( xPosition );
    GLToy_Camera::SetControllerCamEnabled( true );
}

void FPSToy_Player::Update()
{
    static bool s_bOldMouseDown = false;
    const bool bMouseDown = GLToy_Input_System::IsMouseLeftButtonDown();
    if( !s_bOldMouseDown && bMouseDown )
    {
        // TODO: something better than magic numbers here...
        s_xWeaponInventory.FireCurrent( GLToy_Camera::GetPosition() + GLToy_Camera::GetDirection() * 64.0f, GLToy_Camera::GetDirection() );
    }
    s_bOldMouseDown = bMouseDown;
}

GLToy_Hash FPSToy_Player::GetHash()
{
    return GLToy_Hash_Constant( "Player" );
}
