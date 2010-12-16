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
#include <Environment/GLToy_Environment_System.h>

// GLToy
#include <Core/Console/GLToy_Console.h>
#include <Core/Data Structures/GLToy_HashMap.h>
#include <Entity/GLToy_Entity_System.h>
#include <Environment/GLToy_Environment.h>
#include <Environment/GLToy_Environment_Lightmapped.h>
#include <Environment/GLToy_Environment_Plane.h>
#include <File/GLToy_EnvironmentFile.h>
#include <File/GLToy_File_System.h>
#include <Maths/GLToy_Maths.h>
#include <Physics/GLToy_Physics_System.h>
#include <Render/GLToy_Texture.h>
#include <UI/GLToy_UI_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Environment* GLToy_Environment_System::s_pxCurrentEnvironment = NULL;
bool GLToy_Environment_System::s_bRender = true;
bool GLToy_Environment_System::s_bRenderLightmap = true;
bool GLToy_Environment_System::s_bRenderLightmapOnly = false;
bool GLToy_Environment_System::s_bBSPQuadRes = true;
bool GLToy_Environment_System::s_bDebugRender = false;
GLToy_Environment* ( *GLToy_Environment_System::s_pfnProject_CreateFromType )( const u_int ) = NULL;
GLToy_HashMap< GLToy_EnvironmentFile* > GLToy_Environment_System::s_xEnvironments;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Environment_System::Initialise()
{
    s_pxCurrentEnvironment = NULL;

    GLToy_Console::RegisterVariable( "render.env", &s_bRender );
    GLToy_Console::RegisterVariable( "render.env.debug", &s_bDebugRender );
    GLToy_Console::RegisterVariable( "render.lightmap", &s_bRenderLightmap );
    GLToy_Console::RegisterVariable( "render.lightmaponly", &s_bRenderLightmapOnly );

    GLToy_Console::RegisterCommand( "new.testenv", CreateTestEnvironment );

    GLToy_Console::RegisterCommand( "load.env", LoadEnvironmentFile );
    GLToy_Console::RegisterCommand( "save.env", SaveEnvironmentFile );

    GLToy_Console::RegisterVariable( "bsp.quadres", &s_bBSPQuadRes );
    
    s_xEnvironments.Clear();

    GLToy_Array< GLToy_String > xEnvPaths = GLToy_File_System::PathsFromFilter( "Environments/", "*.env" );
    GLToy_ConstIterate( GLToy_String, szPath, xEnvPaths )
        GLToy_String szName = szPath;
        szName.RemoveAt( 0, 13 ); // remove "Environments/"
        szName.RemoveFromEnd( 4 ); // remove .env
        
        GLToy_DebugOutput( "   - Found environment \"%S\".\r\n", szName.GetWideString() );

        s_xEnvironments.AddNode( new GLToy_EnvironmentFile( szPath ), szName.GetHash() );
    GLToy_Iterate_End;

    GLToy_Array< GLToy_String > xBSPPaths = GLToy_File_System::PathsFromFilter( "Environments/", "*.bsp" );
    GLToy_ConstIterate( GLToy_String, szPath, xBSPPaths )
        GLToy_String szName = szPath;
        szName.RemoveAt( 0, 13 ); // remove "Environments/"
        
        GLToy_DebugOutput( "   - Found environment \"%S\".\r\n", szName.GetWideString() );

        s_xEnvironments.AddNode( new GLToy_EnvironmentFile( szPath ), szName.GetHash() );
    GLToy_Iterate_End;

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

void GLToy_Environment_System::Render2D()
{
    if( s_pxCurrentEnvironment && s_bRender && s_bDebugRender )
    {
        s_pxCurrentEnvironment->Render2D();
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
            GLToy_UI_System::ShowErrorDialog( "Unable to load environment \"%S\", the file for it could not be found", szName.GetWideString() );
            return;
        }

        bNative = false;
    }

    GLToy_Entity_System::DestroyEntities();
    GLToy_Physics_System::Reset();

    ( *ppxEnvFile )->LoadEnvironment();

    // don't do this for .bsp or we stomp the entities in them
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
    SwitchEnvironment( new GLToy_Environment_Plane( GLToy_Plane( GLToy_Vector_3( 0.0f, 1.0f, 0.0f ), 0.0f ), "generic/grid1.ptx" ) );
}

GLToy_Environment* GLToy_Environment_System::CreateEnvironmentFromType( const u_int uType )
{
    GLToy_Environment* pxNewEnv = NULL;

    if( s_pfnProject_CreateFromType )
    {
        pxNewEnv = s_pfnProject_CreateFromType( uType );
    }

    if( !pxNewEnv )
    {
        const GLToy_EnvironmentType eType = static_cast< GLToy_EnvironmentType >( uType );
        switch( eType )
        {
            case ENV_PLANE:         pxNewEnv = new GLToy_Environment_Plane( GLToy_Plane( GLToy_Vector_3( 0.0f, 1.0f, 0.0f ), 0.0f ), "generic/grid1.ptx" ); break;
            case ENV_LIGHTMAPPED:   pxNewEnv = new GLToy_Environment_Lightmapped(); break;

            default:
            {
                GLToy_Assert( false, "Unrecognised environment type: %u", eType );
                break;
            }
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
