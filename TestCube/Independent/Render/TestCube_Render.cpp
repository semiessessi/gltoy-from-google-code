/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009, 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of TestCube.
//
// TestCube is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// TestCube is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with TestCube.
// If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/TestCube.h>

// This file's header
#include <Render/TestCube_Render.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Maths/GLToy_Vector.h>
#include <Model/GLToy_Model_Coloured.h>
#include <Render/GLToy_Camera.h>
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
    GLToy_Camera::SetPosition( GLToy_Vector_3( 0.0f, 0.0f, -3.0f ) );

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

void TestCube_Render::Shutdown()
{
}

void TestCube_Render::Render()
{
    const float fTimer = 30.0f * GLToy_Timer::GetTime();
    
    GLToy_Render::Rotate( GLToy_Vector_3( 0.3f, 0.05f, -0.7f ), fTimer );
    GLToy_Render::Rotate( GLToy_Vector_3( -0.7f, -0.05f, -0.3f ), 0.75f * fTimer );
    GLToy_Render::Rotate( GLToy_Vector_3( 0.0f, 1.0f, 0.0f ), 0.25f * fTimer );

    s_xCube.Render();
}
