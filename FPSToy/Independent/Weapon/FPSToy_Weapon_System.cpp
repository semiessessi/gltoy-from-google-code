/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
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
#include <Weapon/FPSToy_Weapon_System.h>

// GLToy
#include <Core/Data Structures/GLToy_Array.h>
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
GLToy_HashTree< GLToy_Array< GLToy_Hash > > FPSToy_Weapon_System::s_xWeaponDefinitions;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool FPSToy_Weapon_System::Initialise()
{
    if( !InitialiseAmmoTypes() )
    {
        return false;
    }

    if( !InitialiseWeaponTypes() )
    {
        return false;
    }

    if( !InitialiseWeaponDefinitions() )
    {
        return false;
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

FPSToy_Weapon FPSToy_Weapon_System::CreateWeapon( const GLToy_Hash uHash, const GLToy_Hash uOwnerHash )
{
    FPSToy_Weapon xReturnValue( uOwnerHash );

    const GLToy_Array< GLToy_Hash >* const pxDefinition = s_xWeaponDefinitions.FindData( uHash );

    if( pxDefinition )
    {
        GLToy_ConstIterate( GLToy_Hash, xIterator, pxDefinition )
        {
            xReturnValue.AddMode( xIterator.Current() );
        }
    }

    return xReturnValue;
}

bool FPSToy_Weapon_System::InitialiseAmmoTypes()
{
    s_xAmmoTypes.DeleteAll();

    GLToy_Array< GLToy_String > xAmmoPaths = GLToy_File_System::PathsFromFilter( "Weapons/Ammo/", "*.ammo" );

    GLToy_ConstIterate( GLToy_String, xIterator, &xAmmoPaths )
    {
        GLToy_String szName = xIterator.Current();
        szName.RemoveAt( 0, 13 ); // remove "Weapons/Ammo/"
        szName.RemoveFromEnd( 5 ); // remove ".ammo"

        GLToy_DebugOutput( "   - Found ammo type \"%S\".\r\n", szName.GetWideString() );
        
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
        }

        // s_xAmmoTypes.AddNode( new GLToy_AmmoType( szName.GetHash() ), szName.GetHash() );
    }

    return true;
}

bool FPSToy_Weapon_System::InitialiseWeaponDefinitions()
{
    s_xWeaponDefinitions.Clear();

    GLToy_Array< GLToy_String > xWeaponDefinitionPaths = GLToy_File_System::PathsFromFilter( "Weapons/", "*.weapon" );

    GLToy_ConstIterate( GLToy_String, xIterator, &xWeaponDefinitionPaths )
    {
        GLToy_String szName = xIterator.Current();
        szName.RemoveAt( 0, 8 ); // remove "Weapons/"
        szName.RemoveFromEnd( 7 ); // remove ".weapon"

        GLToy_DebugOutput( "   - Found weapon definition \"%S\".\r\n", szName.GetWideString() );

        GLToy_ANSITextFile xFile( xIterator.Current() );

        GLToy_String xData = xFile.GetString();

        s_xWeaponDefinitions.AddNode( GLToy_Array< GLToy_Hash >(), szName.GetHash() );

        GLToy_Array< GLToy_Hash >* const pxDefinition = s_xWeaponDefinitions.FindData( szName.GetHash() );
        if( !pxDefinition )
        {
            GLToy_Assert( pxDefinition != NULL, "Somehow failed to find weapon definition immediately after creating it!" );
            continue;
        }

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

                if( szKey == "NewMode" )
                {
                    pxDefinition->Append( szValue.GetHash() );
                }
            }
        }
    }

    return true;
}

bool FPSToy_Weapon_System::InitialiseWeaponTypes()
{
    s_xWeaponTypes.DeleteAll();

    GLToy_Array< GLToy_String > xWeaponTypePaths = GLToy_File_System::PathsFromFilter( "Weapons/Types/", "*.weapontype" );

    GLToy_ConstIterate( GLToy_String, xIterator, &xWeaponTypePaths )
    {
        GLToy_String szName = xIterator.Current();
        szName.RemoveAt( 0, 14 ); // remove "Weapons/Types/"
        szName.RemoveFromEnd( 11 ); // remove ".weapontype"

        GLToy_DebugOutput( "   - Found weapon type \"%S\".\r\n", szName.GetWideString() );
        
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
        }

        FPSToy_WeaponType* pxWeaponType = NULL;
        switch( uClass )
        {
            case 0:     pxWeaponType = new FPSToy_WeaponType_Trace( szName.GetHash(), uAmmoHash, uBurstCount ); break;
            case 1:     pxWeaponType = new FPSToy_WeaponType_Projectile( szName.GetHash(), uAmmoHash, uBurstCount ); break;
            case 2:     pxWeaponType = new FPSToy_WeaponType_Melee( szName.GetHash(), uAmmoHash, uBurstCount ); break;
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
                    case 0:
                    {
                        static_cast< FPSToy_WeaponType_Trace* >( pxWeaponType )->SetKeyValuePair( szKey, szValue );
                        break;
                    }

                    case 1:
                    {
                        static_cast< FPSToy_WeaponType_Projectile* >( pxWeaponType )->SetKeyValuePair( szKey, szValue );
                        break;
                    }

                    case 2:
                    {
                        static_cast< FPSToy_WeaponType_Melee* >( pxWeaponType )->SetKeyValuePair( szKey, szValue );
                        break;
                    }
                }
            }
            else
            {
                // why not allow comments and whitespace?
                // GLToy_DebugOutput_Release( "Warning: Weapon type file \"%S\" contains a line with no valuable information", xIterator.Current().GetWideString() );
            }
        }

        if( pxWeaponType )
        {
            s_xWeaponTypes.AddNode( pxWeaponType, pxWeaponType->GetHash() );
        }
        else
        {
            GLToy_DebugOutput_Release( "  - Failed to create weapon type \"%S\".\r\n", szName.GetWideString() );
        }
    }

    return true;
}
