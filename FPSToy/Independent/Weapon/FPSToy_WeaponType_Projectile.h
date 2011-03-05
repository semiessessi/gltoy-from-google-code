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
    , m_fInitialSpeed( 400.0f )
    , m_fRadius( 0.0f )
    , m_uSpriteHash( uGLTOY_BAD_HASH )
    , m_uDetonationPFXHash( uGLTOY_BAD_HASH )
	, m_uDetonationSound( GLToy_Hash_Constant( "Explode" ) )
    , m_uFlags( 0 )
    {
        m_bMaintainSpeed = true;
        m_bContactDetonation = true;
    }

    virtual void Fire( const GLToy_Hash uOwnerEntityHash, const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xDirection ) const;
    virtual void SetKeyValuePair( const GLToy_String& szKey, const GLToy_String& szValue );

    GLToy_Inline float GetInitialSpeed() const { return m_fInitialSpeed; }
    GLToy_Inline float GetRadius() const { return m_fRadius; }
    GLToy_Inline GLToy_Hash GetSpriteHash() const { return m_uSpriteHash; }
    GLToy_Inline GLToy_Hash GetDetonationPFX() const { return m_uDetonationPFXHash; }
    GLToy_Inline bool GetMaintainSpeed() const { return m_bMaintainSpeed; }
    GLToy_Inline bool GetContactDetonation() const { return m_bContactDetonation; }
    GLToy_Inline bool GetEmitLight() const { return m_bEmitLight; }
	GLToy_Inline GLToy_Hash GetDetonationSound() const { return m_uDetonationSound; }

    GLToy_Inline void SetInitialSpeed( const float fSpeed ) { m_fInitialSpeed = fSpeed; }
    GLToy_Inline void SetRadius( const float fRadius ) { m_fRadius = fRadius; }
    GLToy_Inline void SetSpriteHash( const GLToy_Hash uHash ) { m_uSpriteHash = uHash; }
    GLToy_Inline void SetDetonationPFX( const GLToy_Hash uHash ) { m_uDetonationPFXHash = uHash; }
    GLToy_Inline void SetMaintainSpeed( const bool bMaintainSpeed ) { m_bMaintainSpeed = bMaintainSpeed; }
    GLToy_Inline void SetContactDetonation( const bool bContactDetonation ) { m_bContactDetonation = bContactDetonation; }
    GLToy_Inline void SetEmitLight( const bool bEmitLight ) { m_bEmitLight = bEmitLight; }

protected:

    float m_fInitialSpeed;
    float m_fRadius;
    GLToy_Hash m_uSpriteHash;
    GLToy_Hash m_uDetonationPFXHash;
	GLToy_Hash m_uDetonationSound;
    
    union
    {
        struct
        {
            u_int m_bMaintainSpeed : 1;
            u_int m_bContactDetonation : 1;
            u_int m_bEmitLight : 1;
        };

        u_int m_uFlags;
    };

};

#endif
