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

#ifndef __GLTOY_RAY_H_
#define __GLTOY_RAY_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_AABB;
class GLToy_Bounded;
class GLToy_OBB;
class GLToy_Plane;
class GLToy_Sphere;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Ray
{

public:

    GLToy_Ray()
    : m_xPosition( 0.0f, 0.0f, 0.0f )
    , m_xDirection( 0.0f, 0.0f, 1.0f )
    {
    }

    GLToy_Ray( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xDirection )
    : m_xPosition( xPosition )
    , m_xDirection( xDirection )
    {
    }

    const GLToy_Vector_3& GetPosition() const { return m_xPosition; }
    const GLToy_Vector_3& GetDirection() const { return m_xDirection; }

    GLToy_Vector_3 Evaluate( const float fParameter ) const { return m_xPosition + m_xDirection * fParameter; }

    bool IntersectsWithPlane( const GLToy_Plane& xPlane, float* const pfParameter = NULL, GLToy_Vector_3* const pxPosition = NULL, GLToy_Vector_3* const pxNormal = NULL ) const;
    bool IntersectsWithAABB( const GLToy_AABB& xAABB, float* const pfParameter = NULL, GLToy_Vector_3* const pxPosition = NULL, GLToy_Vector_3* const pxNormal = NULL ) const;
    bool IntersectsWithSphere( const GLToy_Sphere& xSphere, GLToy_Vector_3* const pxPosition = NULL, GLToy_Vector_3* const pxNormal = NULL ) const;
    bool IntersectsWithOBB( const GLToy_OBB& xOBB, GLToy_Vector_3* const pxPosition = NULL, GLToy_Vector_3* const pxNormal = NULL ) const;
    bool IntersectsWith( const GLToy_Bounded& xBounded, GLToy_Vector_3* const pxPosition = NULL, GLToy_Vector_3* const pxNormal = NULL ) const;

    virtual void Render() const;

protected:

    GLToy_Vector_3 m_xPosition;
    GLToy_Vector_3 m_xDirection;

};

#endif
