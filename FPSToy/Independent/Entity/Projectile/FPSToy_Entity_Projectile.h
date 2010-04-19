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

#ifndef __FPSTOY_ENTITY_PROJECTILE_H_
#define __FPSTOY_ENTITY_PROJECTILE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parent
#include <Entity/GLToy_Entity.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Physics_Object;
class GLToy_Sprite;

class FPSToy_WeaponType_Projectile;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class FPSToy_Entity_Projectile
: public GLToy_Entity_Oriented_AABB
{

    typedef GLToy_Entity_Oriented_AABB GLToy_Parent;

public:

    FPSToy_Entity_Projectile( const GLToy_Hash uHash, const u_int uType )
    : GLToy_Parent( uHash, uType )
    , m_uOwnerEntityHash( uGLTOY_BAD_HASH )
    , m_uWeaponTypeHash( uGLTOY_BAD_HASH )
    , m_pxPhysicsObject( NULL )
    , m_pxSprite( NULL )
    {
    }

    virtual void Render() const;
    virtual void Update();

    virtual void Spawn( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xVelocity );
    virtual void Detonate( const GLToy_Hash uVictimEntityHash = uGLTOY_BAD_HASH );

    GLToy_Inline void SetWeaponType( const GLToy_Hash uHash ) { m_uWeaponTypeHash = uHash; }
    GLToy_Inline void SetOwner( const GLToy_Hash uHash ) { m_uOwnerEntityHash = uHash; }

    virtual const FPSToy_WeaponType_Projectile* GetWeaponType() const;
    
    virtual bool HasSpawned() const { return m_pxPhysicsObject != NULL; }


protected:

    GLToy_Hash m_uOwnerEntityHash;
    GLToy_Hash m_uWeaponTypeHash;
    GLToy_Physics_Object* m_pxPhysicsObject;
    GLToy_Sprite* m_pxSprite;

};

#endif