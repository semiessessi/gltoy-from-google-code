/////////////////////////////////////////////////////////////////////////////////////////////
//
// �Copyright 2010 Semi Essessi
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
#include <Entity/GLToy_Entity_System.h>
#include <Particle/GLToy_PFX_System.h>
#include <Physics/GLToy_Physics_Object.h>
#include <Physics/GLToy_Physics_System.h>
#include <Render/GLToy_Sprite.h>

// FPSToy
#include <Damage/FPSToy_Damage_System.h>
#include <Weapon/FPSToy_WeaponType_Projectile.h>
#include <Weapon/FPSToy_Weapon_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

FPSToy_Entity_Projectile::FPSToy_Entity_Projectile( const GLToy_Hash uHash, const u_int uType )
: GLToy_Parent( uHash, uType )
, m_xPosition( GLToy_Maths::ZeroVector3 )
, m_uOwnerEntityHash( uGLTOY_BAD_HASH )
, m_uWeaponTypeHash( uGLTOY_BAD_HASH )
, m_pxPhysicsObject( NULL )
, m_pxSprite( NULL )
{
}

FPSToy_Entity_Projectile::~FPSToy_Entity_Projectile()
{
    delete m_pxSprite;
}

const GLToy_Vector_3& FPSToy_Entity_Projectile::GetPosition() const
{
    return m_xPosition = m_pxPhysicsObject->GetPosition();
}

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

    if( m_pxSprite )
    {
        m_pxSprite->SetPosition( m_pxPhysicsObject->GetPosition() );
    }

    const FPSToy_WeaponType_Projectile* const pxProjectileType = GetWeaponType();

    if( !pxProjectileType )
    {
        return;
    }

    GLToy_ConstIterate( GLToy_Physics_ObjectCollision, xIterator, &( m_pxPhysicsObject->GetCollisions() ) )
    {
        const GLToy_Physics_ObjectCollision& xCollision = xIterator.Current();
        if( pxProjectileType->GetContactDetonation() )
        {
            // detonate
            if( xCollision.m_bHitEnvironment )
            {
                Detonate();
            }
            else if( xCollision.m_bHitEntity )
            {
                Detonate( xCollision.m_uEntityHash );
            }

            break;
        }
    }
}

void FPSToy_Entity_Projectile::Destroy()
{
    GLToy_Parent::Destroy();

    if( m_pxPhysicsObject )
    {
        GLToy_Physics_System::DestroyPhysicsObject( m_pxPhysicsObject->GetHash() );
        m_pxPhysicsObject = NULL;
    }
}

void FPSToy_Entity_Projectile::Spawn( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xDirection )
{
    const FPSToy_WeaponType_Projectile* const pxProjectileType = GetWeaponType();

    if( !pxProjectileType )
    {
        GLToy_Assert( pxProjectileType != NULL, "Unable to spawn projectile - can not find weapon type 0x%X", m_uWeaponTypeHash );
        return;
    }

    GLToy_Vector_3 xVelocity = xDirection;
    xVelocity.Normalise();
    xVelocity *= pxProjectileType->GetInitialSpeed(); // TODO - something better here
    m_pxPhysicsObject = GLToy_Physics_System::CreatePhysicsSphere( GetHash(), pxProjectileType->GetRadius(), xPosition, xVelocity );
    SetSprite( pxProjectileType->GetSpriteHash(), pxProjectileType->GetRadius() );
}

void FPSToy_Entity_Projectile::Detonate( const GLToy_Hash uVictimEntityHash )
{
    const FPSToy_WeaponType_Projectile* const pxProjectileType = GetWeaponType();

    if( !pxProjectileType )
    {
        GLToy_Assert( pxProjectileType != NULL, "Unable to detonate projectile - can not find weapon type 0x%X", m_uWeaponTypeHash );
        return;
    }
    // TODO - apply damage and forces etc...

    GLToy_PFX_System::CreatePFX( pxProjectileType->GetDetonationPFX(), GetPosition() );

    FPSToy_Damage_System::ApplyDamage( m_uOwnerEntityHash, uVictimEntityHash, 20.0f );

    GLToy_Entity_System::DestroyEntity( GetHash() );
}

const FPSToy_WeaponType_Projectile* FPSToy_Entity_Projectile::GetWeaponType() const
{
    return static_cast< const FPSToy_WeaponType_Projectile* >( FPSToy_Weapon_System::FindWeaponType( m_uWeaponTypeHash ) );
}

void FPSToy_Entity_Projectile::SetSprite( const GLToy_Hash uHash, const float fRadius )
{
    delete m_pxSprite;
    m_pxSprite = 0;
    if( uHash == uGLTOY_BAD_HASH )
    {
        return;
    }

    m_pxSprite = new GLToy_Sprite();
    m_pxSprite->SetTexture( uHash );
    m_pxSprite->SetSize( fRadius );
}