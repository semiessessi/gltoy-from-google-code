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

#ifndef __GLTOY_PHYSICS_CONTROLLER_
#define __GLTOY_PHYSICS_CONTROLLER_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Physics_Controller
{

public:

    GLToy_Physics_Controller( const GLToy_Hash uHash )
    : m_uHash( uHash )
    , m_bOldJump( false )
    , m_bCameraControl( false )
    , m_bActive( false )
    , m_xMovement( GLToy_Maths::ZeroVector2 )
#ifdef GLTOY_USE_HAVOK_PHYSICS
    , m_pxHavokRigidBody( NULL )
    , m_pxHavokContext( NULL )
    , m_pxStandShape( NULL )
    , m_pxCrouchShape( NULL )
#endif
    {
    }

    void Create( const GLToy_Vector_3& xPosition = GLToy_Maths::ZeroVector3 );
    void Destroy();

    void Update( const float fTimestep );
    void LateUpdate();

    GLToy_ForceInline bool IsActive() const { return m_bActive; }

    void SetPosition( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xVelocity = GLToy_Maths::ZeroVector3 );
    void SetVelocity( const GLToy_Vector_3& xVelocity );

    GLToy_ForceInline void SetCameraControl( const bool bCameraControl ) { m_bCameraControl = bCameraControl; }

    GLToy_ForceInline void SetMovementDirection( const GLToy_Vector_2& xDirection ) { m_xMovement = xDirection; }

#ifdef GLTOY_USE_HAVOK_PHYSICS

    GLToy_ForceInline void SetHavokRigidBodyPointer( class hkpCharacterRigidBody* const pxRigidBody ) { m_pxHavokRigidBody = pxRigidBody; }
    GLToy_ForceInline const class hkpCharacterRigidBody* GetHavokRigidBodyPointer() const { return m_pxHavokRigidBody; }
    GLToy_ForceInline void SetHavokContextPointer( class hkpCharacterContext* const pxContext ) { m_pxHavokContext = pxContext; }
    GLToy_ForceInline const class hkpCharacterContext* GetHavokContextPointer() const { return m_pxHavokContext; }

#endif

protected:

    GLToy_Hash m_uHash;
    bool m_bOldJump;
    bool m_bActive;
    bool m_bCameraControl;
    GLToy_Vector_2 m_xMovement;

#ifdef GLTOY_USE_HAVOK_PHYSICS
    
    class hkpCharacterRigidBody* m_pxHavokRigidBody;
    class hkpCharacterContext* m_pxHavokContext;
    class hkpCapsuleShape* m_pxStandShape;
    class hkpCapsuleShape* m_pxCrouchShape;

#endif

};

#endif
