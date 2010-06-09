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
