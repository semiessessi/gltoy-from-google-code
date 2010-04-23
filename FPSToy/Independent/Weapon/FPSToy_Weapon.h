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

#ifndef __FPSTOY_WEAPON_H_
#define __FPSTOY_WEAPON_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Core/Data Structures/GLToy_Array.h>
#include <Core/Data Structures/GLToy_HashTree.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_String;
class GLToy_Vector_3;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class FPSToy_AmmoType
{

public:

    FPSToy_AmmoType( const GLToy_Hash uHash )
    : m_uHash( uHash )
    {
    }

    GLToy_Inline const GLToy_Hash GetHash() const { return m_uHash; }
    GLToy_Inline const GLToy_Hash GetDamageTypeHash() const { return m_uDamageTypeHash; }

protected:

    GLToy_Hash m_uHash;
    GLToy_Hash m_uDamageTypeHash;

};

class FPSToy_WeaponType
{

public:

    FPSToy_WeaponType( const GLToy_Hash uHash, const GLToy_Hash uAmmoHash, const u_int uBurstCount = 1 )
    : m_uHash( uHash )
    , m_uAmmoTypeHash( uGLTOY_BAD_HASH )
    , m_uBurstCount( 1 )
    {
    }

    virtual void Fire( const GLToy_Hash uOwnerEntityHash, const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xDirection ) const = 0;
	virtual void SetKeyValuePair( const GLToy_String& szKey, const GLToy_String& szValue ) = 0;

    GLToy_Inline const u_int GetBurstCount() const { return m_uBurstCount; }
    GLToy_Inline const GLToy_Hash GetHash() const { return m_uHash; }
    GLToy_Inline const GLToy_Hash GetAmmoTypeHash() const { return m_uAmmoTypeHash; }

protected:

    GLToy_Hash m_uHash;
    GLToy_Hash m_uAmmoTypeHash;
    u_int m_uBurstCount;

};

class FPSToy_Weapon
{

public:

    FPSToy_Weapon( const GLToy_Hash uOwnerEntityHash = uGLTOY_BAD_HASH )
    : m_uCurrentWeaponType( 0 )
    , m_uOwnerEntityHash( uOwnerEntityHash )
    , m_xWeaponTypes()
    {
    }

    GLToy_Inline void Fire( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xDirection )
    {
		if( m_xWeaponTypes.GetCount() > 0 )
		{
			m_xWeaponTypes[ m_uCurrentWeaponType ]->Fire( m_uOwnerEntityHash, xPosition, xDirection );
		}
    }

	void AddMode( const GLToy_Hash uWeaponType );

    GLToy_Inline void NextMode()
    {
        m_uCurrentWeaponType = ( m_uCurrentWeaponType == ( m_xWeaponTypes.GetCount() - 1 ) ) ? 0 : ( m_uCurrentWeaponType + 1 );
    }

    GLToy_Inline void PreviousMode()
    {
        m_uCurrentWeaponType = ( m_uCurrentWeaponType == 0 ) ? ( m_xWeaponTypes.GetCount() - 1 ) : ( m_uCurrentWeaponType - 1 );
    }

    GLToy_Inline void SetMode( const u_int uMode )
    {
        m_uCurrentWeaponType = uMode;
    }

protected:

    u_int m_uCurrentWeaponType;
    GLToy_Hash m_uOwnerEntityHash;

    GLToy_Array < const FPSToy_WeaponType* > m_xWeaponTypes;

};

class FPSToy_WeaponInventory
{

public:

    FPSToy_WeaponInventory( const GLToy_Hash uOwnerEntityHash = uGLTOY_BAD_HASH )
	: m_uOwnerEntityHash( uOwnerEntityHash )
    , m_uCurrentWeapon( 0 )
    , m_xWeapons()
	, m_xAmmo()
    {
    }

    bool AddWeapon( const GLToy_Hash uWeaponHash );
    bool AddAmmo( const GLToy_Hash uAmmoTypeHash );

	bool HasWeapons() const { return m_xWeapons.GetCount() > 0; }

    GLToy_Inline void FireCurrent( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xDirection )
    {
        if( HasWeapons() )
        {
           m_xWeapons[ m_uCurrentWeapon ].Fire( xPosition, xDirection );
        }
    }

    GLToy_Inline void Next()
    {
        m_uCurrentWeapon = ( m_uCurrentWeapon == ( m_xWeapons.GetCount() - 1 ) ) ? 0 : m_uCurrentWeapon + 1;
    }

    GLToy_Inline void Previous()
    {
        m_uCurrentWeapon = ( m_uCurrentWeapon == 0 ) ? m_xWeapons.GetCount() - 1 : m_uCurrentWeapon - 1;
    }

	GLToy_Inline void Reset()
	{
		m_xWeapons.Clear();
		m_xAmmo.Clear();
	}

protected:

	GLToy_Hash m_uOwnerEntityHash;
    u_int m_uCurrentWeapon;
    GLToy_Array< FPSToy_Weapon > m_xWeapons;
	GLToy_HashTree< u_int > m_xAmmo;

};

#endif