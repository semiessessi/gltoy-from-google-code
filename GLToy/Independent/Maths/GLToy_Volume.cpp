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
#include <Maths/GLToy_Volume.h>

// GLToy
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const u_int uSPHERE_RENDER_POINTCOUNT = 400;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_AABB::Render( const GLToy_Vector_4 xColour ) const
{
    GLToy_Texture_System::BindWhite();

    // draw two squares, then the remaining 4 lines between them
    GLToy_Render::StartSubmittingLineLoop();

    GLToy_Render::SubmitColour( xColour );

    GLToy_Vector_3 xVertex = m_xPointMin;
    
    GLToy_Render::SubmitVertex( xVertex );
    xVertex[ 0 ] = m_xPointMax[ 0 ];
    GLToy_Render::SubmitVertex( xVertex );
    xVertex[ 1 ] = m_xPointMax[ 1 ];
    GLToy_Render::SubmitVertex( xVertex );
    xVertex[ 0 ] = m_xPointMin[ 0 ];
    GLToy_Render::SubmitVertex( xVertex );

    GLToy_Render::EndSubmit();

    GLToy_Render::StartSubmittingLineLoop();

    GLToy_Render::SubmitColour( xColour );

    xVertex[ 1 ] = m_xPointMin[ 1 ];
    xVertex[ 2 ] = m_xPointMax[ 2 ];

    GLToy_Render::SubmitVertex( xVertex );
    xVertex[ 0 ] = m_xPointMax[ 0 ];
    GLToy_Render::SubmitVertex( xVertex );
    xVertex[ 1 ] = m_xPointMax[ 1 ];
    GLToy_Render::SubmitVertex( xVertex );
    xVertex[ 0 ] = m_xPointMin[ 0 ];
    GLToy_Render::SubmitVertex( xVertex );

    GLToy_Render::EndSubmit();

    GLToy_Render::StartSubmittingLines();

    GLToy_Render::SubmitColour( xColour );

    xVertex[ 1 ] = m_xPointMin[ 1 ];
    xVertex[ 2 ] = m_xPointMin[ 2 ];
    GLToy_Render::SubmitVertex( xVertex );
    xVertex[ 2 ] = m_xPointMax[ 2 ];
    GLToy_Render::SubmitVertex( xVertex );

    xVertex[ 1 ] = m_xPointMax[ 1 ];
    xVertex[ 2 ] = m_xPointMin[ 2 ];
    GLToy_Render::SubmitVertex( xVertex );
    xVertex[ 2 ] = m_xPointMax[ 2 ];
    GLToy_Render::SubmitVertex( xVertex );

    xVertex[ 0 ] = m_xPointMax[ 0 ];
    xVertex[ 2 ] = m_xPointMin[ 2 ];
    GLToy_Render::SubmitVertex( xVertex );
    xVertex[ 2 ] = m_xPointMax[ 2 ];
    GLToy_Render::SubmitVertex( xVertex );

    xVertex[ 1 ] = m_xPointMin[ 1 ];
    xVertex[ 2 ] = m_xPointMin[ 2 ];
    GLToy_Render::SubmitVertex( xVertex );
    xVertex[ 2 ] = m_xPointMax[ 2 ];
    GLToy_Render::SubmitVertex( xVertex );

    GLToy_Render::EndSubmit();
}

void GLToy_AABB::RenderFlat() const
{
    GLToy_Render::StartSubmittingQuads();

    GLToy_Vector_3 xVertex;

    const bool bInside = IsInside( GLToy_Camera::GetPosition() );

    if( bInside || ( ( GLToy_Camera::GetPosition()[ 0 ] - m_xPointMin[ 0 ] ) > 0.0f ) )
    {
        // min x
        xVertex = m_xPointMin;
        GLToy_Render::SubmitVertex( xVertex );
        xVertex[ 1 ] = m_xPointMax[ 1 ];
        GLToy_Render::SubmitVertex( xVertex );
        xVertex[ 2 ] = m_xPointMax[ 2 ];
        GLToy_Render::SubmitVertex( xVertex );
        xVertex[ 1 ] = m_xPointMin[ 1 ];
        GLToy_Render::SubmitVertex( xVertex );
    }
    
    if( bInside || ( ( GLToy_Camera::GetPosition()[ 0 ] - m_xPointMax[ 0 ] ) <= 0.0f ) )
    {
        // max x
        xVertex = m_xPointMin;
        xVertex[ 0 ] = m_xPointMax[ 0 ];
        GLToy_Render::SubmitVertex( xVertex );
        xVertex[ 1 ] = m_xPointMax[ 1 ];
        GLToy_Render::SubmitVertex( xVertex );
        xVertex[ 2 ] = m_xPointMax[ 2 ];
        GLToy_Render::SubmitVertex( xVertex );
        xVertex[ 1 ] = m_xPointMin[ 1 ];
        GLToy_Render::SubmitVertex( xVertex );
    }

    if( bInside || ( ( GLToy_Camera::GetPosition()[ 1 ] - m_xPointMin[ 1 ] ) > 0.0f ) )
    {
        // min y
        xVertex = m_xPointMin;
        GLToy_Render::SubmitVertex( xVertex );
        xVertex[ 2 ] = m_xPointMax[ 2 ];
        GLToy_Render::SubmitVertex( xVertex );
        xVertex[ 0 ] = m_xPointMax[ 0 ];
        GLToy_Render::SubmitVertex( xVertex );
        xVertex[ 2 ] = m_xPointMin[ 2 ];
        GLToy_Render::SubmitVertex( xVertex );
    }

    if( bInside || ( ( GLToy_Camera::GetPosition()[ 1 ] - m_xPointMax[ 1 ] ) <= 0.0f ) )
    {
        // max y
        xVertex = m_xPointMin;
        xVertex[ 1 ] = m_xPointMax[ 1 ];
        GLToy_Render::SubmitVertex( xVertex );
        xVertex[ 2 ] = m_xPointMax[ 2 ];
        GLToy_Render::SubmitVertex( xVertex );
        xVertex[ 0 ] = m_xPointMax[ 0 ];
        GLToy_Render::SubmitVertex( xVertex );
        xVertex[ 2 ] = m_xPointMin[ 2 ];
        GLToy_Render::SubmitVertex( xVertex );
    }

    if( bInside || ( ( GLToy_Camera::GetPosition()[ 2 ] - m_xPointMin[ 2 ] ) > 0.0f ) )
    {
        // min z
        xVertex = m_xPointMin;
        GLToy_Render::SubmitVertex( xVertex );
        xVertex[ 0 ] = m_xPointMax[ 0 ];
        GLToy_Render::SubmitVertex( xVertex );
        xVertex[ 1 ] = m_xPointMax[ 1 ];
        GLToy_Render::SubmitVertex( xVertex );
        xVertex[ 0 ] = m_xPointMin[ 0 ];
        GLToy_Render::SubmitVertex( xVertex );
    }

    if( bInside || ( ( GLToy_Camera::GetPosition()[ 2 ] - m_xPointMax[ 2 ] ) <= 0.0f ) )
    {
        // max z
        xVertex = m_xPointMin;
        xVertex[ 2 ] = m_xPointMax[ 2 ];
        GLToy_Render::SubmitVertex( xVertex );
        xVertex[ 0 ] = m_xPointMax[ 0 ];
        GLToy_Render::SubmitVertex( xVertex );
        xVertex[ 1 ] = m_xPointMax[ 1 ];
        GLToy_Render::SubmitVertex( xVertex );
        xVertex[ 0 ] = m_xPointMin[ 0 ];
        GLToy_Render::SubmitVertex( xVertex );
    }

    GLToy_Render::EndSubmit();
}

void GLToy_Sphere::Render() const
{
    GLToy_Texture_System::BindWhite();

    // draw two squares, then the remaining 4 lines between them
    GLToy_Render::StartSubmittingLineStrip();

    GLToy_Render::SubmitColour( GLToy_Vector_3( 1.0f, 1.0f, 1.0f ) );

    for( u_int u = 0; u < uSPHERE_RENDER_POINTCOUNT; ++u )
    {
        const float fS = static_cast< float >( u );
        const float fT = static_cast< float >( u & 0xF );
        GLToy_Render::SubmitVertex(
            GLToy_Vector_3(
                GLToy_Maths::Sin( fS ) * GLToy_Maths::Sin( fT ),
                GLToy_Maths::Cos( fS ) * GLToy_Maths::Sin( fT ),
                GLToy_Maths::Cos( fT )
            )
        );
    }

    GLToy_Render::EndSubmit();
}

void GLToy_OBB::Render() const
{
    GLToy_Render::PushViewMatrix();

    GLToy_Render::Translate( GetPosition() );

    GLToy_Matrix_3 xInverseOrientation = m_xOrientation;
    xInverseOrientation.InvertTransformationMatrix();

    GLToy_Render::Transform( xInverseOrientation );

    GLToy_Render::Translate( -GetPosition() );
    
    m_xBox.Render();
    
    GLToy_Render::PopViewMatrix();
}

bool GLToy_AABB::IntersectsWithAABB( const GLToy_AABB& xAABB ) const
{
    return !( ( xAABB.m_xPointMax[ 0 ] < m_xPointMin[ 0 ] )
        || ( xAABB.m_xPointMin[ 0 ] > m_xPointMax[ 0 ] )
        || ( xAABB.m_xPointMax[ 1 ] < m_xPointMin[ 1 ] )
        || ( xAABB.m_xPointMin[ 1 ] > m_xPointMax[ 1 ] )
        || ( xAABB.m_xPointMax[ 2 ] < m_xPointMin[ 2 ] )
        || ( xAABB.m_xPointMin[ 2 ] > m_xPointMax[ 2 ] ) );
}

bool GLToy_Sphere::IntersectsWithAABB( const GLToy_AABB& xAABB ) const
{
    if( xAABB.IsInside( GetPosition() ) )
    {
        return true;
    }

    // TODO: ray tests with each of the AABB edges?

    return false;
}

bool GLToy_Sphere::IntersectsWithSphere( const GLToy_Sphere& xSphere ) const
{
    const float fTotalRadii = xSphere.GetRadius() + GetRadius();
    return ( GetPosition() - xSphere.GetPosition() ).MagnitudeSquared() < ( fTotalRadii * fTotalRadii );
}

bool GLToy_OBB::IntersectsWithAABB( const GLToy_AABB& xAABB ) const
{
    // TODO
    return false;
}

GLToy_Sphere GLToy_AABB::GetBoundingSphere() const
{
    return GLToy_Sphere( m_xPosition, GLToy_Maths::Sqrt( GetHalfExtents() * GetHalfExtents() ) );
}

void GLToy_AABB::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    xStream >> m_xPointMax;
    xStream >> m_xPointMin;
    m_xPosition = ( m_xPointMax + m_xPointMin ) * 0.5f;
}

void GLToy_AABB::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    xStream << m_xPointMax;
    xStream << m_xPointMin;
}

void GLToy_Sphere::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    GLToy_Parent::ReadFromBitStream( xStream );

    xStream >> m_fRadius;
}

void GLToy_Sphere::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    GLToy_Parent::WriteToBitStream( xStream );

    xStream << m_fRadius;
}

void GLToy_OBB::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    GLToy_Parent::ReadFromBitStream( xStream );

    xStream >> m_xBox;
    xStream >> m_xOrientation;
}

void GLToy_OBB::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    GLToy_Parent::WriteToBitStream( xStream );

    xStream << m_xBox;
    xStream << m_xOrientation;
}

GLToy_Plane GLToy_AABB::GetPlane( int iIndex ) const
{
	GLToy_Vector_3 xNormal;
	float fDistance = 0.0f;

	switch( iIndex )
	{
		case 0:
		{
			// Y = 1
			xNormal = GLToy_Vector_3( 0.0f, 1.0f, 0.0f );
			fDistance = -m_xPointMax[1];
		}
		break;

		case 1:
		{
			// X = -1
			xNormal = GLToy_Vector_3( -1.0f, 0.0f, 0.0f );
			fDistance = m_xPointMin[0];
		}
		break;

		case 2:
		{
			// Z = 1
			xNormal = GLToy_Vector_3( 0.0f, 0.0f, 1.0f );
			fDistance = -m_xPointMax[2];
		}
		break;

		case 3:
		{
			// X = 1
			xNormal = GLToy_Vector_3( 1.0f, 0.0f, 0.0f );
			fDistance = -m_xPointMax[0];
		}
		break;

		case 4:
		{
			// Z = -1
			xNormal = GLToy_Vector_3( 0.0f, 0.0f, -1.0f );
			fDistance = m_xPointMin[2];
		}
		break;

		case 5:
		{
			// Y = -1
			xNormal = GLToy_Vector_3( 0.0f, -1.0f, 0.0f );
			fDistance = m_xPointMin[1];
		}
		break;

		default:
		{
			GLToy_Assert( false, "Invalid index" );
		}
	}

	return GLToy_Plane( xNormal, fDistance );
}
