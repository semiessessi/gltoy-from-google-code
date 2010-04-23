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
#include <Weapon/FPSToy_Weapon.h>

// FPSToy
#include <Weapon/FPSToy_Weapon_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void FPSToy_Weapon::AddMode( const GLToy_Hash uWeaponType )
{
	const FPSToy_WeaponType* const pxType = FPSToy_Weapon_System::FindWeaponType( uWeaponType );
	if( !pxType )
	{
		return;
	}

	m_xWeaponTypes.Append( pxType );
}

bool FPSToy_WeaponInventory::AddWeapon( const GLToy_Hash uWeaponHash )
{
	m_xWeapons.Append( FPSToy_Weapon_System::CreateWeapon( uWeaponHash, m_uOwnerEntityHash ) );
	return false;
}