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

#ifndef __FPSTOY_ENTITY_PROJECTILE_H_
#define __FPSTOY_ENTITY_PROJECTILE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parent
#include <Entity/GLToy_Entity.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Physics_Object;
class GLToy_Sprite;

class FPSToy_WeaponType_Projectile;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class FPSToy_Entity_Projectile
: public GLToy_Entity_Oriented_AABB
{

    typedef GLToy_Entity_Oriented_AABB GLToy_Parent;

public:

    FPSToy_Entity_Projectile( const GLToy_Hash uHash, const u_int uType );
    virtual ~FPSToy_Entity_Projectile();

    virtual void Render() const;
    virtual void Update();

    virtual void Destroy();

    virtual void Spawn( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xDirection );
    virtual void Detonate( const GLToy_Hash uVictimEntityHash = uGLTOY_BAD_HASH );

    GLToy_Inline void SetWeaponType( const GLToy_Hash uHash ) { m_uWeaponTypeHash = uHash; }
    GLToy_Inline void SetOwner( const GLToy_Hash uHash ) { m_uOwnerEntityHash = uHash; }
    void SetSprite( const GLToy_Hash uHash, const float fRadius );

    virtual const FPSToy_WeaponType_Projectile* GetWeaponType() const;
    
    virtual bool HasSpawned() const { return m_pxPhysicsObject != NULL; }

    virtual const GLToy_Vector_3& GetPosition() const;


protected:

    GLToy_Hash m_uOwnerEntityHash;
    GLToy_Hash m_uWeaponTypeHash;
    GLToy_Physics_Object* m_pxPhysicsObject;
    GLToy_Sprite* m_pxSprite;
    mutable GLToy_Vector_3 m_xPosition; // TODO - get rid of this

};

#endif
