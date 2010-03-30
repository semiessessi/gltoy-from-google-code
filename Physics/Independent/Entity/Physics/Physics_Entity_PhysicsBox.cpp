/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of Physics.
//
// Physics is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// Physics is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with Physics.
// If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/Physics.h>

// This file's header
#include <Entity/Physics/Physics_Entity_PhysicsBox.h>

// GLToy
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture.h>

// Physics
#include <Physics/Physics_Physics_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

Physics_Entity_PhysicsBox::Physics_Entity_PhysicsBox( const GLToy_Hash uHash, const u_int uType )
: GLToy_Parent( uHash, uType )
, m_pxPhysicsObject( NULL )
{
}

void Physics_Entity_PhysicsBox::Spawn( const GLToy_AABB& xBB, const GLToy_Vector_3& xVelocity )
{
    m_pxPhysicsObject = Physics_Physics_System::CreatePhysicsBox( GetHash(), xBB, xVelocity );
}

void Physics_Entity_PhysicsBox::Render() const
{
    if( m_pxPhysicsObject )
    {
        // get the obb and render it
        const GLToy_OBB xOBB = m_pxPhysicsObject->GetOBB();
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