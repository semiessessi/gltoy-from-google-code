/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Maths/GLToy_Volume.h>

// GLToy
#include <Core/Data Structures/GLToy_BitStream.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_AABB::Render() const
{
    GLToy_Texture_System::BindWhite();

    // draw two squares, then the remaining 4 lines between them
    GLToy_Render::StartSubmittingLineLoop();

    GLToy_Render::SubmitColour( GLToy_Vector_3( 1.0f, 1.0f, 1.0f ) );

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

    GLToy_Render::SubmitColour( GLToy_Vector_3( 1.0f, 1.0f, 1.0f ) );

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

    GLToy_Render::SubmitColour( GLToy_Vector_3( 1.0f, 1.0f, 1.0f ) );

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

void GLToy_OBB::Render() const
{
    GLToy_Render::PushViewMatrix();

    GLToy_Render::Translate( GetPosition() );

    GLToy_Matrix_3 xInverseOrientation = m_xOrientation;
    xInverseOrientation.Transpose();

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
    // TODO
    return false;
}

bool GLToy_OBB::IntersectsWithAABB( const GLToy_AABB& xAABB ) const
{
    // TODO
    return false;
}

GLToy_Sphere GLToy_Volume::GetBoundingSphere() const
{
    return GLToy_Sphere();
}

GLToy_AABB GLToy_Volume::GetBoundingAABB() const
{
    return GLToy_AABB();
}

GLToy_Sphere GLToy_AABB::GetBoundingSphere() const
{
    return GLToy_Sphere( m_xPosition, GLToy_Maths::Sqrt( GetHalfExtents() * GetHalfExtents() ) );
}

void GLToy_Volume::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    xStream >> m_xPosition;
}

void GLToy_Volume::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    xStream << m_xPosition;
}

void GLToy_AABB::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    GLToy_Parent::ReadFromBitStream( xStream );

    xStream >> m_xPointMax;
    xStream >> m_xPointMin;
}

void GLToy_AABB::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    GLToy_Parent::WriteToBitStream( xStream );

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
    // xStream >> m_xOrientation; // TODO
}

void GLToy_OBB::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    GLToy_Parent::WriteToBitStream( xStream );

    xStream << m_xBox;
    // xStream << m_xOrientation; // TODO
}
