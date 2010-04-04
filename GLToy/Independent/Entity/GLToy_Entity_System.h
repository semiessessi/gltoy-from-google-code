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

#ifndef __GLTOY_ENTITY_SYSTEM_H_
#define __GLTOY_ENTITY_SYSTEM_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Core/GLToy_Hash.h>
#include <Entity/GLToy_EntityTypes.h>
#include <String/GLToy_String.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class T > class GLToy_HashTree;
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

    static float Trace( const GLToy_Ray& xRay, const float fLimitingDistance = -1.0f );

    static GLToy_Entity* FindEntity( const GLToy_Hash uHash );
    static GLToy_Entity* LookUpEntity( const GLToy_String& szName );
    static void Traverse( GLToy_ConstFunctor< GLToy_Entity* >& xFunctor );

    static GLToy_Entity* CreateEntity( const GLToy_Hash uHash, const u_int uType );
    static void DestroyEntities();

    static void LoadEntityFile( const GLToy_String& szName );
    static void SaveEntityFile( const GLToy_String& szName );

    static u_int GetEntityCount();

    static void SpawnAnim( const GLToy_String& szName, const GLToy_Vector_3& xPosition, const GLToy_Matrix_3& xOrientation );
    static void SpawnModel( const GLToy_String& szName, const GLToy_Vector_3& xPosition, const GLToy_Matrix_3& xOrientation );
    static void SpawnSprite( const GLToy_String& szName, const GLToy_Vector_3& xPosition );

    static void SetRender( const bool bRender ) { s_bRender = bRender; };

    static void SetProjectEntityCreateCallback( GLToy_Entity* ( *pfnCreateFromType )( const GLToy_Hash, const u_int ) ) { s_pfnProject_CreateFromType = pfnCreateFromType; }

private:

    static void SpawnAnim_Console( const GLToy_String& szName );
    static void SpawnModel_Console( const GLToy_String& szName );
    static void SpawnSprite_Console( const GLToy_String& szName );

    static bool s_bRender;
    static bool s_bRenderAABBs;
    static bool s_bRenderOBBs;
    static bool s_bRenderSpheres;

    static GLToy_Entity* ( *s_pfnProject_CreateFromType )( const GLToy_Hash, const u_int );

    static GLToy_HashTree< GLToy_Entity* > s_xEntities;

};

#endif