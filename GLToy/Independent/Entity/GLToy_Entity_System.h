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

#ifndef __GLTOY_ENTITY_SYSTEM_H_
#define __GLTOY_ENTITY_SYSTEM_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Core/Data Structures/GLToy_Pair.h>
#include <Core/GLToy_Hash.h>
#include <Entity/GLToy_EntityTypes.h>
#include <String/GLToy_String.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class T > class GLToy_HashMap;
class GLToy_BitStream;
class GLToy_Entity;
class GLToy_Matrix_3;
class GLToy_Ray;
class GLToy_Vector_3;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Entity_System
{

public:

    static bool Initialise();
    static void Shutdown();

    static void Update();
    static void Render();

	static void DebugRender();

    static float Trace( const GLToy_Ray& xRay, const float fLimitingDistance = -1.0f, GLToy_Hash* const puHitEntity = NULL, GLToy_Vector_3* const pxPosition = NULL, GLToy_Vector_3* const pxNormal = NULL );

    static GLToy_Entity* FindEntity( const GLToy_Hash uHash );
    static GLToy_Entity* LookUpEntity( const GLToy_String& szName );
    static void Traverse( GLToy_ConstFunctor< GLToy_Entity* >& xFunctor );

    static GLToy_Entity* CreateEntity( const GLToy_Hash uHash, const u_int uType );
    static GLToy_Entity* CreateEntity( const GLToy_Array< GLToy_Pair< GLToy_String > >& xKeyValuePairs );
    static void DestroyEntity( const GLToy_Hash uHash );
    static void DestroyEntities();

    static void LoadEntityFile( const GLToy_String& szName );
    static void SaveEntityFile( const GLToy_String& szName );

    static void LoadEntitiesFromStream( const GLToy_BitStream& xStream );
    static void SaveEntitiesToStream( GLToy_BitStream& xStream );

    static u_int GetEntityCount();

    static void SpawnAnim( const GLToy_String& szName, const GLToy_Vector_3& xPosition, const GLToy_Matrix_3& xOrientation );
    static void SpawnModel( const GLToy_String& szName, const GLToy_Vector_3& xPosition, const GLToy_Matrix_3& xOrientation );
    static void SpawnSprite( const GLToy_String& szName, const GLToy_Vector_3& xPosition );

    static void SetRender( const bool bRender ) { s_bRender = bRender; };

    static GLToy_ForceInline void SetProjectEntityCreateCallback( GLToy_Entity* ( *pfnCreateFromType )( const GLToy_Hash, const u_int ) ) { s_pfnProject_CreateFromType = pfnCreateFromType; }

private:

    static void SpawnAnim_Console( const GLToy_String& szName );
    static void SpawnModel_Console( const GLToy_String& szName );
    static void SpawnSprite_Console( const GLToy_String& szName );

    static bool s_bRender;
    static bool s_bRenderAABBs;
    static bool s_bRenderOBBs;
    static bool s_bRenderSpheres;

    static GLToy_Entity* ( *s_pfnProject_CreateFromType )( const GLToy_Hash, const u_int );
	static bool ( *s_pfnProject_SupportKeyValueEntity )( const GLToy_Array< GLToy_Pair< GLToy_String > >& );
	static GLToy_Entity* ( *s_pfnProject_CreateKeyValueEntity )( const GLToy_Array< GLToy_Pair< GLToy_String > >& );

    static GLToy_HashMap< GLToy_Entity* > s_xEntities;

};

#endif
