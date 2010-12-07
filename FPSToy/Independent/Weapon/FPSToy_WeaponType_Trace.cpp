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
#include <Weapon/FPSToy_WeaponType_Trace.h>

// GLToy
#include <Maths/GLToy_Maths.h>
#include <Trace/GLToy_Trace_System.h>

// FPSToy
#include <Damage/FPSToy_Damage_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void FPSToy_WeaponType_Trace::Fire( const GLToy_Hash uOwnerEntityHash, const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xDirection ) const
{
    const GLToy_Hash uHitEntity = GLToy_Trace_System::TraceEntity( GLToy_Ray( xPosition, xDirection ) );

    FPSToy_Damage_System::ApplyDamage( uOwnerEntityHash, uHitEntity, 20.0f );
}

void FPSToy_WeaponType_Trace::SetKeyValuePair( const GLToy_String& szKey, const GLToy_String& szValue )
{
}
