/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010, 2011 Semi Essessi
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

#ifndef __GLTOY_CONVEXHULL_H_
#define __GLTOY_CONVEXHULL_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/Data Structures/GLToy_Array.h>
#include <Maths/GLToy_Plane.h>
#include <Maths/GLToy_Volume.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_ConvexHull
: public GLToy_Array< GLToy_Plane >
, public GLToy_Volume< GLToy_ConvexHull >
{

    typedef GLToy_Volume< GLToy_ConvexHull > GLToy_Parent;
    typedef GLToy_Array< GLToy_Plane > GLToy_DataParent;

public:

    GLToy_ConvexHull( const GLToy_Vector_3& xPosition )
    : GLToy_Parent( xPosition )
    , GLToy_DataParent()
    {
    }

    float GetSurfaceArea() const
    {
        return GLToy_Maths::LargeFloat;
    }

    float GetVolume() const
    {
        return GLToy_Maths::LargeFloat;
    }

    bool IsInside( const GLToy_Vector_3& xPosition ) const
    {
        // if on the wrong side of any plane, then outside
        for( GLToy_ConstIterator< GLToy_Plane > xIterator; !xIterator.Done( *this ); xIterator.Next() )
{
const GLToy_Plane& xPlane = xIterator.Current( *this );
            if( xPlane.IsOnPositiveSide( xPosition ) )
            {
                return false;
            }
        }

        return true;
    }

    void SetToPoint( const GLToy_Vector_3& xPosition ) {}
    void GrowByPoint( const GLToy_Vector_3& xPosition ) {}

    void ReadFromBitStream( const GLToy_BitStream& xStream ) {}
    void WriteToBitStream( GLToy_BitStream& xStream ) const {}

    GLToy_Array< GLToy_Vector_3 > GeneratePointCloud() const;

protected:

};

#endif
