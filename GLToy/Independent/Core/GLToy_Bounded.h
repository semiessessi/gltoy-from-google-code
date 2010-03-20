#ifndef __GLTOY_BOUNDED_H_
#define __GLTOY_BOUNDED_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Maths/GLToy_Ray.h>
#include <Maths/GLToy_Volume.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Bounded
{

public:

    virtual const GLToy_Vector_3& GetPosition() const = 0;
    virtual void SetPosition( const GLToy_Vector_3& xPosition ) = 0;

    virtual bool IntersectWithRay( const GLToy_Ray& xRay, GLToy_Vector_3* const pxPosition, GLToy_Vector_3* const pxNormal ) const = 0;
    virtual bool IntersectWithAABB( const GLToy_AABB& xAABB ) const = 0;
    //virtual bool IntersectWithOBB();
    //virtual bool IntersectWithSphere();

};

class GLToy_Bounded_AABB
: public virtual GLToy_Bounded
{

public:

    GLToy_Bounded_AABB()
    : GLToy_Bounded()
    , m_xBoundingBox()
    {
    }

    virtual ~GLToy_Bounded_AABB()
    {
    }

    GLToy_Inline const GLToy_AABB& GetBB() const { return m_xBoundingBox; }
    GLToy_Inline void SetBB( const GLToy_AABB& xBB ) { m_xBoundingBox = xBB; }

    virtual const GLToy_Vector_3& GetPosition() const { return m_xBoundingBox.GetPosition(); }
    
    virtual void SetPosition( const GLToy_Vector_3& xPosition )
    {
        GLToy_Vector_3 xDiff = xPosition - GetPosition();
        m_xBoundingBox = GLToy_AABB( m_xBoundingBox.GetMax() + xDiff, m_xBoundingBox.GetMin() + xDiff );

    }

    void SetBBToPoint( const GLToy_Vector_3& xPosition ) { m_xBoundingBox.SetToPoint( xPosition ); }
    void GrowBBByPoint( const GLToy_Vector_3& xPosition ) { m_xBoundingBox.GrowByPoint( xPosition ); }

    virtual bool IntersectWithRay(
        const GLToy_Ray& xRay,
        GLToy_Vector_3* const pxPosition,
        GLToy_Vector_3* const pxNormal ) const
    {
        return xRay.IntersectsWithAABB( m_xBoundingBox, pxPosition, pxNormal );
    }

    virtual bool IntersectWithAABB( const GLToy_AABB& xAABB ) const
    {
        return m_xBoundingBox.IntersectsWithAABB( xAABB );
    }

protected:

    GLToy_AABB m_xBoundingBox;

};

class GLToy_Bounded_Sphere
: public virtual GLToy_Bounded
{

public:

    GLToy_Bounded_Sphere()
    : GLToy_Bounded()
    , m_xBoundingSphere()
    {
    }

    virtual ~GLToy_Bounded_Sphere()
    {
    }

    const GLToy_Sphere& GetBoundingSphere() const { return m_xBoundingSphere; }
    virtual const GLToy_Vector_3& GetPosition() const { return m_xBoundingSphere.GetPosition(); }

    virtual void SetPosition( const GLToy_Vector_3& xPosition )
    {
        m_xBoundingSphere = GLToy_Sphere( xPosition, m_xBoundingSphere.GetRadius() );
    }

    void SetBoundingSphereToPoint( const GLToy_Vector_3& xPosition ) { m_xBoundingSphere.SetToPoint( xPosition ); }
    void GrowBoundingSphereByPoint( const GLToy_Vector_3& xPosition ) { m_xBoundingSphere.GrowByPoint( xPosition ); }

    virtual bool IntersectWithRay(
        const GLToy_Ray& xRay,
        GLToy_Vector_3* const pxPosition,
        GLToy_Vector_3* const pxNormal ) const
    {
        return xRay.IntersectsWithSphere( m_xBoundingSphere, pxPosition, pxNormal );
    }

    virtual bool IntersectWithAABB( const GLToy_AABB& xAABB ) const
    {
        return m_xBoundingSphere.IntersectsWithAABB( xAABB );
    }

protected:

    GLToy_Sphere m_xBoundingSphere;

};

class GLToy_Bounded_OBB
: public virtual GLToy_Bounded
{

public:

    GLToy_Bounded_OBB()
    : GLToy_Bounded()
    , m_xBoundingBox()
    {
    }

    virtual ~GLToy_Bounded_OBB()
    {
    }

    const GLToy_OBB& GetOBB() const { return m_xBoundingBox; }
    void SetOBB( const GLToy_OBB& xBB ) { m_xBoundingBox = xBB; }

    virtual const GLToy_Vector_3& GetPosition() const { return m_xBoundingBox.GetPosition(); }
    
    virtual void SetPosition( const GLToy_Vector_3& xPosition )
    {
        GLToy_Vector_3 xDiff = xPosition - GetPosition();
        m_xBoundingBox = GLToy_OBB( GLToy_AABB( m_xBoundingBox.GetUnrotatedBB().GetMax() + xDiff, m_xBoundingBox.GetUnrotatedBB().GetMin() + xDiff ), m_xBoundingBox.GetOrientation() );

    }

    void SetBBToPoint( const GLToy_Vector_3& xPosition ) { m_xBoundingBox.SetToPoint( xPosition ); }
    void GrowBBByPoint( const GLToy_Vector_3& xPosition ) { m_xBoundingBox.GrowByPoint( xPosition ); }

    virtual bool IntersectWithRay(
        const GLToy_Ray& xRay,
        GLToy_Vector_3* const pxPosition,
        GLToy_Vector_3* const pxNormal ) const
    {
        return xRay.IntersectsWithOBB( m_xBoundingBox, pxPosition, pxNormal );
    }

    virtual bool IntersectWithAABB( const GLToy_AABB& xAABB ) const
    {
        return m_xBoundingBox.IntersectsWithAABB( xAABB );
    }

protected:

    GLToy_OBB m_xBoundingBox;

};

#endif