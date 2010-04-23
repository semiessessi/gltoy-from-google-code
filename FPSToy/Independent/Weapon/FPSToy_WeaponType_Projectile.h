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

#ifndef __FPSTOY_WEAPONTYPE_PROJECTILE_H_
#define __FPSTOY_WEAPONTYPE_PROJECTILE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parent
#include <Weapon/FPSToy_Weapon.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class FPSToy_WeaponType_Projectile
: public FPSToy_WeaponType
{

    typedef FPSToy_WeaponType GLToy_Parent;

public:

    FPSToy_WeaponType_Projectile( const GLToy_Hash uHash, const GLToy_Hash uAmmoHash, const u_int uBurstCount = 1 )
    : GLToy_Parent( uHash, uAmmoHash, uBurstCount )
    , m_fInitialSpeed( 2000.0f )
    , m_fRadius( 0.0f )
    , m_uSpriteHash( uGLTOY_BAD_HASH )
    , m_uFlags( 0 )
    {
    }

    virtual void Fire( const GLToy_Hash uOwnerEntityHash, const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xDirection ) const;
	virtual void SetKeyValuePair( const GLToy_String& szKey, const GLToy_String& szValue );

    GLToy_Inline float GetRadius() const { return m_fRadius; }
    GLToy_Inline GLToy_Hash GetSpriteHash() const { return m_uSpriteHash; }
    GLToy_Inline bool GetMaintainSpeed() const { return m_bMaintainSpeed; }
    GLToy_Inline bool GetContactDetonation() const { return m_bContactDetonation; }

    GLToy_Inline void SetRadius( const float fRadius ) { m_fRadius = fRadius; }
    GLToy_Inline void SetSpriteHash( const GLToy_Hash uHash ) { m_uSpriteHash = uHash; }
    GLToy_Inline void SetMaintainSpeed( const bool bMaintainSpeed ) { m_bMaintainSpeed = bMaintainSpeed; }
    GLToy_Inline void SetContactDetonation( const bool bContactDetonation ) { m_bContactDetonation = bContactDetonation; }

protected:

    float m_fInitialSpeed;
    float m_fRadius;
    GLToy_Hash m_uSpriteHash;
    
    union
    {
        struct
        {
            u_int m_bMaintainSpeed : 1;
            u_int m_bContactDetonation : 1;
        };

        u_int m_uFlags;
    };

};

#endif