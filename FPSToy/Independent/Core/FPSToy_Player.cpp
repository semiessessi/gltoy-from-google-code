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
