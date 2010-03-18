/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Entity/GLToy_Entity_System.h>

// GLToy
#include <Core/Console/GLToy_Console.h>
#include <Core/Data Structures/GLToy_HashTree.h>
#include <Core/GLToy_UpdateFunctor.h>
#include <Entity/GLToy_Entity.h>
#include <Entity/Model/GLToy_Entity_ModelStatic.h>
#include <Entity/Model/GLToy_Entity_ModelAnimated.h>
#include <File/GLToy_EntityFile.h>
#include <File/GLToy_File_System.h>
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_RenderFunctor.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Entity_System::s_bRender = true;
bool GLToy_Entity_System::s_bRenderAABBs = false;
bool GLToy_Entity_System::s_bRenderOBBs = false;
bool GLToy_Entity_System::s_bRenderSpheres = false;
GLToy_HashTree< GLToy_Entity* > GLToy_Entity_System::s_xEntities;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Entity_System::Initialise()
{
    s_xEntities.Clear();

    GLToy_Console::RegisterVariable( "render.ents", &s_bRender );
    GLToy_Console::RegisterVariable( "render.aabbs", &s_bRenderAABBs );
    GLToy_Console::RegisterVariable( "render.obbs", &s_bRenderOBBs );
    GLToy_Console::RegisterVariable( "render.spheres", &s_bRenderSpheres ); // TODO
    
    GLToy_Console::RegisterCommand( "load.ents", LoadEntityFile );
    GLToy_Console::RegisterCommand( "save.ents", SaveEntityFile );
    GLToy_Console::RegisterCommand( "spawnanim", SpawnAnim_Console );
    GLToy_Console::RegisterCommand( "spawnmodel", SpawnModel_Console );

    return true;
}

void GLToy_Entity_System::Shutdown()
{
    DestroyEntities();
}

void GLToy_Entity_System::Update()
{
    s_xEntities.Traverse( GLToy_IndirectUpdateFunctor< GLToy_Entity >() );
}

void GLToy_Entity_System::Render()
{
    if( s_bRender )
    {
        s_xEntities.Traverse( GLToy_IndirectRenderFunctor< GLToy_Entity >() );
    }

    if( s_bRenderAABBs )
    {
        s_xEntities.Traverse( GLToy_IndirectRenderAABBFunctor< GLToy_Entity >() );
    }

    if( s_bRenderOBBs )
    {
        s_xEntities.Traverse( GLToy_IndirectRenderOBBFunctor< GLToy_Entity >() );
    }
}

GLToy_Entity* GLToy_Entity_System::FindEntity( const GLToy_Hash uHash )
{
    GLToy_Entity** ppxEntity = s_xEntities.FindData( uHash );
    return ppxEntity ? *ppxEntity : NULL;
}

GLToy_Entity* GLToy_Entity_System::LookUpEntity( const GLToy_String& szName )
{
    return FindEntity( szName.GetHash() );
}

void GLToy_Entity_System::Traverse( GLToy_ConstFunctor< GLToy_Entity* >& xFunctor )
{
    s_xEntities.Traverse( xFunctor );
}

GLToy_Entity* GLToy_Entity_System::CreateEntity( const GLToy_Hash uHash, const GLToy_EntityType eType )
{
    GLToy_Assert( !FindEntity( uHash ), "Trying to add entity with duplicate hash 0x%X", uHash );
    
    GLToy_Entity* pxNewEntity = NULL;

    switch( eType )
    {
        case ENTITY_NULL:               pxNewEntity = new GLToy_Entity_Null( uHash, eType ); break;
        case ENTITY_MODELSTATIC:        pxNewEntity = new GLToy_Entity_ModelStatic( uHash, eType ); break;
        case ENTITY_MODELANIMATED:      pxNewEntity = new GLToy_Entity_ModelAnimated( uHash, eType ); break;

        default:
        {
            GLToy_Assert( false, "Unrecognised entity type: %u", eType );
            break;
        }
    }

    if( pxNewEntity )
    {
        s_xEntities.AddNode( pxNewEntity, uHash );
    }

    return pxNewEntity;
}

void GLToy_Entity_System::DestroyEntities()
{
    s_xEntities.DeleteAll();
    s_xEntities.Clear();
}

void GLToy_Entity_System::LoadEntityFile( const GLToy_String& szName )
{
    GLToy_EntityFile( GLToy_String( "Environments/" ) + szName + ".ents" ).LoadEntities();
}

void GLToy_Entity_System::SaveEntityFile( const GLToy_String& szName )
{
    GLToy_EntityFile::Save( GLToy_String( "Environments/" ) + szName + ".ents" );
}

void GLToy_Entity_System::SpawnModel( const GLToy_String& szName, const GLToy_Vector_3& xPosition, const GLToy_Matrix_3& xOrientation )
{
    GLToy_String szEntityName;
    szEntityName.SetToFormatString( "Entity%d", s_xEntities.GetCount() );

    GLToy_Entity* pxEntity = CreateEntity( szEntityName.GetHash(), ENTITY_MODELSTATIC );
    GLToy_Entity_ModelStatic* pxModelEntity = reinterpret_cast< GLToy_Entity_ModelStatic* >( pxEntity );

    pxModelEntity->SetModel( szName );
    pxModelEntity->SetPosition( xPosition );
    pxModelEntity->SetOrientation( xOrientation );
}

void GLToy_Entity_System::SpawnAnim( const GLToy_String& szName, const GLToy_Vector_3& xPosition, const GLToy_Matrix_3& xOrientation )
{
    GLToy_String szEntityName;
    szEntityName.SetToFormatString( "Entity%d", s_xEntities.GetCount() );

    GLToy_Entity* pxEntity = CreateEntity( szEntityName.GetHash(), ENTITY_MODELANIMATED );
    GLToy_Entity_ModelAnimated* pxModelEntity = reinterpret_cast< GLToy_Entity_ModelAnimated* >( pxEntity );

    pxModelEntity->SetModel( szName );
    pxModelEntity->SetPosition( xPosition );
    pxModelEntity->SetOrientation( xOrientation );
}

void GLToy_Entity_System::SpawnAnim_Console( const GLToy_String& szName )
{
    SpawnAnim( szName, GLToy_Camera::GetPosition(), GLToy_Camera::GetOrientation() );

}
void GLToy_Entity_System::SpawnModel_Console( const GLToy_String& szName )
{
    SpawnModel( szName, GLToy_Camera::GetPosition(), GLToy_Camera::GetOrientation() );
}

#define TESTFORTYPE( szName, eType ) if( szString.GetHash() == GLToy_Hash_Constant( szName ) ) { return eType; }

GLToy_EntityType GLToy_EntityTypeFromString( const GLToy_String& szString )
{
    TESTFORTYPE( "null", ENTITY_NULL );
    TESTFORTYPE( "static model", ENTITY_MODELSTATIC );
    TESTFORTYPE( "animated model", ENTITY_MODELANIMATED );

    GLToy_Assert( false, "Unrecognised entity type: %S. Maybe you forgot to add it to GLToy_EntityTypeFromString?", szString.GetWideString() );
    return ENTITY_NULL;
}

#undef TESTFORTYPE