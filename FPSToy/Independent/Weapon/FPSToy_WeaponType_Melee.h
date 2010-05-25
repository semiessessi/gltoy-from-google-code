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

#ifndef __FPSTOY_WEAPONTYPE_MELEE_H_
#define __FPSTOY_WEAPONTYPE_MELEE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parent
#include <Weapon/FPSToy_Weapon.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class FPSToy_WeaponType_Melee
: public FPSToy_WeaponType
{

    typedef FPSToy_WeaponType GLToy_Parent;

public:

    FPSToy_WeaponType_Melee( const GLToy_Hash uHash, const GLToy_Hash uAmmoHash, const u_int uBurstCount = 1 )
    : GLToy_Parent( uHash, uAmmoHash, uBurstCount )
    {
    }

    virtual void Fire( const GLToy_Hash uOwnerEntityHash, const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xDirection ) const;
    virtual void SetKeyValuePair( const GLToy_String& szKey, const GLToy_String& szValue );

};

#endif