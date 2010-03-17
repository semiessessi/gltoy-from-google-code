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

    bool IntersectsWithPlane( const GLToy_Plane& xPlane, GLToy_Vector_3* const pxPosition = NULL, GLToy_Vector_3* const pxNormal = NULL ) const;
    bool IntersectsWithAABB( const GLToy_AABB& xAABB, GLToy_Vector_3* const pxPosition = NULL, GLToy_Vector_3* const pxNormal = NULL ) const;
    bool IntersectsWithSphere( const GLToy_Sphere& xSphere, GLToy_Vector_3* const pxPosition = NULL, GLToy_Vector_3* const pxNormal = NULL ) const;
    bool IntersectsWithOBB( const GLToy_OBB& xOBB, GLToy_Vector_3* const pxPosition = NULL, GLToy_Vector_3* const pxNormal = NULL ) const;
    bool IntersectsWith( const GLToy_Bounded& xBounded, GLToy_Vector_3* const pxPosition = NULL, GLToy_Vector_3* const pxNormal = NULL ) const;

protected:

    GLToy_Vector_3 m_xPosition;
    GLToy_Vector_3 m_xDirection;

};

#endif