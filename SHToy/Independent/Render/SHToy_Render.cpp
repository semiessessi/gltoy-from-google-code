/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Render/SHToy_Render.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Maths/GLToy_Vector.h>
#include <Model/GLToy_Model_Coloured.h>
#include <Render/GLToy_Render.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Model_Coloured SHToy_Render::s_xCube;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool SHToy_Render::Initialise()
{
    s_xCube.AddStripFromQuad(   GLToy_Vector_3( 1.0f, 1.0f, 1.0f ),
                                GLToy_Vector_3( 1.0f, -1.0f, 1.0f ),
                                GLToy_Vector_3( -1.0f, 1.0f, 1.0f ),
                                GLToy_Vector_3( -1.0f, -1.0f, 1.0f ),
                                GLToy_Vector_3( 1.0f, 0.0f, 0.0f )
                                );

    s_xCube.AddStripFromQuad(   GLToy_Vector_3( 1.0f, 1.0f, -1.0f ),
                                GLToy_Vector_3( 1.0f, -1.0f, -1.0f ),
                                GLToy_Vector_3( -1.0f, 1.0f, -1.0f ),
                                GLToy_Vector_3( -1.0f, -1.0f, -1.0f ),
                                GLToy_Vector_3( 0.0f, 1.0f, 0.0f )
                                );

    s_xCube.AddStripFromQuad(   GLToy_Vector_3( 1.0f, 1.0f, 1.0f ),
                                GLToy_Vector_3( 1.0f, 1.0f, -1.0f ),
                                GLToy_Vector_3( -1.0f, 1.0f, 1.0f ),
                                GLToy_Vector_3( -1.0f, 1.0f, -1.0f ),
                                GLToy_Vector_3( 0.0f, 0.0f, 1.0f )
                                );
    
    s_xCube.AddStripFromQuad(   GLToy_Vector_3( 1.0f, -1.0f, 1.0f ),
                                GLToy_Vector_3( 1.0f, -1.0f, -1.0f ),
                                GLToy_Vector_3( -1.0f, -1.0f, 1.0f ),
                                GLToy_Vector_3( -1.0f, -1.0f, -1.0f ),
                                GLToy_Vector_3( 0.0f, 1.0f, 1.0f )
                                );

    s_xCube.AddStripFromQuad(   GLToy_Vector_3( 1.0f, 1.0f, 1.0f ),
                                GLToy_Vector_3( 1.0f, 1.0f, -1.0f ),
                                GLToy_Vector_3( 1.0f, -1.0f, 1.0f ),
                                GLToy_Vector_3( 1.0f, -1.0f, -1.0f ),
                                GLToy_Vector_3( 1.0f, 0.0f, 1.0f )
                                );

    s_xCube.AddStripFromQuad(   GLToy_Vector_3( -1.0f, 1.0f, 1.0f ),
                                GLToy_Vector_3( -1.0f, 1.0f, -1.0f ),
                                GLToy_Vector_3( -1.0f, -1.0f, 1.0f ),
                                GLToy_Vector_3( -1.0f, -1.0f, -1.0f ),
                                GLToy_Vector_3( 1.0f, 1.0f, 0.0f )
                                );

    return true;
}

void SHToy_Render::Shutdown()
{
}

void SHToy_Render::Render()
{
    const float fTimer = 30.0f * GLToy_Timer::GetTime();
    
    GLToy_Render::Rotate( GLToy_Vector_3( 0.3f, 0.05f, -0.7f ), fTimer );
    GLToy_Render::Rotate( GLToy_Vector_3( -0.7f, -0.05f, -0.3f ), 0.75f * fTimer );
    GLToy_Render::Rotate( GLToy_Vector_3( 0.0f, 1.0f, 0.0f ), 0.25f * fTimer );

    s_xCube.Render();
}