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
#include <Weapon/FPSToy_WeaponType_Projectile.h>

// GLToy
#include <Entity/GLToy_Entity_System.h>
#include <Maths/GLToy_Maths.h>

// FPSToy
#include <Entity/FPSToy_EntityTypes.h>
#include <Entity/Projectile/FPSToy_Entity_Projectile.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void FPSToy_WeaponType_Projectile::Fire( const GLToy_Hash uOwnerEntityHash, const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xDirection ) const
{
    static u_int uCount = 0;
    GLToy_String szEntityName;
    szEntityName.SetToFormatString( "Projectile%d", uCount );
    ++uCount;
    
    FPSToy_Entity_Projectile* const pxProjectile
        = static_cast< FPSToy_Entity_Projectile* const >(
            GLToy_Entity_System::CreateEntity( szEntityName.GetHash(), FPSTOY_ENTITY_PROJECTILE ) );

    if( !pxProjectile )
    {
        return;
    }

    pxProjectile->SetOwner( uOwnerEntityHash );
    pxProjectile->SetWeaponType( GetHash() );
    pxProjectile->Spawn( xPosition, xDirection );
}

void FPSToy_WeaponType_Projectile::SetKeyValuePair( const GLToy_String& szKey, const GLToy_String& szValue )
{
    if( szKey == "MaintainSpeed" )
    {
		m_bContactDetonation = !szValue.MeansFalse();
    }
    else if( szKey == "ContactDetonation" )
    {
		m_bContactDetonation = !szValue.MeansFalse();
    }
    else if( szKey == "Radius" )
    {
        // TODO - need something to get floats from strings
        SetRadius( static_cast< float >( szValue.ExtractUnsignedInt() ) );
    }
    else if( szKey == "Sprite" )
    {
        SetSpriteHash( szValue.GetHash() );
    }
	else if( szKey == "DetonationPFX" )
	{
		SetDetonationPFX( szValue.GetHash() );
	}
}
