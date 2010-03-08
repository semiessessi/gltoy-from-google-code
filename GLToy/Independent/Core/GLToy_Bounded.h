#ifndef __GLTOY_BOUNDED_H_
#define __GLTOY_BOUNDED_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Maths/GLToy_Volume.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Bounded_AABB
{

public:

    GLToy_Bounded_AABB()
    : m_xBoundingBox()
    {
    }

    ~GLToy_Bounded_AABB()
    {
    }

    const GLToy_AABB& GetBB() const { return m_xBoundingBox; }

    void SetBBToPoint( const GLToy_Vector_3& xPosition ) { m_xBoundingBox.SetToPoint( xPosition ); }
    void GrowBBByPoint( const GLToy_Vector_3& xPosition ) { m_xBoundingBox.GrowByPoint( xPosition ); }

protected:

    GLToy_AABB m_xBoundingBox;

};

class GLToy_Bounded_Sphere
{

public:

    GLToy_Bounded_Sphere()
    : m_xBoundingSphere()
    {
    }

    ~GLToy_Bounded_Sphere()
    {
    }

    const GLToy_Sphere& GetBoundingSphere() const { return m_xBoundingSphere; }

    void SetBoundingSphereToPoint( const GLToy_Vector_3& xPosition ) { m_xBoundingSphere.SetToPoint( xPosition ); }
    void GrowBoundingSphereByPoint( const GLToy_Vector_3& xPosition ) { m_xBoundingSphere.GrowByPoint( xPosition ); }

protected:

    GLToy_Sphere m_xBoundingSphere;

};

#endif