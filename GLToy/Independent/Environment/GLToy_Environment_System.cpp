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

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Environment/GLToy_Environment_System.h>

// GLToy
#include <Core/Console/GLToy_Console.h>
#include <Core/Data Structures/GLToy_HashTree.h>
#include <Entity/GLToy_Entity_System.h>
#include <Environment/GLToy_Environment.h>
#include <Environment/GLToy_Environment_Lightmapped.h>
#include <Environment/GLToy_Environment_Plane.h>
#include <File/GLToy_EnvironmentFile.h>
#include <File/GLToy_File_System.h>
#include <Maths/GLToy_Maths.h>
#include <Render/GLToy_Texture.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Environment* GLToy_Environment_System::s_pxCurrentEnvironment = NULL;
bool GLToy_Environment_System::s_bRender = true;
bool GLToy_Environment_System::s_bRenderLightmap = true;
bool GLToy_Environment_System::s_bRenderLightmapOnly = false;
bool GLToy_Environment_System::s_bBSPQuadRes = false;

GLToy_HashTree< GLToy_EnvironmentFile* > GLToy_Environment_System::s_xEnvironments;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Environment_System::Initialise()
{
    s_pxCurrentEnvironment = NULL;

    GLToy_Console::RegisterVariable( "render.env", &s_bRender );
    GLToy_Console::RegisterVariable( "render.lightmap", &s_bRenderLightmap );
    GLToy_Console::RegisterVariable( "render.lightmaponly", &s_bRenderLightmapOnly );

    GLToy_Console::RegisterCommand( "new.testenv", CreateTestEnvironment );

    GLToy_Console::RegisterCommand( "load.env", LoadEnvironmentFile );
    GLToy_Console::RegisterCommand( "save.env", SaveEnvironmentFile );

    GLToy_Console::RegisterVariable( "bsp.quadres", &s_bBSPQuadRes );
    
    s_xEnvironments.Clear();

    GLToy_Array< GLToy_String > xEnvPaths = GLToy_File_System::PathsFromFilter( "Environments/", "*.env" );
    GLToy_ConstIterate( GLToy_String, xIterator, &xEnvPaths )
    {
        GLToy_String xName = xIterator.Current();
        xName.RemoveAt( 0, 13 ); // remove "Environments/"
        xName.RemoveFromEnd( 4 ); // remove .env
        
        GLToy_DebugOutput( "   - Found environment \"%S\".\r\n", xName.GetWideString() );

        s_xEnvironments.AddNode( new GLToy_EnvironmentFile( xIterator.Current() ), xName.GetHash() );
    }

    GLToy_Array< GLToy_String > xBSPPaths = GLToy_File_System::PathsFromFilter( "Environments/", "*.bsp" );
    GLToy_ConstIterate( GLToy_String, xIterator, &xBSPPaths )
    {
        GLToy_String xName = xIterator.Current();
        xName.RemoveAt( 0, 13 ); // remove "Environments/"
        
        GLToy_DebugOutput( "   - Found environment \"%S\".\r\n", xName.GetWideString() );

        s_xEnvironments.AddNode( new GLToy_EnvironmentFile( xIterator.Current() ), xName.GetHash() );
    }

    return true;
}

void GLToy_Environment_System::Shutdown()
{
    s_xEnvironments.DeleteAll();
}

void GLToy_Environment_System::Render()
{
    if( s_pxCurrentEnvironment && s_bRender )
    {
        s_pxCurrentEnvironment->Render();
    }
}

void GLToy_Environment_System::Update()
{
    if( s_pxCurrentEnvironment )
    {
        s_pxCurrentEnvironment->Update();
    }
}

float GLToy_Environment_System::Trace( const GLToy_Ray& xRay, const float fLimitingDistance )
{
    return s_pxCurrentEnvironment ? s_pxCurrentEnvironment->Trace( xRay, fLimitingDistance ) : -1.0f;
}

void GLToy_Environment_System::LoadEnvironmentFile( const GLToy_String& szName )
{
    bool bNative = true;
    // try .env first ...
    GLToy_EnvironmentFile** ppxEnvFile = s_xEnvironments.FindData( szName.GetHash() );
    if( !ppxEnvFile )
    {
        // ... else .bsp
        ppxEnvFile = s_xEnvironments.FindData( ( szName + ".bsp" ).GetHash() );
        if( !ppxEnvFile )
        {
            return;
        }

        bNative = false;
    }

    ( *ppxEnvFile )->LoadEnvironment();

    if( bNative )
    {
        // try loading entities that match environment
        GLToy_Entity_System::LoadEntityFile( szName );
    }
}

void GLToy_Environment_System::SaveEnvironmentFile( const GLToy_String& szName )
{
    GLToy_EnvironmentFile::Save( GLToy_String( "Environments/" ) + szName + ".env" );
}

void GLToy_Environment_System::CreateTestEnvironment()
{
    GLToy_Entity_System::DestroyEntities();
    SwitchEnvironment( new GLToy_Environment_Plane( GLToy_Plane( GLToy_Vector_3( 0.0f, 1.0f, 0.0f ), 0.0f ), "generic/grid1.png" ) );
}

GLToy_Environment* GLToy_Environment_System::CreateEnvironmentFromType( const GLToy_EnvironmentType eType )
{
    GLToy_Environment* pxNewEnv = NULL;

    switch( eType )
    {
        case ENV_PLANE:         pxNewEnv = new GLToy_Environment_Plane( GLToy_Plane( GLToy_Vector_3( 0.0f, 1.0f, 0.0f ), 0.0f ), "generic/grid1.png" ); break;
        case ENV_LIGHTMAPPED:   pxNewEnv = new GLToy_Environment_Lightmapped(); break;

        default:
        {
            GLToy_Assert( false, "Unrecognised environment type: %u", eType );
            break;
        }
    }

    return pxNewEnv;
}

void GLToy_Environment_System::SwitchEnvironment( GLToy_Environment* const pxEnv )
{
    if( s_pxCurrentEnvironment )
    {
        s_pxCurrentEnvironment->Shutdown();
    }

    delete s_pxCurrentEnvironment;
    s_pxCurrentEnvironment = pxEnv;
    if( s_pxCurrentEnvironment )
    {
        s_pxCurrentEnvironment->Initialise();
    }
}

void GLToy_Environment_System::ClearCurrentEnvironment()
{
    if( s_pxCurrentEnvironment )
    {
        s_pxCurrentEnvironment->Shutdown();
    }

    delete s_pxCurrentEnvironment;
}

void GLToy_Environment_System::SetCurrentEnvironment( GLToy_Environment* const pxEnv )
{
    s_pxCurrentEnvironment = pxEnv;
    if( s_pxCurrentEnvironment )
    {
        s_pxCurrentEnvironment->Initialise();
    }
}
