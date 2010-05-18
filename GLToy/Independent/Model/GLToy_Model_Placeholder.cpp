/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of GLToy.
//
// GLToy is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// GLToy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with GLToy.
// If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////////////////

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
