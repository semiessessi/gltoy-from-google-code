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

// TODO - finish
bool GLToy_Ray::IntersectsWithAABB( const GLToy_AABB& xAABB, GLToy_Vector_3* const pxPosition, GLToy_Vector_3* const pxNormal ) const
{
    if( xAABB.IsInside( m_xPosition ) )
    {
        return false;
    }

    // we can do a load of position/direction based early outs...
    //
    // ^         |
    // | - - - - o__       o - BB corner
    // |                   * - ray pos
    // y   *--->    we can see that if the pos is below the bb minimum in x we must
    //              have a positive y component to intersect

    const GLToy_Vector_3& xMin = xAABB.GetMin();
    const GLToy_Vector_3& xMax = xAABB.GetMax();

    for( u_int u = 0; u < 3; ++u )
    {
        if( m_xPosition[ u ] < xMin [ u ] && m_xDirection[ u ] < 0.0f )
        {
            return false;
        }
    }

    for( u_int u = 0; u < 3; ++u )
    {
        if( m_xPosition[ u ] > xMax [ u ] && m_xDirection[ u ] > 0.0f )
        {
            return false;
        }
    }

    return false;
}

bool GLToy_Ray::IntersectsWithSphere( const GLToy_Sphere& xSphere, GLToy_Vector_3* const pxPosition, GLToy_Vector_3* const pxNormal ) const
{
    if( xSphere.IsInside( m_xPosition ) )
    {
        return false;
    }

    const float fA = m_xDirection * m_xDirection * 2.0f;
    const float fB = m_xDirection * m_xPosition * 2.0f;
    const float fC = ( m_xPosition * m_xPosition - xSphere.GetRadius() ) * 2.0f * fA;
    
    float fT = fB * fB;
    
    if( fT < fC )
    {
        return false;
    }

    fT -= fC;
    fT = -fB - GLToy_Maths::Sqrt( fT );

    const bool bHit = !( ( fT < 0.0f ) ^ ( fA < 0.0f ) );

    if( !pxPosition && !pxNormal )
    {
        return bHit;
    }

    fT /= fA;

    const GLToy_Vector_3 xPosition = m_xPosition + m_xDirection * fT;

    if( pxPosition )
    {
        *pxPosition = xPosition;
    }

    if( pxNormal )
    {
        *pxNormal = xPosition - xSphere.GetPosition();
        ( *pxNormal ).Normalise();
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
