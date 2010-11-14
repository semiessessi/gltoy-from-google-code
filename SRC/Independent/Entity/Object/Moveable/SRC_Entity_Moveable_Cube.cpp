/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Entity/Object/Moveable/SRC_Entity_Moveable_Cube.h>

// GLToy
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture.h>

// GLToy
#include <Physics/GLToy_Physics_Object.h>
#include <Physics/GLToy_Physics_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

SRC_Entity_Moveable_Cube::SRC_Entity_Moveable_Cube( const GLToy_Hash uHash, const u_int uType )
: GLToy_Parent( uHash, uType )
{
}

void SRC_Entity_Moveable_Cube::Spawn( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xVelocity )
{
    SetPhysicsObject( GLToy_Physics_System::CreatePhysicsBox( GetHash(), GLToy_AABB( xPosition, 8.0f, 8.0f, 8.0f ), xVelocity ) );
}

void SRC_Entity_Moveable_Cube::Render() const
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
