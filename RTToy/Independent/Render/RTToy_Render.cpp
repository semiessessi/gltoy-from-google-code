/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/RTToy.h>

// This file's header
#include <Render/RTToy_Render.h>

// GLToy
#include <Environment/GLToy_Environment_Plane.h>

//static GLToy_Environment_Plane* pxEnv = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool RTToy_Render::Initialise()
{
    //pxEnv = new GLToy_Environment_Plane( GLToy_Plane( GLToy_Vector_3( 0.0f, 1.0f, 0.0f ), 0.0f ), "generic/grid1.png" );
    //pxEnv->Initialise();

    return true;
}

void RTToy_Render::Shutdown()
{
    //pxEnv->Shutdown();
    //delete pxEnv;
    //pxEnv = NULL;
}

void RTToy_Render::Render()
{
    //pxEnv->Render();
}