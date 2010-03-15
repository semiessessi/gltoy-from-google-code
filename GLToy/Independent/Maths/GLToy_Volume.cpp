/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Maths/GLToy_Volume.h>

// GLToy
#include <Core/Data Structures/GLToy_BitStream.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Sphere GLToy_Volume::GetBoundingSphere() const
{
    return GLToy_Sphere();
}

GLToy_AABB GLToy_Volume::GetBoundingAABB() const
{
    return GLToy_AABB();
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
