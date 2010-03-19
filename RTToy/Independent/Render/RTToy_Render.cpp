/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/RTToy.h>

// This file's header
#include <Render/RTToy_Render.h>

// GLToy
#include <Render/GLToy_Sprite.h>
#include <String/GLToy_String.h>

const u_int uNUM_SPRITES = 10;
static GLToy_Sprite* pxSprite[ uNUM_SPRITES ];

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool RTToy_Render::Initialise()
{
    for( u_int u = 0; u < uNUM_SPRITES; ++u )
    {
        pxSprite[ u ] = new GLToy_Sprite();
        pxSprite[ u ]->SetTexture( "sprites/star.png" );
        pxSprite[ u ]->SetSize( 10.0f );
        pxSprite[ u ]->SetPosition( GLToy_Vector_3( 0.0f, 25.0f, 0.0f ) + GLToy_Vector_3( 20.0f, 0.0f, 20.0f ) * ( static_cast< float >( u ) - 2.0f ) );
    }

    return true;
}

void RTToy_Render::Shutdown()
{
    for( u_int u = 0; u < uNUM_SPRITES; ++u )
    {
        pxSprite[ u ]->Shutdown();
        delete pxSprite[ u ];
        pxSprite[ u ] = NULL;
    }
}

void RTToy_Render::Render()
{
    for( u_int u = 0; u < uNUM_SPRITES; ++u )
    {
        pxSprite[ u ]->Render();
    }
}