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
#include <Render/GLToy_Render.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Ray::IntersectsWithPlane( const GLToy_Plane& xPlane, float* const pfParameter, GLToy_Vector_3* const pxPosition, GLToy_Vector_3* const pxNormal ) const
{
    const float fTop = xPlane.GetDistance() + xPlane.GetNormal() * m_xPosition;
    const float fBottom = xPlane.GetNormal() * m_xDirection;

    // this answers "is fTop / fBottom negative?", which is the same as if we hit, and should be cheaper than a division
    const bool bHit = ( ( fTop < 0.0f ) ^ ( fBottom < 0.0f ) );

    if( pfParameter )
    {
        *pfParameter = -fTop / fBottom;

        if( pxPosition )
        {
            *pxPosition = m_xPosition + m_xDirection * *pfParameter;
        }
    }
    else if( pxPosition )
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

bool GLToy_Ray::IntersectsWithAABB( const GLToy_AABB& xAABB, float* const pfParameter, GLToy_Vector_3* const pxPosition, GLToy_Vector_3* const pxNormal ) const
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

    // now do the actual intersection - 
    // for each axis we only need to intersect against a single plane - the one facing opposite to the ray direction (back face culling)
    GLToy_Vector_3 axIntersections[ 3 ];
    
    const GLToy_Vector_3 axNormalsMax[ 3 ] =
    {
        GLToy_Vector_3( 1.0f, 0.0f, 0.0f ),
        GLToy_Vector_3( 0.0f, 1.0f, 0.0f ),
        GLToy_Vector_3( 0.0f, 0.0f, 1.0f ),
    };

    bool abHit[ 3 ] =
    {
        false,
        false,
        false,
    };

    const u_int aauIndexHelper[ 3 ][ 2 ] =
    {
        { 1, 2 },
        { 2, 0 },
        { 0, 1 }
    };

    float fParameter[ 3 ];

    static const u_int uRAYTRACE_AABB_NOHIT_INDEX = 4;
    u_int uHitIndex = uRAYTRACE_AABB_NOHIT_INDEX;
    for( u_int u = 0; u < 3; ++u )
    {
        if( m_xDirection[ u ] > 0.0f )
        {
            abHit[ u ] = IntersectsWithPlane( GLToy_Plane( -axNormalsMax[ u ], xMin[ u ] ), &( fParameter[ u ] ), &( axIntersections[ u ] ) );
        }
        else
        {
            abHit[ u ] = IntersectsWithPlane( GLToy_Plane( axNormalsMax[ u ], -xMax[ u ] ), &( fParameter[ u ] ), &( axIntersections[ u ] ) );
        }

        // check the intersection point is actually within bounds and decide if we hit accordingly
        for( u_int v = 0; v < 3; ++v )
        {
            const u_int uIndex1 = aauIndexHelper[ u ][ 0 ];
            const u_int uIndex2 = aauIndexHelper[ u ][ 1 ];
            if( axIntersections[ u ][ uIndex1 ] < xMin[ uIndex1 ] )
            {
                abHit[ u ] = false;
            }

            if( axIntersections[ u ][ uIndex1 ] > xMax[ uIndex1 ] )
            {
                abHit[ u ] = false;
            }

            if( axIntersections[ u ][ uIndex2 ] < xMin[ uIndex2 ] )
            {
                abHit[ u ] = false;
            }

            if( axIntersections[ u ][ uIndex2 ] > xMax[ uIndex2 ] )
            {
                abHit[ u ] = false;
            }
        }

        if( abHit[ u ] )
        {
            uHitIndex = u;
            break; // early out if we have hit
        }
    }

    if( uHitIndex == uRAYTRACE_AABB_NOHIT_INDEX )
    {
        return false;
    }

    if( pfParameter )
    {
        *pfParameter = fParameter[ uHitIndex ];
    }

    if( pxPosition )
    {
        *pxPosition = axIntersections[ uHitIndex ];
    }

    if( pxNormal )
    {
        *pxNormal = ( m_xDirection[ uHitIndex ] > 0.0f ) ? -axNormalsMax[ uHitIndex ] : axNormalsMax[ uHitIndex ];
    }

    return true;
}

bool GLToy_Ray::IntersectsWithSphere( const GLToy_Sphere& xSphere, float* const pfParameter, GLToy_Vector_3* const pxPosition, GLToy_Vector_3* const pxNormal ) const
{
    if( xSphere.IsInside( m_xPosition ) )
    {
        return false;
    }

    // xParams = ( 2a, b, 2c ) where a, b, c are the traditional coefficients for at^2 + bt + c = 0 
    const GLToy_Vector_3 xParams = 2.0f * GLToy_Vector_3( m_xDirection * m_xDirection, m_xDirection * m_xPosition, m_xPosition * m_xPosition - xSphere.GetRadius() * xSphere.GetRadius() );
    
    // b^2 - 4ac
    const float fDiscriminant = xParams[ 1 ] * xParams[ 1 ] - xParams[ 0 ] * xParams[ 2 ];
    
    if( fDiscriminant < 0 )
    {
        return false;
    }

    // -b - sqrt( b^2 - 4ac )
    float fT = ( -xParams[ 1 ] - GLToy_Maths::Sqrt( fDiscriminant ) );

    const bool bHit = !( ( fT < 0.0f ) ^ ( xParams[ 0 ] < 0.0f ) );

    if( !pxPosition && !pxNormal )
    {
        return bHit;
    }

    // ( -b - sqrt( b^2 - 4ac ) ) / 2a
    fT /= xParams[ 0 ];

    if( pfParameter )
    {
        *pfParameter = fT;
    }

    if( pxPosition || pxNormal )
    {
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
    }

    return false;
}

// TODO: implement this
bool GLToy_Ray::IntersectsWithOBB( const GLToy_OBB& xOBB, float* const pfParameter, GLToy_Vector_3* const pxPosition, GLToy_Vector_3* const pxNormal ) const
{
    return false;
}

bool GLToy_Ray::IntersectsWith( const GLToy_Bounded& xBounded, float* const pfParameter, GLToy_Vector_3* const pxPosition, GLToy_Vector_3* const pxNormal ) const
{
    return xBounded.IntersectWithRay( *this, pfParameter, pxPosition, pxNormal );
}

void GLToy_Ray::Render() const
{
    GLToy_Render::EnableBlending();
    GLToy_Render::StartSubmittingLines();

    GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );
    GLToy_Render::SubmitVertex( m_xPosition );
    GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 0.0f ) );
    GLToy_Render::SubmitVertex( m_xPosition + m_xDirection * 32.0f );

    GLToy_Render::EndSubmit();
    GLToy_Render::DisableBlending();
}
