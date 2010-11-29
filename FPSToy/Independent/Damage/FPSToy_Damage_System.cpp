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
#include <Damage/FPSToy_Damage_System.h>

// GLToy
#include <Core/Data Structures/GLToy_HashMap.h>
#include <Entity/GLToy_Entity.h>
#include <Entity/GLToy_Entity_System.h>

// FPSToy
#include <Damage/FPSToy_DamageType.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_HashMap< FPSToy_DamageType* > FPSToy_Damage_System::s_xDamageTypes;

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

void FPSToy_Damage_System::ApplyDamage( const GLToy_Hash uTriggerHash, const GLToy_Hash uEntityHash, const float fAmount, const FPSToy_DamageType* const pxDamageType )
{
    GLToy_Entity* pxEntity = GLToy_Entity_System::FindEntity( uEntityHash );
    if( !pxEntity )
    {
        return;
    }

    if( !pxDamageType )
    {
        pxEntity->Hurt( uTriggerHash, fAmount );
        return;
    }
}
