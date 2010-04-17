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
#include <Damage/FPSToy_Damage_System.h>

// GLToy
#include <Core/Data Structures/GLToy_HashTree.h>
#include <Entity/GLToy_Entity.h>
#include <Entity/GLToy_Entity_System.h>

// FPSToy
#include <Damage/FPSToy_DamageType.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_HashTree< FPSToy_DamageType* > FPSToy_Damage_System::s_xDamageTypes;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool FPSToy_Damage_System::Initialise()
{
    return true;
}

void FPSToy_Damage_System::Shutdown()
{
}

void FPSToy_Damage_System::ApplyDamage( const GLToy_Hash uEntityHash, const float fAmount, const FPSToy_DamageType* const pxDamageType )
{
    GLToy_Entity* pxEntity = GLToy_Entity_System::FindEntity( uEntityHash );
    if( !pxEntity )
    {
        return;
    }

    if( !pxDamageType )
    {
        pxEntity->Hurt( fAmount );
        return;
    }
}