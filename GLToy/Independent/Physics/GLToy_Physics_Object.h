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

#ifndef __GLTOY_PHYSICS_OBJECT_
#define __GLTOY_PHYSICS_OBJECT_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parent
#include <Core/GLToy_Updateable.h>

// GLToy
#include <Core/Data Structures/GLToy_Array.h>
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Physics_ObjectCollision
{

public:

    union
    {
        struct
        {
            u_int m_bHitEnvironment : 1;
            u_int m_bHitEntity : 1;

        };

        u_int m_uFlags;
    };

    u_int m_uEntityHash;
};

class GLToy_Physics_Object
: public GLToy_Updateable
{

#ifdef GLTOY_USE_HAVOK_PHYSICS

    friend class GLToy_Havok_PhysicsCollisionListener;

#endif

public:

    GLToy_Physics_Object( const GLToy_Hash uHash )
    : m_uHash( uHash )
    , m_xMovement( GLToy_Maths::ZeroVector2 )
    , m_xCollisions()
#ifdef GLTOY_USE_HAVOK_PHYSICS
    , m_pxHavokRigidBody( NULL )
    , m_pxHavokContext( NULL )
    , m_pHavokData( NULL )
#endif
    {
    }

    virtual ~GLToy_Physics_Object()
    {
#ifdef GLTOY_USE_HAVOK_PHYSICS
        m_pxHavokRigidBody = NULL;
#endif
    }

    void SetPosition( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xVelocity = GLToy_Maths::ZeroVector3 );
    void SetVelocity( const GLToy_Vector_3& xVelocity );

    const GLToy_Array< GLToy_Physics_ObjectCollision >& GetCollisions() const { return m_xCollisions; }
    void Destroy();
    void ResetCollisions() { m_xCollisions.Clear(); }
    void ControlMovement( const GLToy_Vector_2& xMovement );

    GLToy_OBB GetOBB() const;
    GLToy_Vector_3 GetPosition() const;
    GLToy_Inline GLToy_Hash GetHash() const { return m_uHash; }

    virtual void Update();
    virtual void LateUpdate();

#ifdef GLTOY_USE_HAVOK_PHYSICS

    void SetHavokRigidBodyPointer( class hkpRigidBody* const pxRigidBody ) { m_pxHavokRigidBody = pxRigidBody; }
    const hkpRigidBody* GetHavokRigidBodyPointer() const { return m_pxHavokRigidBody; }
    void SetHavokContextPointer( class hkpCharacterContext* const pxContext ) { m_pxHavokContext = pxContext; }
    hkpCharacterContext* GetHavokContextPointer() { return m_pxHavokContext; }
    const hkpCharacterContext* GetHavokContextPointer() const { return m_pxHavokContext; }
    void SetHavokData( void* const pData ) { m_pHavokData = pData; }
    void* GetHavokData() { return m_pHavokData; }
    const void* GetHavokData() const { return m_pHavokData; }

#endif

protected:

    GLToy_Hash m_uHash;
    GLToy_Vector_2 m_xMovement;
    GLToy_Array< GLToy_Physics_ObjectCollision > m_xCollisions;

#ifdef GLTOY_USE_HAVOK_PHYSICS
    
    class hkpRigidBody* m_pxHavokRigidBody;
    class hkpCharacterContext* m_pxHavokContext;
    void* m_pHavokData;

#endif

};


#endif
