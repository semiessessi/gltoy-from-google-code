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

    GLToy_Ray( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xDirection )
    : m_xPosition( xPosition )
    , m_xDirection( xDirection )
    {
    }

    const GLToy_Vector_3& GetPosition() const { return m_xPosition; }
    const GLToy_Vector_3& GetDirection() const { return m_xDirection; }

    bool IntersectsWithPlane( const GLToy_Plane& xPlane, float* const pfParameter = NULL, GLToy_Vector_3* const pxPosition = NULL, GLToy_Vector_3* const pxNormal = NULL ) const;
    bool IntersectsWithAABB( const GLToy_AABB& xAABB, GLToy_Vector_3* const pxPosition = NULL, GLToy_Vector_3* const pxNormal = NULL ) const;
    bool IntersectsWithSphere( const GLToy_Sphere& xSphere, GLToy_Vector_3* const pxPosition = NULL, GLToy_Vector_3* const pxNormal = NULL ) const;
    bool IntersectsWithOBB( const GLToy_OBB& xOBB, GLToy_Vector_3* const pxPosition = NULL, GLToy_Vector_3* const pxNormal = NULL ) const;
    bool IntersectsWith( const GLToy_Bounded& xBounded, GLToy_Vector_3* const pxPosition = NULL, GLToy_Vector_3* const pxNormal = NULL ) const;

protected:

    GLToy_Vector_3 m_xPosition;
    GLToy_Vector_3 m_xDirection;

};

#endif
