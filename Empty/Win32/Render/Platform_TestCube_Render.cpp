/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/TestCube.h>

// This file's header
#include <Render/TestCube_Render.h>

// Win32
#include <windows.h>

// GL
#include <gl/gl.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void TestCube_Render::Platform_Render()
{
    static float fTimer = 0.0f;
    
    glTranslatef( 0.0f, 0.0f, -3.0f );
    glRotatef( fTimer, 0.3f, 0.05f, -0.7f );
    glRotatef( 0.75f * fTimer, -0.7f, -0.05f, -0.3f );
    glRotatef( 0.25f * fTimer, 0.0f, 1.0f, 0.0f );

    glBegin( GL_QUADS );

    glColor4f( 1.0f, 0.0f, 0.0f, 1.0f );
    glVertex3f( 1.0f, 1.0f, 1.0f );
    glVertex3f( 1.0f, -1.0f, 1.0f );
    glVertex3f( -1.0f, -1.0f, 1.0f );
    glVertex3f( -1.0f, 1.0f, 1.0f );

    glColor4f( 0.0f, 1.0f, 0.0f, 1.0f );
    glVertex3f( 1.0f, 1.0f, -1.0f );
    glVertex3f( 1.0f, -1.0f, -1.0f );
    glVertex3f( -1.0f, -1.0f, -1.0f );
    glVertex3f( -1.0f, 1.0f, -1.0f );

    glColor4f( 0.0f, 0.0f, 1.0f, 1.0f );
    glVertex3f( 1.0f, 1.0f, 1.0f );
    glVertex3f( 1.0f, 1.0f, -1.0f );
    glVertex3f( -1.0f, 1.0f, -1.0f );
    glVertex3f( -1.0f, 1.0f, 1.0f );

    glColor4f( 0.0f, 1.0f, 1.0f, 1.0f );
    glVertex3f( 1.0f, -1.0f, 1.0f );
    glVertex3f( 1.0f, -1.0f, -1.0f );
    glVertex3f( -1.0f, -1.0f, -1.0f );
    glVertex3f( -1.0f, -1.0f, 1.0f );

    glColor4f( 1.0f, 0.0f, 1.0f, 1.0f );
    glVertex3f( 1.0f, 1.0f, 1.0f );
    glVertex3f( 1.0f, 1.0f, -1.0f );
    glVertex3f( 1.0f, -1.0f, -1.0f );
    glVertex3f( 1.0f, -1.0f, 1.0f );

    glColor4f( 1.0f, 1.0f, 0.0f, 1.0f );
    glVertex3f( -1.0f, 1.0f, 1.0f );
    glVertex3f( -1.0f, 1.0f, -1.0f );
    glVertex3f( -1.0f, -1.0f, -1.0f );
    glVertex3f( -1.0f, -1.0f, 1.0f );

    glEnd();

    fTimer += 0.03f;
}