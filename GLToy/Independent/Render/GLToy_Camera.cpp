/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Render/GLToy_Camera.h>

// GLToy
#include <Maths/GLToy_Vector.h>
#include <Render/GLToy_Render.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Vector_3 GLToy_Camera::s_xPosition    = GLToy_Vector_3( 0.0f, 0.0f, -200.0f );
GLToy_Vector_3 GLToy_Camera::s_xDirection   = GLToy_Vector_3( 0.0f, 0.0f, 1.0f );
GLToy_Vector_3 GLToy_Camera::s_xUp          = GLToy_Vector_3( 0.0f, 1.0f, 0.0f );

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Camera::Initialise()
{
    return true;
}

void GLToy_Camera::ApplyTransforms()
{
    GLToy_Render::SetPerspectiveProjectionMatrix( GLToy::GetWindowViewportWidth(), GLToy::GetWindowViewportHeight() );

    GLToy_Render::SetLookAtViewMatrix( s_xPosition, s_xPosition + s_xDirection, s_xUp );
}