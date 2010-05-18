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

#ifndef __FPSTOY_WEAPON_SYSTEM_H_
#define __FPSTOY_WEAPON_SYSTEM_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class T > class GLToy_Array;
template < class T > class GLToy_HashTree;

class FPSToy_AmmoType;
class FPSToy_Weapon;
class FPSToy_WeaponType;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class FPSToy_Weapon_System
{

public:

    static bool Initialise();
    static void Shutdown();

    static const FPSToy_WeaponType* FindWeaponType( const GLToy_Hash uHash );
    static FPSToy_Weapon CreateWeapon( const GLToy_Hash uHash, const GLToy_Hash uOwnerHash );

private:

    static bool InitialiseAmmoTypes();
    static bool InitialiseWeaponTypes();
    static bool InitialiseWeaponDefinitions();

    static GLToy_HashTree< FPSToy_AmmoType* > s_xAmmoTypes;
    static GLToy_HashTree< FPSToy_WeaponType* > s_xWeaponTypes;
    static GLToy_HashTree< GLToy_Array< GLToy_Hash > > s_xWeaponDefinitions;

};

#endif
