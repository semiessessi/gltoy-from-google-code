/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
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
