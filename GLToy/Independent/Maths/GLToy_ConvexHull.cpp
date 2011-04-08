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

// GLToy
#include <Core/GLToy.h>

// this file's header
#include <Maths/GLToy_ConvexHull.h>

// GLToy
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Array< GLToy_Vector_3 > GLToy_ConvexHull::GeneratePointCloud() const
{
    GLToy_Array< GLToy_Vector_3 > xReturnValue;

    // for each plane
    for( GLToy_ConstIterator< GLToy_Plane > xIterator; !xIterator.Done( *this ); xIterator.Next() )
{
const GLToy_Plane& xPlane1 = xIterator.Current( *this );
        // for each other plane
        for( GLToy_ConstIterator< GLToy_Plane > xIterator; !xIterator.Done( *this ); xIterator.Next() )
{
const GLToy_Plane& xPlane2 = xIterator.Current( *this );
            if( xPlane1 == xPlane2 )
            {
                continue;
            }

            // find the line of intersection
            const GLToy_Ray xIntersection = GLToy_Maths::PlanePlaneIntersection( xPlane1, xPlane2 );
            
            // now for every other plane, clip the line
            GLToy_Array< GLToy_Plane > xClipPlanes;
            for( GLToy_ConstIterator< GLToy_Plane > xIterator; !xIterator.Done( *this ); xIterator.Next() )
{
const GLToy_Plane& xClipPlane = xIterator.Current( *this );
                if( ( xClipPlane == xPlane1 ) || ( xClipPlane == xPlane2 ) )
                {
                    continue;
                }
                xClipPlanes.Append( xClipPlane );
            }

            const GLToy_Pair< GLToy_Vector_3 > xEndPoints = GLToy_Maths::ClipLineWithPlanes( xIntersection, xClipPlanes );

            // see if we already have vertex 1 or add it
            bool bFound = false;
            for( GLToy_ConstIterator< GLToy_Vector_3 > xIterator; !xIterator.Done( xReturnValue ); xIterator.Next() )
{
const GLToy_Vector_3& xVertex = xIterator.Current( xReturnValue );
                if( xVertex.ApproximatelyEqual( xEndPoints.First() ) )
                {
                    bFound = true;
                    break;
                }
            }
            if( !bFound )
            {
                xReturnValue.Append( xEndPoints.First() );
            }
            
            // see if we already have vertex 2 or add it
            bFound = false;
            for( GLToy_ConstIterator< GLToy_Vector_3 > xIterator; !xIterator.Done( xReturnValue ); xIterator.Next() )
{
const GLToy_Vector_3& xVertex = xIterator.Current( xReturnValue );
                if( xVertex.ApproximatelyEqual( xEndPoints.Second() ) )
                {
                    bFound = true;
                    break;
                }
            }
            if( !bFound )
            {
                xReturnValue.Append( xEndPoints.Second() );
            }
        }
    }

    return xReturnValue;
}
