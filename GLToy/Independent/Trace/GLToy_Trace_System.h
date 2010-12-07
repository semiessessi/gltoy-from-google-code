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

#ifndef __GLTOY_TRACE_SYSTEM_H_
#define __GLTOY_TRACE_SYSTEM_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Maths/GLToy_Ray.h>
#include <Maths/GLToy_Vector.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// M A C R O S
/////////////////////////////////////////////////////////////////////////////////////////////

#define GLToy_Trace_System_VectorPairOverload( xReturnType, xFunction ) static GLToy_Inline xReturnType xFunction( const GLToy_Vector_3& xPosition1, const GLToy_Vector_3& xPosition2 )\
    {\
        const float fLimitingDistance = ( xPosition2 - xPosition1 ).Magnitude();\
        const GLToy_Ray xRay = GLToy_Ray::FromPositions( xPosition1, xPosition2 );\
        return xFunction( xRay, fLimitingDistance );\
    }

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Trace_Result
{

public:

    bool m_bHit;
    float m_fParameter;
    // TODO: although the raytrace functions at the bottom of this stuff can always get these they aren't propogated upwards yet
    GLToy_Vector_3 m_xPosition;
    GLToy_Vector_3 m_xNormal;
    GLToy_Hash m_uHitEntity;

};

class GLToy_Trace_System
{

public:

    static bool LineOfSightTest( const GLToy_Ray& xRay, const float fLimitingDistance );
    static float Trace( const GLToy_Ray& xRay, const float fLimitingDistance );
    static GLToy_Hash TraceEntity( const GLToy_Ray& xRay, const float fLimitingDistance );

    static GLToy_Trace_Result FullTrace( const GLToy_Ray& xRay, const float fLimitingDistance );

    GLToy_Trace_System_VectorPairOverload( bool, LineOfSightTest )
    GLToy_Trace_System_VectorPairOverload( float, Trace )
    GLToy_Trace_System_VectorPairOverload( GLToy_Hash, TraceEntity )

    GLToy_Trace_System_VectorPairOverload( GLToy_Trace_Result, FullTrace )

private:

};

#endif
