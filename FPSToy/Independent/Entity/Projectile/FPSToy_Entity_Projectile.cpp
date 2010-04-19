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
#include <Entity/Projectile/FPSToy_Entity_Projectile.h>

// GLToy
#include <Physics/GLToy_Physics_Object.h>
#include <Physics/GLToy_Physics_System.h>
#include <Render/GLToy_Sprite.h>

// FPSToy
#include <Weapon/FPSToy_WeaponType_Projectile.h>
#include <Weapon/FPSToy_Weapon_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void FPSToy_Entity_Projectile::Render() const
{
    if( !HasSpawned() )
    {
        return;
    }

    if( m_pxSprite )
    {
        m_pxSprite->Render();
    }
}

void FPSToy_Entity_Projectile::Update()
{
    if( !HasSpawned() )
    {
        return;
    }

    if( !m_pxPhysicsObject )
    {
        return;
    }

    const FPSToy_WeaponType_Projectile* const pxProjectileType = GetWeaponType();

    if( !pxProjectileType )
    {
        return;
    }

    GLToy_ConstIterate( GLToy_Physics_ObjectCollision, xIterator, &( m_pxPhysicsObject->GetCollisions() ) )
    {
        if( pxProjectileType->GetContactDetonation() )
        {
            // detonate
            break;
        }
    }
}

void FPSToy_Entity_Projectile::Spawn( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xVelocity )
{
    const FPSToy_WeaponType_Projectile* const pxProjectileType = GetWeaponType();

    if( !pxProjectileType )
    {
        GLToy_Assert( pxProjectileType != NULL, "Unable to spawn projectile - can not find weapon type 0x%X", m_uWeaponTypeHash );
        return;
    }

    m_pxPhysicsObject = GLToy_Physics_System::CreatePhysicsProjectile( GetHash(), pxProjectileType->GetRadius(), xVelocity );
}

void FPSToy_Entity_Projectile::Detonate( const GLToy_Hash uVictimEntityHash )
{
    // TODO - apply damage and forces etc...
    // Destroy();
}

const FPSToy_WeaponType_Projectile* FPSToy_Entity_Projectile::GetWeaponType() const
{
    return static_cast< const FPSToy_WeaponType_Projectile* >( FPSToy_Weapon_System::FindWeaponType( m_uWeaponTypeHash ) );
}