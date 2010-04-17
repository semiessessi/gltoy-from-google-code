/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of GLToy.
//
// GLToy is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// GLToy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with GLToy.
// If not, see <http://www.gnu.org/licenses/>.
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
static const float fPHYSICS_STEP_TIME = 1.0f / 41.0f;

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

    static GLToy_Physics_Object* CreatePhysicsPlane( const GLToy_Hash uHash, const GLToy_Plane& xPlane );
    static GLToy_Physics_Object* CreatePhysicsEnvironment( const GLToy_Hash uHash, const GLToy_Environment_Lightmapped& xEnvironment );
    static GLToy_Physics_Object* CreatePhysicsBox( const GLToy_Hash uHash, const GLToy_AABB& xAABB, const GLToy_Vector_3& xVelocity = GLToy_Maths::ZeroVector3 );
    static GLToy_Physics_Object* CreatePhysicsBox( const GLToy_Hash uHash, const GLToy_OBB& xOBB, const GLToy_Vector_3& xVelocity = GLToy_Maths::ZeroVector3 );
    static GLToy_Physics_Object* CreatePhysicsParticle( const GLToy_Hash uHash, const float fSize = 32.0f, const GLToy_Vector_3& xVelocity = GLToy_Maths::ZeroVector3 );

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
