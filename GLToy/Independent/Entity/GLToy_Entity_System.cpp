/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Entity/GLToy_Entity_System.h>

// GLToy
#include <Core/Data Structures/GLToy_HashTree.h>
#include <Entity/GLToy_Entity.h>
#include <File/GLToy_File_System.h>

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

    return true;
}

void GLToy_Entity_System::Shutdown()
{
    s_xEntities.DeleteAll();
    s_xEntities.Clear();
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

    switch( eType )
    {
        case ENTITY_NULL: return new GLToy_Entity_Null( uHash, eType );
        // case ENTITY_MD2_STATIC: return new GLToy_Entity_ModelStatic( uHash, eType );

        default:
        {
            GLToy_Assert( false, "Unrecognised entity type: %u", eType );
            return NULL;
        }
    }
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