/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Render/TestCube_Render.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Maths/GLToy_Vector.h>
#include <Render/GLToy_Render.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Model_Coloured TestCube_Render::s_xCube;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool TestCube_Render::Initialise()
{
	s_xCube.AddStripFromQuad(	GLToy_Vector_3( 1.0f, 1.0f, 1.0f ),
								GLToy_Vector_3( 1.0f, -1.0f, 1.0f ),
								GLToy_Vector_3( -1.0f, 1.0f, 1.0f ),
								GLToy_Vector_3( -1.0f, -1.0f, 1.0f ),
								GLToy_Vector_3( 1.0f, 0.0f, 0.0f )
								);

	s_xCube.AddStripFromQuad(	GLToy_Vector_3( 1.0f, 1.0f, -1.0f ),
								GLToy_Vector_3( 1.0f, -1.0f, -1.0f ),
								GLToy_Vector_3( -1.0f, 1.0f, -1.0f ),
								GLToy_Vector_3( -1.0f, -1.0f, -1.0f ),
								GLToy_Vector_3( 0.0f, 1.0f, 0.0f )
								);

    s_xCube.AddStripFromQuad(	GLToy_Vector_3( 1.0f, 1.0f, 1.0f ),
								GLToy_Vector_3( 1.0f, 1.0f, -1.0f ),
								GLToy_Vector_3( -1.0f, 1.0f, 1.0f ),
								GLToy_Vector_3( -1.0f, 1.0f, -1.0f ),
								GLToy_Vector_3( 0.0f, 0.0f, 1.0f )
								);
    
    s_xCube.AddStripFromQuad(	GLToy_Vector_3( 1.0f, -1.0f, 1.0f ),
								GLToy_Vector_3( 1.0f, -1.0f, -1.0f ),
								GLToy_Vector_3( -1.0f, -1.0f, 1.0f ),
								GLToy_Vector_3( -1.0f, -1.0f, -1.0f ),
								GLToy_Vector_3( 0.0f, 1.0f, 1.0f )
								);

    s_xCube.AddStripFromQuad(	GLToy_Vector_3( 1.0f, 1.0f, 1.0f ),
								GLToy_Vector_3( 1.0f, 1.0f, -1.0f ),
								GLToy_Vector_3( 1.0f, -1.0f, 1.0f ),
								GLToy_Vector_3( 1.0f, -1.0f, -1.0f ),
								GLToy_Vector_3( 1.0f, 0.0f, 1.0f )
								);

    s_xCube.AddStripFromQuad(	GLToy_Vector_3( -1.0f, 1.0f, 1.0f ),
								GLToy_Vector_3( -1.0f, 1.0f, -1.0f ),
								GLToy_Vector_3( -1.0f, -1.0f, 1.0f ),
								GLToy_Vector_3( -1.0f, -1.0f, -1.0f ),
								GLToy_Vector_3( 1.0f, 1.0f, 0.0f )
								);

    return true;
}

void TestCube_Render::Shutdown()
{
}

void TestCube_Render::Render()
{
	const float fTimer = 30.0f * GLToy_Timer::GetTime();
    
    GLToy_Render::Translate( GLToy_Vector_3( 0.0f, 0.0f, -3.0f ) );
    GLToy_Render::Rotate( GLToy_Vector_3( 0.3f, 0.05f, -0.7f ), fTimer );
    GLToy_Render::Rotate( GLToy_Vector_3( -0.7f, -0.05f, -0.3f ), 0.75f * fTimer );
    GLToy_Render::Rotate( GLToy_Vector_3( 0.0f, 1.0f, 0.0f ), 0.25f * fTimer );

	s_xCube.Render();
}