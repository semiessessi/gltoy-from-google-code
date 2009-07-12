/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/TestCube.h>

// This file's header
#include <Render/TestCube_Render.h>

// GLToy
#include <Maths/GLToy_Vector.h>
#include <Render/GLToy_Render.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void TestCube_Render::Platform_Render()
{
    static float fTimer = 0.0f;
    
    GLToy_Render::Translate( GLToy_Vector_3( 0.0f, 0.0f, -3.0f ) );
    GLToy_Render::Rotate( GLToy_Vector_3( 0.3f, 0.05f, -0.7f ), fTimer );
    GLToy_Render::Rotate( GLToy_Vector_3( -0.7f, -0.05f, -0.3f ), 0.75f * fTimer );
    GLToy_Render::Rotate( GLToy_Vector_3( 0.0f, 1.0f, 0.0f ), 0.25f * fTimer );

	GLToy_Render::StartSubmittingQuads();

    GLToy_Render::SubmitColour( GLToy_Vector_3( 1.0f, 0.0f, 0.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( 1.0f, 1.0f, 1.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( 1.0f, -1.0f, 1.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( -1.0f, -1.0f, 1.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( -1.0f, 1.0f, 1.0f ) );

    GLToy_Render::SubmitColour( GLToy_Vector_3( 0.0f, 1.0f, 0.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( 1.0f, 1.0f, -1.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( 1.0f, -1.0f, -1.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( -1.0f, -1.0f, -1.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( -1.0f, 1.0f, -1.0f ) );

    GLToy_Render::SubmitColour( GLToy_Vector_3( 0.0f, 0.0f, 1.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( 1.0f, 1.0f, 1.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( 1.0f, 1.0f, -1.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( -1.0f, 1.0f, -1.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( -1.0f, 1.0f, 1.0f ) );

    GLToy_Render::SubmitColour( GLToy_Vector_3( 0.0f, 1.0f, 1.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( 1.0f, -1.0f, 1.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( 1.0f, -1.0f, -1.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( -1.0f, -1.0f, -1.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( -1.0f, -1.0f, 1.0f ) );

    GLToy_Render::SubmitColour( GLToy_Vector_3( 1.0f, 0.0f, 1.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( 1.0f, 1.0f, 1.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( 1.0f, 1.0f, -1.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( 1.0f, -1.0f, -1.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( 1.0f, -1.0f, 1.0f ) );

    GLToy_Render::SubmitColour( GLToy_Vector_3( 1.0f, 1.0f, 0.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( -1.0f, 1.0f, 1.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( -1.0f, 1.0f, -1.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( -1.0f, -1.0f, -1.0f ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( -1.0f, -1.0f, 1.0f ) );

    GLToy_Render::EndSubmit();

    fTimer += 0.3f;
}