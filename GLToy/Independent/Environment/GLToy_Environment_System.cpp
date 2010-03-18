/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Environment/GLToy_Environment_System.h>

// GLToy
#include <Core/Console/GLToy_Console.h>
#include <Core/Data Structures/GLToy_HashTree.h>
#include <Environment/GLToy_Environment.h>
#include <File/GLToy_EnvironmentFile.h>
#include <File/GLToy_File_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Environment* GLToy_Environment_System::s_pxCurrentEnvironment = NULL;
bool GLToy_Environment_System::s_bRender = true;

GLToy_HashTree< GLToy_EnvironmentFile* > GLToy_Environment_System::s_xEnvironments;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Environment_System::Initialise()
{
    s_pxCurrentEnvironment = NULL;

    GLToy_Console::RegisterVariable( "render.env", &s_bRender );
    // GLToy_Console::RegisterCommand( "testenv", LoadTestEnvironment );
    
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
    s_xEnvironments.Clear();
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

void GLToy_Environment_System::LoadTestEnvironment()
{
}