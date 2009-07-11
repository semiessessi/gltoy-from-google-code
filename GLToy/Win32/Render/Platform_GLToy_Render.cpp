/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's headers
#include <Render/GLToy_Render.h>
#include <Render/Platform_GLToy_Render.h>

// Win32
#include <windows.h>

// GL
#include <gl/gl.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool Platform_GLToy_Render::Initialise()
{
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClearDepth( 1.0f );
    glClearStencil( 0 );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );

	return true;
}

void Platform_GLToy_Render::Shutdown()
{

}

void Platform_GLToy_Render::BeginRender()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

void Platform_GLToy_Render::EndRender()
{
    glFlush();
    GLToy::UpdateBuffers();
}

/////////////////////////////////////////////////////////////////////////////////////////////
// G L   I N T E R F A C E
/////////////////////////////////////////////////////////////////////////////////////////////

void Platform_GLToy_Render::StartSubmittingTriangles()
{
	glBegin( GL_TRIANGLES );
}

void Platform_GLToy_Render::StartSubmittingQuads()
{
	glBegin( GL_QUADS );
}

void Platform_GLToy_Render::StartSubmittingTriangleStrip()
{
	glBegin( GL_TRIANGLE_STRIP );
}

void Platform_GLToy_Render::StartSubmittingTriangleFan()
{
	glBegin( GL_TRIANGLE_FAN );
}

void Platform_GLToy_Render::StartSubmittingPolygon()
{
	glBegin( GL_POLYGON );
}

void Platform_GLToy_Render::EndSubmit()
{
	glEnd();
}

void Platform_GLToy_Render::SubmitVertex3( const float* const pfVertex )
{
	glVertex3fv( pfVertex );
}

void Platform_GLToy_Render::SubmitNormal3( const float* const pfNormal )
{
	glNormal3fv( pfNormal );
}

void Platform_GLToy_Render::SubmitColour3( const float* const pfColour )
{
	glColor3fv( pfColour );
}

void Platform_GLToy_Render::SubmitTextureCoordinate3( const float* const pfTexCoord, const u_int uTextureUnit )
{
	// ignore texture unit for now...
	glTexCoord3fv( pfTexCoord );
}