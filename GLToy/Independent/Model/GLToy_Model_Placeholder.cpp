/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Model/GLToy_Model_Placeholder.h>

// GLToy
#include <Render/GLToy_Render.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Model_Placeholder::GLToy_Model_Placeholder()
: GLToy_Parent()
{
    AddStripFromQuad(   GLToy_Vector_3( 10.0f, 10.0f, 10.0f ),
                        GLToy_Vector_3( 10.0f, -10.0f, 10.0f ),
                        GLToy_Vector_3( -10.0f, 10.0f, 10.0f ),
                        GLToy_Vector_3( -10.0f, -10.0f, 10.0f ),
                        GLToy_Vector_3( 1.0f, 0.0f, 0.0f )
                        );

    AddStripFromQuad(   GLToy_Vector_3( 10.0f, 10.0f, -10.0f ),
                        GLToy_Vector_3( 10.0f, -10.0f, -10.0f ),
                        GLToy_Vector_3( -10.0f, 10.0f, -10.0f ),
                        GLToy_Vector_3( -10.0f, -10.0f, -10.0f ),
                        GLToy_Vector_3( 0.0f, 1.0f, 0.0f )
                        );

    AddStripFromQuad(   GLToy_Vector_3( 10.0f, 10.0f, 10.0f ),
                        GLToy_Vector_3( 10.0f, 10.0f, -10.0f ),
                        GLToy_Vector_3( -10.0f, 10.0f, 10.0f ),
                        GLToy_Vector_3( -10.0f, 10.0f, -10.0f ),
                        GLToy_Vector_3( 0.0f, 0.0f, 1.0f )
                        );
    
    AddStripFromQuad(   GLToy_Vector_3( 10.0f, -10.0f, 10.0f ),
                        GLToy_Vector_3( 10.0f, -10.0f, -10.0f ),
                        GLToy_Vector_3( -10.0f, -10.0f, 10.0f ),
                        GLToy_Vector_3( -10.0f, -10.0f, -10.0f ),
                        GLToy_Vector_3( 0.0f, 1.0f, 1.0f )
                        );

    AddStripFromQuad(   GLToy_Vector_3( 10.0f, 10.0f, 10.0f ),
                        GLToy_Vector_3( 10.0f, 10.0f, -10.0f ),
                        GLToy_Vector_3( 10.0f, -10.0f, 10.0f ),
                        GLToy_Vector_3( 10.0f, -10.0f, -10.0f ),
                        GLToy_Vector_3( 1.0f, 0.0f, 1.0f )
                        );

    AddStripFromQuad(   GLToy_Vector_3( -10.0f, 10.0f, 10.0f ),
                        GLToy_Vector_3( -10.0f, 10.0f, -10.0f ),
                        GLToy_Vector_3( -10.0f, -10.0f, 10.0f ),
                        GLToy_Vector_3( -10.0f, -10.0f, -10.0f ),
                        GLToy_Vector_3( 1.0f, 1.0f, 0.0f )
                        );
}

void GLToy_Model_Placeholder::Render() const
{
	GLToy_Parent::Render();

	GLToy_Render::StartSubmittingLines();
	
	GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );

	GLToy_Render::SubmitVertex( GLToy_Vector_3( 0.0f, 0.0f, 10.0f ) );
	GLToy_Render::SubmitVertex( GLToy_Vector_3( 0.0f, 0.0f, 25.0f ) );

	GLToy_Render::SubmitVertex( GLToy_Vector_3( 2.5f, 0.0f, 22.5f ) );
	GLToy_Render::SubmitVertex( GLToy_Vector_3( 0.0f, 0.0f, 25.0f ) );

	GLToy_Render::SubmitVertex( GLToy_Vector_3( -2.5f, 0.0f, 22.5f ) );
	GLToy_Render::SubmitVertex( GLToy_Vector_3( 0.0f, 0.0f, 25.0f ) );

	GLToy_Render::SubmitVertex( GLToy_Vector_3( 0.0f, 2.5f, 22.5f ) );
	GLToy_Render::SubmitVertex( GLToy_Vector_3( 0.0f, 0.0f, 25.0f ) );

	GLToy_Render::SubmitVertex( GLToy_Vector_3( 0.0f, -2.5f, 22.5f ) );
	GLToy_Render::SubmitVertex( GLToy_Vector_3( 0.0f, 0.0f, 25.0f ) );

	GLToy_Render::EndSubmit();
}