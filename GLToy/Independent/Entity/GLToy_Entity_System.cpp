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
#include <Entity/BSP/Common/GLToy_Entity_BSP_Func_Button.h>
#include <Entity/BSP/Common/GLToy_Entity_BSP_Info_Player_Start.h>
#include <Entity/BSP/Common/GLToy_Entity_BSP_Trigger_Counter.h>
#include <Entity/BSP/Quake 2/GLToy_Entity_BSP_Target_ChangeLevel.h>
#include <Entity/Model/GLToy_Entity_ModelStatic.h>
#include <Entity/Model/GLToy_Entity_ModelAnimated.h>
#include <Entity/Physics/GLToy_Entity_PhysicsBox.h>
#include <Entity/Sprite/GLToy_Entity_Sprite.h>
#include <File/GLToy_EntityFile.h>
#include <File/GLToy_File_System.h>
#include <Maths/GLToy_Maths.h>
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_RenderFunctor.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Entity_System::s_bRender = true;
bool GLToy_Entity_System::s_bRenderAABBs = false;
bool GLToy_Entity_System::s_bRenderOBBs = false;
bool GLToy_Entity_System::s_bRenderSpheres = false;
GLToy_Entity* ( *GLToy_Entity_System::s_pfnProject_CreateFromType )( const GLToy_Hash, const u_int ) = NULL;
GLToy_HashTree< GLToy_Entity* > GLToy_Entity_System::s_xEntities;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_EntitySaveFunctor
: public GLToy_ConstFunctor< GLToy_Entity* >
{

public:

    GLToy_EntitySaveFunctor( GLToy_BitStream& xBitStream )
    : m_xStream( xBitStream )
    {
    }

    void operator()( GLToy_Entity* const* const pxEntity )
    {
        ( *pxEntity )->WriteToBitStream( m_xStream );
    }

protected:

    GLToy_BitStream& m_xStream;

};

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

    GLToy_Console::RegisterCommand( "spawn.anim", SpawnAnim_Console );
    GLToy_Console::RegisterCommand( "spawn.model", SpawnModel_Console );
    GLToy_Console::RegisterCommand( "spawn.sprite", SpawnSprite_Console );

    return true;
}

void GLToy_Entity_System::Shutdown()
{
    DestroyEntities();
}

void GLToy_Entity_System::Update()
{
    GLToy_IndirectUpdateFunctor< GLToy_Entity > xFunctor;
    s_xEntities.Traverse( xFunctor );

    // destroy entities as required
    // TODO - I'm sure this can be optimised...
    for( u_int u = 0; u < s_xEntities.GetCount(); ++u )
    {
        GLToy_Entity* const pxEntity = s_xEntities[ u ];
        if( pxEntity->IsFlaggedForDestruction() )
        {
            s_xEntities.Remove( pxEntity->GetHash() );
            delete pxEntity;
            u = 0;
        }
    }
}

void GLToy_Entity_System::Render()
{
    if( s_bRender )
    {
        GLToy_IndirectRenderFunctor< GLToy_Entity > xFunctor;
        s_xEntities.Traverse( xFunctor  );
    }

    if( s_bRenderAABBs )
    {
        GLToy_IndirectRenderAABBFunctor< GLToy_Entity > xFunctor;
        s_xEntities.Traverse( xFunctor );
    }

    if( s_bRenderOBBs )
    {
        GLToy_IndirectRenderOBBFunctor< GLToy_Entity > xFunctor;
        s_xEntities.Traverse( xFunctor );
    }
}

float GLToy_Entity_System::Trace( const GLToy_Ray& xRay, const float fLimitingDistance, GLToy_Hash* const puHitEntity ) 
{
    float fMin = fLimitingDistance;
    float fParameter = 0;
    
    if( puHitEntity )
    {
        *puHitEntity = uGLTOY_BAD_HASH;
    }

    GLToy_ConstIterate( GLToy_Entity*, xIterator, &s_xEntities )
    {
        const GLToy_Entity* const pxEntity = xIterator.Current();
        if( pxEntity->IntersectWithRay( xRay, &fParameter ) )
        {
            if( fParameter < fMin )
            {
                fMin = fParameter;
                if( puHitEntity )
                {
                    *puHitEntity = pxEntity->GetHash();
                }
            }
        }
    }

    return fMin;
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

// CreateEntity is at the bottom of the file...

void GLToy_Entity_System::DestroyEntity( const GLToy_Hash uHash )
{
    GLToy_Entity* pxEntity = FindEntity( uHash );
    if( !pxEntity )
    {
        return;
    }

    pxEntity->Destroy();

    // do this later so as to not ruin the tree...
    //s_xEntities.Remove( uHash );
    //delete pxEntity;
}

void GLToy_Entity_System::DestroyEntities()
{
    s_xEntities.DeleteAll();
}

void GLToy_Entity_System::LoadEntityFile( const GLToy_String& szName )
{
    GLToy_EntityFile( GLToy_String( "Environments/" ) + szName + ".ents" ).LoadEntities();
}

void GLToy_Entity_System::SaveEntityFile( const GLToy_String& szName )
{
    GLToy_EntityFile::Save( GLToy_String( "Environments/" ) + szName + ".ents" );
}

void GLToy_Entity_System::LoadEntitiesFromStream( const GLToy_BitStream& xStream )
{
    GLToy_Entity_System::DestroyEntities();

    while( xStream.HasNMoreBytes( 5 ) )
    {
        u_char ucType;
        GLToy_Hash uHash;

        xStream >> ucType;
        xStream >> uHash;

        GLToy_Entity* pxEntity = GLToy_Entity_System::CreateEntity( uHash, static_cast< GLToy_EntityType >( ucType ) );

        if( pxEntity )
        {
            pxEntity->ReadFromBitStream( xStream );
        }
        else
        {
            GLToy_Assert( pxEntity != NULL, "Fatal error whilst loading entities - can not continue processing stream" );
            return;
        }
    }
}

void GLToy_Entity_System::SaveEntitiesToStream( GLToy_BitStream& xStream )
{
    GLToy_EntitySaveFunctor xFunctor( xStream );
    GLToy_Entity_System::Traverse( xFunctor );
}

void GLToy_Entity_System::SpawnModel( const GLToy_String& szName, const GLToy_Vector_3& xPosition, const GLToy_Matrix_3& xOrientation )
{
    const GLToy_String szEntityName = GLToy_String( "Entity" ) + s_xEntities.GetCount();

    GLToy_Entity_ModelStatic* pxModelEntity = static_cast< GLToy_Entity_ModelStatic* >( CreateEntity( szEntityName.GetHash(), ENTITY_MODELSTATIC ) );

    pxModelEntity->SetModel( szName );
    pxModelEntity->SetPosition( xPosition );
    pxModelEntity->SetOrientation( xOrientation );
}

u_int GLToy_Entity_System::GetEntityCount()
{
    return s_xEntities.GetCount();
}

void GLToy_Entity_System::SpawnAnim( const GLToy_String& szName, const GLToy_Vector_3& xPosition, const GLToy_Matrix_3& xOrientation )
{
    GLToy_String szEntityName;
    szEntityName.SetToFormatString( "Entity%d", s_xEntities.GetCount() );

    GLToy_Entity_ModelAnimated* pxModelEntity = static_cast< GLToy_Entity_ModelAnimated* >( CreateEntity( szEntityName.GetHash(), ENTITY_MODELANIMATED ) );

    pxModelEntity->SetModel( szName );
    pxModelEntity->SetPosition( xPosition );
    pxModelEntity->SetOrientation( xOrientation );
}

void GLToy_Entity_System::SpawnSprite( const GLToy_String& szName, const GLToy_Vector_3& xPosition )
{
    GLToy_String szEntityName;
    szEntityName.SetToFormatString( "Entity%d", s_xEntities.GetCount() );

    GLToy_Entity_Sprite* pxSpriteEntity = static_cast< GLToy_Entity_Sprite* >( CreateEntity( szEntityName.GetHash(), ENTITY_SPRITE ) );

    pxSpriteEntity->SetPosition( xPosition );
    pxSpriteEntity->SetTexture( szName );
    pxSpriteEntity->SetSize( 25.0f );
    pxSpriteEntity->SetBlendFunction( ucSPRITE_BLEND_NORMAL );
}

void GLToy_Entity_System::SpawnAnim_Console( const GLToy_String& szName )
{
    SpawnAnim( szName, GLToy_Camera::GetPosition(), GLToy_Camera::GetOrientation() );
}

void GLToy_Entity_System::SpawnModel_Console( const GLToy_String& szName )
{
    SpawnModel( szName, GLToy_Camera::GetPosition(), GLToy_Camera::GetOrientation() );
}

void GLToy_Entity_System::SpawnSprite_Console( const GLToy_String& szName )
{
    SpawnSprite( szName, GLToy_Camera::GetPosition() );
}

#define TESTFORTYPE( szName, uType ) if( szString.GetHash() == GLToy_Hash_Constant( szName ) ) { return uType; }

GLToy_EntityType GLToy_EntityTypeFromString( const GLToy_String& szString )
{
    TESTFORTYPE( "null", ENTITY_NULL );
    TESTFORTYPE( "static model", ENTITY_MODELSTATIC );
    TESTFORTYPE( "animated model", ENTITY_MODELANIMATED );
    TESTFORTYPE( "sprite", ENTITY_SPRITE );

    TESTFORTYPE( "func_button", ENTITY_BSP_FUNC_BUTTON );
    TESTFORTYPE( "info_player_start", ENTITY_BSP_INFO_PLAYER_START );
    TESTFORTYPE( "trigger_counter", ENTITY_BSP_TRIGGER_COUNTER );

    TESTFORTYPE( "target_changelevel", ENTITY_QUAKE2_TARGET_CHANGELEVEL );

    // GLToy_Assert( false, "Unrecognised entity type: %S. Maybe you forgot to add it to GLToy_EntityTypeFromString?", szString.GetWideString() );
    return ENTITY_NULL;
}

#undef TESTFORTYPE

GLToy_Entity* GLToy_Entity_System::CreateEntity( const GLToy_Hash uHash, const u_int uType )
{
    GLToy_Assert( !FindEntity( uHash ), "Trying to add entity with duplicate hash 0x%X", uHash );
    
    GLToy_Entity* pxNewEntity = NULL;

    if( s_pfnProject_CreateFromType )
    {
        pxNewEntity = s_pfnProject_CreateFromType( uHash, uType );
    }

    if( !pxNewEntity )
    {
        switch( uType )
        {
            case ENTITY_NULL:                       pxNewEntity = static_cast< GLToy_Entity* >( new GLToy_Entity_Null( uHash, uType ) ); break;

            case ENTITY_MODELSTATIC:                pxNewEntity = static_cast< GLToy_Entity* >( new GLToy_Entity_ModelStatic( uHash, uType ) ); break;
            case ENTITY_MODELANIMATED:              pxNewEntity = static_cast< GLToy_Entity* >( new GLToy_Entity_ModelAnimated( uHash, uType ) ); break;

            case ENTITY_SPRITE:                     pxNewEntity = static_cast< GLToy_Entity* >( new GLToy_Entity_Sprite( uHash, uType ) ); break;

            case ENTITY_PHYSICSBOX:                 pxNewEntity = static_cast< GLToy_Entity* >( new GLToy_Entity_PhysicsBox( uHash, uType ) ); break;


            case ENTITY_BSP_FUNC_BUTTON:            pxNewEntity = static_cast< GLToy_Entity* >( new GLToy_Entity_BSP_Func_Button( uHash, uType ) ); break;
            case ENTITY_BSP_INFO_PLAYER_START:      pxNewEntity = static_cast< GLToy_Entity* >( new GLToy_Entity_BSP_Info_Player_Start( uHash, uType ) ); break;
            case ENTITY_BSP_TRIGGER_COUNTER:        pxNewEntity = static_cast< GLToy_Entity* >( new GLToy_Entity_BSP_Trigger_Counter( uHash, uType ) ); break;

            case ENTITY_QUAKE2_TARGET_CHANGELEVEL:  pxNewEntity = static_cast< GLToy_Entity* >( new GLToy_Entity_BSP_Target_ChangeLevel( uHash, uType ) ); break;

            default:
            {
                GLToy_Assert( false, "Unrecognised entity type: %u", uType );
                break;
            }
        }
    }

    if( pxNewEntity )
    {
        s_xEntities.AddNode( pxNewEntity, uHash );
    }

    return pxNewEntity;
}

GLToy_Entity* GLToy_Entity_System::CreateEntity( const GLToy_Array< GLToy_Pair< GLToy_String > >& xKeyValuePairs )
{
    if( xKeyValuePairs.IsEmpty() )
    {
        return NULL;
    }

    static u_int uKeyValueEntityCount = 0;
    GLToy_String szNewEntityName = GLToy_String( "Entity" ) + uKeyValueEntityCount;
    ++uKeyValueEntityCount;

    // build a hash tree for convenient lookups
    GLToy_HashTree< GLToy_String > xValueTree;
    GLToy_ConstIterate( GLToy_Pair< GLToy_String >, xIterator, &xKeyValuePairs )
    {
        xValueTree.AddNode( xIterator.Current().Second(), xIterator.Current().First().GetHash() );
    }

    // find the entity class
    const GLToy_String* pszClass = xValueTree.FindData( GLToy_Hash_Constant( "classname" ) );

    // .. and "targetname" if it has one
    const GLToy_String* pszTargetName = xValueTree.FindData( GLToy_Hash_Constant( "targetname" ) );
    if( pszTargetName )
    {
        szNewEntityName = *pszTargetName;
    }

    if( !pszClass )
    {
        GLToy_DebugOutput_Release( "Warning: Entity has no classification!\r\n" );
        return NULL;
    }

    u_int uType = ENTITY_NULL;
    if( *pszClass == "worldspawn" )
    {
        // ... do nothing for now, maybe extract the environment name and gravity somehow?
    }
    else
    {
        uType = GLToy_EntityTypeFromString( *pszClass );
        if( uType == ENTITY_NULL )
        {
            GLToy_DebugOutput_Release( "Warning: GLToy does not support \"%S\" entities from BSP files\r\n", pszClass->GetWideString() );
            return NULL;
        }
    }

    GLToy_Entity* pxNewEntity = CreateEntity( szNewEntityName.GetHash(), uType );

    if( pxNewEntity )
    {
        GLToy_ConstIterate( GLToy_Pair< GLToy_String >, xIterator, &xKeyValuePairs )
        {
            pxNewEntity->SetKeyValuePair( xIterator.Current().First(), xIterator.Current().Second() );
        }
    }

    return pxNewEntity;
}
