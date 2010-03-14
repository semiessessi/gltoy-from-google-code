/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Render/RTToy_Render.h>

#include <Model/GLToy_Model.h>
#include <Model/GLToy_Model_System.h>
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_Texture.h>

static GLToy_Model* pxModel = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool RTToy_Render::Initialise()
{
    GLToy_Texture_System::CreateTexture( "skins/loadtrucka.png" );

    return true;
}

void RTToy_Render::Shutdown()
{
    GLToy_Texture_System::DestroyTexture( "skins/loadtrucka.png" );
    delete pxModel;
    pxModel = NULL;
}

void RTToy_Render::Render()
{
    if( !pxModel )
    {
        pxModel = GLToy_Model_System::LoadModel( "loadtrucka" );
    }

    GLToy_Texture_System::BindTexture( "skins/loadtrucka.png" );
    pxModel->Render();
}