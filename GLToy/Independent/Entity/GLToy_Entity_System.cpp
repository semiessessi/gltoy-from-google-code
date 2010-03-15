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
#include <File/GLToy_File_System.h>
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_RenderFunctor.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_HashTree< GLToy_Entity* > GLToy_Entity_System::s_xEntities;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Entity_System::Initialise()
{
    s_xEntities.Clear();

    GLToy_Console::RegisterCommand( "spawnmodel", SpawnModel_Console );

    return true;
}

void GLToy_Entity_System::Shutdown()
{
    s_xEntities.DeleteAll();
    s_xEntities.Clear();
}

void GLToy_Entity_System::Update()
{
    s_xEntities.Traverse( GLToy_IndirectUpdateFunctor< GLToy_Entity >() );
}

void GLToy_Entity_System::Render()
{
    s_xEntities.Traverse( GLToy_IndirectRenderFunctor< GLToy_Entity >() );
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

GLToy_Entity* GLToy_Entity_System::CreateEntity( const GLToy_Hash uHash, const GLToy_EntityType eType )
{
    GLToy_Assert( !FindEntity( uHash ), "Trying to add entity with duplicate hash 0x%X", uHash );
    
    GLToy_Entity* pxNewEntity = NULL;

    switch( eType )
    {
        case ENTITY_NULL:               pxNewEntity = new GLToy_Entity_Null( uHash, eType ); break;
        case ENTITY_MODELSTATIC:        pxNewEntity = new GLToy_Entity_ModelStatic( uHash, eType ); break;

        default:
        {
            GLToy_Assert( false, "Unrecognised entity type: %u", eType );
            break;
        }
    }

    if( pxNewEntity )
    {
        s_xEntities.AddNode( pxNewEntity, pxNewEntity->GetHash() );
    }

    return pxNewEntity;
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

void GLToy_Entity_System::SpawnModel_Console( const GLToy_String& szName )
{
    SpawnModel( szName, GLToy_Camera::GetPosition(), GLToy_Maths::IdentityMatrix3 );
}

#define TESTFORTYPE( szName, eType ) if( szString.GetHash() == GLToy_Hash_Constant( szName ) ) { return eType; }

GLToy_EntityType GLToy_EntityTypeFromString( const GLToy_String& szString )
{
    TESTFORTYPE( "null", ENTITY_NULL );
    TESTFORTYPE( "static model", ENTITY_MODELSTATIC );

    GLToy_Assert( false, "Unrecognised entity type: %S", szString.GetWideString() );
    return ENTITY_NULL;
}

#undef TESTFORTYPE