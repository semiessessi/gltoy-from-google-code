/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010, 2011 Semi Essessi
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

#include <Core/GLToy.h>

// This file's header
#include <Entity/Object/Moveable/SRC_Entity_Moveable_Square.h>

// GLToy
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture_System.h>

// GLToy
#include <Physics/GLToy_Physics_Object.h>
#include <Physics/GLToy_Physics_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

SRC_Entity_Moveable_Square::SRC_Entity_Moveable_Square( const GLToy_Hash uHash, const u_int uType )
: GLToy_Parent( uHash, uType )
{
}

void SRC_Entity_Moveable_Square::Spawn( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xVelocity )
{
    SetPhysicsObject( GLToy_Physics_System::CreatePhysicsBox( GetHash(), GLToy_AABB( xPosition, 12.0f, 12.0f, 0.5f ), xVelocity ) );
}

void SRC_Entity_Moveable_Square::Render() const
{
    const GLToy_Physics_Object* const pxPhysicsObject = GetPhysicsObject();
    if( pxPhysicsObject )
    {
        // get the obb and render it
        const GLToy_OBB xOBB = pxPhysicsObject->GetOBB();
        const GLToy_Vector_3& xMax = xOBB.GetUnrotatedBB().GetMax();
        const GLToy_Vector_3& xMin = xOBB.GetUnrotatedBB().GetMin();

        GLToy_Render::DisableBlending();

        GLToy_Render::PushViewMatrix();

        GLToy_Render::Translate( xOBB.GetPosition() );

        GLToy_Matrix_3 xInverseOrientation = xOBB.GetOrientation();
        xInverseOrientation.Transpose();

        GLToy_Render::Transform( xInverseOrientation );

        GLToy_Render::Translate( -xOBB.GetPosition() );
        
        GLToy_Texture_System::BindTexture( "Generic/Grid2.png" );

        GLToy_Render::StartSubmittingQuads();

        GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );

        GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 1.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], xMax[ 1 ], xMax[ 2 ] ) );
        GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 0.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], xMin[ 1 ], xMax[ 2 ] ) );
        GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 0.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], xMin[ 1 ], xMax[ 2 ] ) );
        GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 1.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], xMax[ 1 ], xMax[ 2 ] ) );

        GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 1.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], xMax[ 1 ], xMin[ 2 ] ) );
        GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 0.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], xMin[ 1 ], xMin[ 2 ] ) );
        GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 0.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], xMin[ 1 ], xMin[ 2 ] ) );
        GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 1.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], xMax[ 1 ], xMin[ 2 ] ) );

        GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 1.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], xMax[ 1 ], xMax[ 2 ] ) );
        GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 0.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], xMax[ 1 ], xMin[ 2 ] ) );
        GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 0.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], xMax[ 1 ], xMin[ 2 ] ) );
        GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 1.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], xMax[ 1 ], xMax[ 2 ] ) );

        GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 1.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], xMin[ 1 ], xMax[ 2 ] ) );
        GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 0.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], xMin[ 1 ], xMin[ 2 ] ) );
        GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 0.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], xMin[ 1 ], xMin[ 2 ] ) );
        GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 1.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], xMin[ 1 ], xMax[ 2 ] ) );

        GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 1.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], xMax[ 1 ], xMax[ 2 ] ) );
        GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 0.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], xMax[ 1 ], xMin[ 2 ] ) );
        GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 0.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], xMin[ 1 ], xMin[ 2 ] ) );
        GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 1.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( xMax[ 0 ], xMin[ 1 ], xMax[ 2 ] ) );

        GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 1.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], xMax[ 1 ], xMax[ 2 ] ) );
        GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 0.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], xMax[ 1 ], xMin[ 2 ] ) );
        GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 0.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], xMin[ 1 ], xMin[ 2 ] ) );
        GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 1.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( xMin[ 0 ], xMin[ 1 ], xMax[ 2 ] ) );

        GLToy_Render::EndSubmit();
        
        GLToy_Render::PopViewMatrix();
    }
}
