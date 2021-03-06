/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010, 2011 Semi Essessi
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
    else if( szKey == "EmitLight" )
    {
        m_bEmitLight = !szValue.MeansFalse();
    }
}
