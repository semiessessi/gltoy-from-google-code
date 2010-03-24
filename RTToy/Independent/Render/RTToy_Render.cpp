/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/RTToy.h>

// This file's header
#include <Render/RTToy_Render.h>

// GLToy
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_Sprite.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool RTToy_Render::Initialise()
{
    return true;
}

void RTToy_Render::Shutdown()
{
}

void RTToy_Render::Render()
{
    static GLToy_Sprite xSprite;

    xSprite.SetPosition( GLToy_Camera::GetPosition() + GLToy_Camera::GetDirection() * 4.0f );
    xSprite.SetSize( 3.0f );
    u_int u = 1337;
    xSprite.SetTexture( _GLToy_GetHash( reinterpret_cast< const char* const >( &( u ) ), 4 ) );
    // xSprite.Render();
}