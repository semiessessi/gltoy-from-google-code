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

#ifndef __GLTOY_PHYSICS_SYSTEM_
#define __GLTOY_PHYSICS_SYSTEM_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

#ifdef GLTOY_USE_HAVOK_PHYSICS

static const float fHAVOK_SCALE = 1.0f / 32.0f;
static const float fINVERSE_HAVOK_SCALE = 1.0f / fHAVOK_SCALE;
static const float fPHYSICS_STEP_TIME = 1.0f / 85.0f;

#endif

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_AABB;
class GLToy_Environment_Lightmapped;
template < class T > class GLToy_HashTree;
class GLToy_OBB;
class GLToy_Physics_Controller;
class GLToy_Physics_Object;
class GLToy_Plane;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Physics_System
{

public:

    static bool Initialise();
    static void Shutdown();
    
    static void Update();

    static void TestBox_Console();

    static void Reset() { Shutdown(); Initialise(); }

    static const GLToy_Physics_Controller& GetDefaultController() { return s_xDefaultController; }
    static void SetDefaultControllerActive( const bool bActive, const GLToy_Vector_3& xPosition = GLToy_Maths::ZeroVector3 );
    static void SetDefaultControllerPosition( const GLToy_Vector_3& xVector );
	
    static GLToy_Physics_Object* FindPhysicsObject( const GLToy_Hash uHash );

	static GLToy_Physics_Object* CreatePhysicsObject( const GLToy_Hash uHash );
	static GLToy_Physics_Object* CreateControlledCapsule( const GLToy_Hash uHash, const GLToy_Vector_3& xOrigin );
    static GLToy_Physics_Object* CreatePhysicsPlane( const GLToy_Hash uHash, const GLToy_Plane& xPlane );
    static GLToy_Physics_Object* CreatePhysicsEnvironment( const GLToy_Hash uHash, const GLToy_Environment_Lightmapped& xEnvironment );
    static GLToy_Physics_Object* CreatePhysicsBox( const GLToy_Hash uHash, const GLToy_AABB& xAABB, const GLToy_Vector_3& xVelocity = GLToy_Maths::ZeroVector3 );
    static GLToy_Physics_Object* CreatePhysicsBox( const GLToy_Hash uHash, const GLToy_OBB& xOBB, const GLToy_Vector_3& xVelocity = GLToy_Maths::ZeroVector3 );
    static GLToy_Physics_Object* CreatePhysicsParticle( const GLToy_Hash uHash, const float fRadius = 32.0f, const GLToy_Vector_3& xPosition = GLToy_Maths::ZeroVector3, const GLToy_Vector_3& xVelocity = GLToy_Maths::ZeroVector3 );
    static GLToy_Physics_Object* CreatePhysicsSphere( const GLToy_Hash uHash, const float fRadius = 32.0f, const GLToy_Vector_3& xPosition = GLToy_Maths::ZeroVector3, const GLToy_Vector_3& xVelocity = GLToy_Maths::ZeroVector3 );
    
    static GLToy_Physics_Object* CreateFixedBox( const GLToy_Hash uHash, const GLToy_AABB& xAABB );
	#ifdef GLTOY_USE_HAVOK_PHYSICS
	static void CreateCollisionListener( class hkpRigidBody* pxRigidBody );
	#endif

    static void DestroyPhysicsObject( const GLToy_Hash uHash );

    static void ResetCollisions();

#ifdef GLTOY_USE_HAVOK_PHYSICS

    static class hkpWorld* GetHavokWorld();

#endif

private:

    static GLToy_HashTree< GLToy_Physics_Object* > s_xPhysicsObjects;
    static GLToy_Physics_Controller s_xDefaultController;

};

#ifdef GLTOY_USE_HAVOK_PHYSICS

void GLToy_Havok_MarkForRead();
void GLToy_Havok_MarkForWrite();
void GLToy_Havok_UnmarkForRead();
void GLToy_Havok_UnmarkForWrite();

#endif

#endif
