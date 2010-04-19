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
#include <Weapon/FPSToy_Weapon_System.h>

// GLToy
#include <Core/Data Structures/GLToy_HashTree.h>
#include <File/GLToy_ANSITextFile.h>
#include <File/GLToy_File_System.h>
#include <String/GLToy_String.h>

// FPSToy
#include <Weapon/FPSToy_Weapon.h>
#include <Weapon/FPSToy_WeaponType_Melee.h>
#include <Weapon/FPSToy_WeaponType_Projectile.h>
#include <Weapon/FPSToy_WeaponType_Trace.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_HashTree< FPSToy_AmmoType* > FPSToy_Weapon_System::s_xAmmoTypes;
GLToy_HashTree< FPSToy_WeaponType* > FPSToy_Weapon_System::s_xWeaponTypes;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool FPSToy_Weapon_System::Initialise()
{
    s_xAmmoTypes.DeleteAll();
    s_xWeaponTypes.DeleteAll();

    GLToy_Array< GLToy_String > xAmmoPaths = GLToy_File_System::PathsFromFilter( "Weapons/Ammo/", "*.ammo" );

    GLToy_ConstIterate( GLToy_String, xIterator, &xAmmoPaths )
    {
        GLToy_String xName = xIterator.Current();
        xName.RemoveAt( 0, 13 ); // remove "Weapons/Ammo/"
        xName.RemoveFromEnd( 5 ); // remove ".ammo"

        GLToy_DebugOutput( "   - Found ammo type \"%S\".\r\n", xName.GetWideString() );
        
        GLToy_ANSITextFile xFile( xIterator.Current() );

        GLToy_String xData = xFile.GetString();

        while( xData.GetLength() > 0 )
        {
            GLToy_String szValue = xData.RemoveFirstLine();
            if( szValue.Contains( L'=' ) )
            {
                GLToy_String szKey = szValue.RemoveUpTo( L'=' );
                if( szValue[ 0 ] == L'=' )
                {
                    szKey.RemoveAt( 0 );
                }

                //if( szKey == "" )
                //{
                //}
            }
            else
            {
                GLToy_DebugOutput_Release( "Warning: Ammo type file \"%S\" contains a line with no valuable information", xIterator.Current() );
            }
        }

        // s_xAmmoTypes.AddNode( new GLToy_AmmoType( xName.GetHash() ), xName.GetHash() );
    }

    // TODO: take the inside of the loop out into its own function for tidyness
    GLToy_Array< GLToy_String > xWeaponTypePaths = GLToy_File_System::PathsFromFilter( "Weapons/Types/", "*.weapontype" );

    GLToy_ConstIterate( GLToy_String, xIterator, &xWeaponTypePaths )
    {
        GLToy_String xName = xIterator.Current();
        xName.RemoveAt( 0, 14 ); // remove "Weapons/Types/"
        xName.RemoveFromEnd( 11 ); // remove ".weapontype"

        GLToy_DebugOutput( "   - Found weapon type \"%S\".\r\n", xName.GetWideString() );
        
        GLToy_ANSITextFile xFile( xIterator.Current() );

        GLToy_String xData = xFile.GetString();
        GLToy_String xDataCopy = xData;

        GLToy_Hash uAmmoHash = uGLTOY_BAD_HASH;
        u_int uBurstCount = 1;
        u_int uClass = 0;
        while( xData.GetLength() > 0 )
        {
            GLToy_String szValue = xData.RemoveFirstLine();
            if( szValue.Contains( L'=' ) )
            {
                GLToy_String szKey = szValue.RemoveUpTo( L'=' );
                if( szValue[ 0 ] == L'=' )
                {
                    szKey.RemoveAt( 0 );
                }

                if( szKey == "AmmoType" )
                {
                    uAmmoHash = szValue.GetHash();
                }
                else if( szKey == "Class" )
                {
                    if( szValue == "Trace" )
                    {
                        uClass = 0;
                    }
                    else if( szValue == "Projectile" )
                    {
                        uClass = 1;
                    }
                    else if( szValue == "Melee" )
                    {
                        uClass = 2;
                    }
                }
                else if( szKey == "Burst" )
                {
                    uBurstCount = szValue.ExtractUnsignedInt();
                }
            }
            else
            {
                GLToy_DebugOutput_Release( "Warning: Weapon type file \"%S\" contains a line with no valuable information", xIterator.Current().GetWideString() );
            }
        }

        FPSToy_WeaponType* pxWeaponType = NULL;
        switch( uClass )
        {
            case 0:     pxWeaponType = new FPSToy_WeaponType_Trace( xName.GetHash(), uAmmoHash, uBurstCount ); break;
            case 1:     pxWeaponType = new FPSToy_WeaponType_Projectile( xName.GetHash(), uAmmoHash, uBurstCount ); break;
            case 2:     pxWeaponType = new FPSToy_WeaponType_Melee( xName.GetHash(), uAmmoHash, uBurstCount ); break;
        }

        if( !pxWeaponType )
        {
            GLToy_Assert( pxWeaponType != NULL, "Failed to create weapon type from class ID %u", uClass );
            continue;
        }

        while( xDataCopy.GetLength() > 0 )
        {
            GLToy_String szValue = xDataCopy.RemoveFirstLine();
            if( szValue.Contains( L'=' ) )
            {
                GLToy_String szKey = szValue.RemoveUpTo( L'=' );
                if( szValue[ 0 ] == L'=' )
                {
                    szKey.RemoveAt( 0 );
                }

                switch( uClass )
                {
                    case 1:
                    {
                        FPSToy_WeaponType_Projectile* pxProjectileType = static_cast< FPSToy_WeaponType_Projectile* >( pxWeaponType );
                        if( szKey == "MaintainSpeed" )
                        {
                        }
                        else if( szKey == "ContactDetonation" )
                        {
                        }
                        else if( szKey == "Radius" )
                        {
                            // TODO - need something to get floats from strings
                            pxProjectileType->SetRadius( static_cast< float >( szValue.ExtractUnsignedInt() ) );
                        }
                        else if( szKey == "Sprite" )
                        {
                            pxProjectileType->SetSpriteHash( szValue.GetHash() );
                        }

                        break;
                    }
                }
            }
            else
            {
                GLToy_DebugOutput_Release( "Warning: Weapon type file \"%S\" contains a line with no valuable information", xIterator.Current().GetWideString() );
            }
        }

        if( pxWeaponType )
        {
            s_xWeaponTypes.AddNode( pxWeaponType, pxWeaponType->GetHash() );
        }
        else
        {
            GLToy_DebugOutput_Release( "  - Failed to create weapon type \"%S\".\r\n", xName.GetWideString() );
        }
    }

    return true;
}

void FPSToy_Weapon_System::Shutdown()
{
    s_xAmmoTypes.DeleteAll();
    s_xWeaponTypes.DeleteAll();
}

const FPSToy_WeaponType* FPSToy_Weapon_System::FindWeaponType( const GLToy_Hash uHash )
{
    const FPSToy_WeaponType* const* const ppxWeaponType = s_xWeaponTypes.FindData( uHash );
    return ppxWeaponType ? *ppxWeaponType : NULL;
}