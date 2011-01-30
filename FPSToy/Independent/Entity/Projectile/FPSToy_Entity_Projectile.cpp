/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010, 2011 Semi Essessi
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

#include <Core/FPSToy.h>

// This file's header
#include <Entity/Projectile/FPSToy_Entity_Projectile.h>

// GLToy
#include <Entity/GLToy_Entity_System.h>
#include <Particle/GLToy_PFX_System.h>
#include <Physics/GLToy_Physics_Object.h>
#include <Physics/GLToy_Physics_System.h>
#include <Render/GLToy_Light_System.h>
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
, m_uLightHash( uGLTOY_BAD_HASH )
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

    if( m_uLightHash != uGLTOY_BAD_HASH )
    {
        GLToy_Light* const pxLight = GLToy_Light_System::FindLight( m_uLightHash );
        if( pxLight )
        {
            pxLight->SetPosition( m_pxPhysicsObject->GetPosition() );
        }
    }

    const FPSToy_WeaponType_Projectile* const pxProjectileType = GetWeaponType();

    if( !pxProjectileType )
    {
        return;
    }

    GLToy_ConstIterate( GLToy_Physics_ObjectCollision, xCollision, m_pxPhysicsObject->GetCollisions() )
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
    GLToy_Iterate_End;
}

void FPSToy_Entity_Projectile::Destroy()
{
    GLToy_Parent::Destroy();

    if( m_uLightHash != uGLTOY_BAD_HASH )
    {
        GLToy_Light_System::DestroyLight( m_uLightHash );
        m_uLightHash = uGLTOY_BAD_HASH;
    }

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

    // create a light if needed
    if( pxProjectileType->GetEmitLight() )
    {
        m_uLightHash = GLToy_Random_Hash();
        GLToy_Light_PointProperties xProperties;
        xProperties.m_xPosition = xPosition;
        xProperties.m_xColour = GLToy_Vector_3( 0.5f, 0.5f, 0.5f );
        xProperties.m_fSphereRadius = 150.0f;
        xProperties.m_uFlags = 0;
        GLToy_Light_System::AddPointLight( m_uLightHash, xProperties );
    }
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

    if( m_uLightHash != uGLTOY_BAD_HASH )
    {
        GLToy_Light_System::DestroyLight( m_uLightHash );
        m_uLightHash = uGLTOY_BAD_HASH;
    }

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
