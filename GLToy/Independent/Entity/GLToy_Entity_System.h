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

template < class T > class GLToy_BSPTree;
template < class T > class GLToy_HashTree;
class GLToy_Entity;
class GLToy_Matrix_3;
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

    static GLToy_Entity* FindEntity( const GLToy_Hash uHash );
    static GLToy_Entity* LookUpEntity( const GLToy_String& szName );
    static void Traverse( GLToy_ConstFunctor< GLToy_Entity* >& xFunctor );

    static GLToy_Entity* CreateEntity( const GLToy_Hash uHash, const GLToy_EntityType uType );
    static void DestroyEntities();

    static void LoadEntityFile( const GLToy_String& szName );
    static void SaveEntityFile( const GLToy_String& szName );

    static void SpawnAnim( const GLToy_String& szName, const GLToy_Vector_3& xPosition, const GLToy_Matrix_3& xOrientation );
    static void SpawnModel( const GLToy_String& szName, const GLToy_Vector_3& xPosition, const GLToy_Matrix_3& xOrientation );

    static bool SetRender( const bool bRender ) { s_bRender = bRender; };

private:

    static void SpawnAnim_Console( const GLToy_String& szName );
    static void SpawnModel_Console( const GLToy_String& szName );

    static bool s_bRender;
    static bool s_bRenderAABBs;
    static bool s_bRenderOBBs;
    static bool s_bRenderSpheres;

    static GLToy_HashTree< GLToy_Entity* > s_xEntities;

};

#endif