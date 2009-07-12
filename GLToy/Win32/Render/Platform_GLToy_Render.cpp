/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's headers
#include <Render/GLToy_Render.h>
#include <Render/Platform_GLToy_Render.h>

// GLToy
#include <Maths/GLToy_Vector.h>

// Win32
#include <windows.h>

// GL
#include <gl/gl.h>
#include <gl/glu.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool Platform_GLToy_Render::Initialise()
{
    GLToy_DebugOutput( "\r\nPlatform_GLToy_Render::Initialise() - Initialising Graphics\r\n" );

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
}

void Platform_GLToy_Render::EndRender()
{
    GLToy::UpdateBuffers();
}

/////////////////////////////////////////////////////////////////////////////////////////////
// G L   I N T E R F A C E
/////////////////////////////////////////////////////////////////////////////////////////////

void Platform_GLToy_Render::SetViewport( int iX, int iY, u_int uWidth, u_int uHeight )
{
    glViewport( iX, iY, uWidth, uHeight );
}

void Platform_GLToy_Render::SetIdentityProjectionMatrix()
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
}

void Platform_GLToy_Render::SetPerspectiveProjectionMatrix( float fFOV, u_int uViewportWidth, u_int uViewportHeight )
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( fFOV, static_cast<float>( uViewportWidth ) / static_cast<float>( uViewportHeight ), 0.01f, 10000.0f );
}

void Platform_GLToy_Render::SetOrthogonalProjectionMatrix()
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f );
}

void Platform_GLToy_Render::SetIdentityViewMatrix()
{
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

void Platform_GLToy_Render::SetLookAtViewMatrix( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xLookAt, const GLToy_Vector_3& xUp )
{
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt( xPosition[0], xPosition[1], xPosition[2],
                xLookAt[0], xLookAt[1], xLookAt[2],
                xUp[0], xUp[1], xUp[2] );
}

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

/////////////////////////////////////////////////////////////////////////////////////////////
// G L   I N T E R F A C E
/////////////////////////////////////////////////////////////////////////////////////////////

void Platform_GLToy_Render::Translate( const GLToy_Vector_3& xTranslation )
{
    glTranslatef( xTranslation[0], xTranslation[1], xTranslation[2] );
}

void Platform_GLToy_Render::Rotate( const GLToy_Vector_3& xAxis, const float fAngle )
{
    glRotatef( fAngle, xAxis[0], xAxis[1], xAxis[2] );
}

void Platform_GLToy_Render::SubmitVertex( const GLToy_Vector_3& xVertex )
{
    glVertex3fv( xVertex.GetConstFloatPointer() );
}

void Platform_GLToy_Render::SubmitNormal( const GLToy_Vector_3& xNormal )
{
    glNormal3fv( xNormal.GetConstFloatPointer() );
}

void Platform_GLToy_Render::SubmitColour( const GLToy_Vector_3& xColour )
{
    glColor3fv( xColour.GetConstFloatPointer() );
}

void Platform_GLToy_Render::SubmitTextureCoordinate( const GLToy_Vector_3& xTexCoord, const u_int uTextureUnit )
{
    // ### - ignore texture unit for now...
    glTexCoord3fv( xTexCoord.GetConstFloatPointer() );
}

void Platform_GLToy_Render::Flush()
{
    glFlush();
}