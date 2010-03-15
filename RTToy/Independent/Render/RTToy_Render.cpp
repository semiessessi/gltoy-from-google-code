/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/RTToy.h>

// This file's header
#include <Render/RTToy_Render.h>

// GLToy
#include <Model/GLToy_Model.h>
#include <Model/GLToy_Model_System.h>
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_Texture.h>

// RTToy
#include <Environment/RTToy_Environment_Plane.h>

static GLToy_Model* pxModel = NULL;
static RTToy_Environment_Plane* pxEnv = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool RTToy_Render::Initialise()
{
    GLToy_Texture_System::CreateTexture( "skins/loadtrucka.png" );
    
    pxEnv = new RTToy_Environment_Plane( GLToy_Plane( GLToy_Vector_3( 0.0f, 1.0f, 0.0f ), 0.0f ), "generic/grid1.png" );
    pxEnv->Initialise();
    
    return true;
}

void RTToy_Render::Shutdown()
{
    GLToy_Texture_System::DestroyTexture( "skins/loadtrucka.png" );
    pxEnv->Shutdown();
    delete pxEnv;
    pxEnv = NULL;
}

void RTToy_Render::Render()
{
    pxEnv->Render();

    pxModel = GLToy_Model_System::LoadModel( "loadtrucka" );
    GLToy_Texture_System::BindTexture( "skins/loadtrucka.png" );
    pxModel->Render();
}