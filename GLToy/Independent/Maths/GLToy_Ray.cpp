/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Maths/GLToy_Ray.h>

// GLToy
#include <Core/GLToy_Bounded.h>
#include <Maths/GLToy_Plane.h>
#include <Maths/GLToy_Volume.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Ray::IntersectsWithPlane( const GLToy_Plane& xPlane, GLToy_Vector_3* const pxPosition, GLToy_Vector_3* const pxNormal ) const
{
    const float fTop = xPlane.GetDistance() + xPlane.GetNormal() * m_xPosition;
    const float fBottom = xPlane.GetNormal() * m_xDirection;

    // this answers "is fTop / fBottom negative?", which is the same as if we hit, and should be cheaper than a division
    const bool bHit = ( ( fTop < 0.0f ) ^ ( fBottom < 0.0f ) );
    
    if( pxPosition )
    {
        const float fT = -fTop / fBottom;
        *pxPosition = m_xPosition + m_xDirection * fT;
    }

    if( pxNormal )
    {
        *pxNormal = xPlane.GetNormal();
    }

    return bHit;
}

bool GLToy_Ray::IntersectsWithAABB( const GLToy_AABB& xAABB, GLToy_Vector_3* const pxPosition, GLToy_Vector_3* const pxNormal ) const
{
    if( xAABB.IsInside( m_xPosition ) )
    {
        return false;
    }

    return false;
}

bool GLToy_Ray::IntersectsWithSphere( const GLToy_Sphere& xSphere, GLToy_Vector_3* const pxPosition, GLToy_Vector_3* const pxNormal ) const
{
    if( xSphere.IsInside( m_xPosition ) )
    {
        return false;
    }

    return false;
}

bool GLToy_Ray::IntersectsWithOBB( const GLToy_OBB& xOBB, GLToy_Vector_3* const pxPosition, GLToy_Vector_3* const pxNormal ) const
{
    return false;
}

bool GLToy_Ray::IntersectsWith( const GLToy_Bounded& xBounded, GLToy_Vector_3* const pxPosition, GLToy_Vector_3* const pxNormal ) const
{
    return xBounded.IntersectWithRay( *this, pxPosition, pxNormal );
}
