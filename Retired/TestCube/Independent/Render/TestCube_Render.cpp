/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009, 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
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
