/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Environment/GLToy_Environment_System.h>

// GLToy
#include <Environment/GLToy_Environment.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Environment* GLToy_Environment_System::s_pxCurrentEnvironment = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Environment_System::Initialise()
{
    s_pxCurrentEnvironment = NULL;
    return true;
}

void GLToy_Environment_System::Shutdown()
{

}

void GLToy_Environment_System::Render()
{
    if( s_pxCurrentEnvironment )
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

GLToy_Environment* GLToy_Environment_System::CreateFromOBJFile( const char* const szFilename )
{
    GLToy_EnvironmentLeaf* pxLeaf = new GLToy_EnvironmentLeaf();
    GLToy_Model::LoadFromOBJFile( pxLeaf, szFilename );

    GLToy_Environment* pxEnvironment = new GLToy_Environment();
    pxEnvironment->AddLeaf( GLToy_Vector_3( 0.0f, 0.0f, 0.0f ), pxLeaf );
    
    return pxEnvironment;
}