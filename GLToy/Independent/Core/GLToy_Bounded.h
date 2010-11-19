/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi, Thomas Young
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

    GLToy_Bounded()
    {
    }

    virtual ~GLToy_Bounded() {}

    virtual const GLToy_Vector_3& GetPosition() const = 0;
    virtual void SetPosition( const GLToy_Vector_3& xPosition ) = 0;

    virtual GLToy_Sphere GetBoundingSphere() const = 0;

    virtual bool IntersectWithRay( const GLToy_Ray& xRay, float* const pfParameter = 0, GLToy_Vector_3* const pxPosition = 0, GLToy_Vector_3* const pxNormal = 0 ) const = 0;
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

    virtual GLToy_Sphere GetBoundingSphere() const { return m_xBoundingBox.GetBoundingSphere(); }

    GLToy_Inline GLToy_AABB& GetBB() { return m_xBoundingBox; }
    GLToy_Inline const GLToy_AABB& GetBB() const { return m_xBoundingBox; }
    GLToy_Inline void SetBB( const GLToy_AABB& xBB ) { m_xBoundingBox = xBB; }

    virtual const GLToy_Vector_3& GetPosition() const { return m_xBoundingBox.GetPosition(); }
    virtual float GetDistanceToPoint( const GLToy_Vector_3& xPoint ) const
	{
		GLToy_Vector_3 xToPoint = xPoint - m_xBoundingBox.GetPosition();
		return xToPoint.Magnitude();
	}

    virtual void SetPosition( const GLToy_Vector_3& xPosition )
    {
        GLToy_Vector_3 xDiff = xPosition - GetPosition();
        m_xBoundingBox = GLToy_AABB( m_xBoundingBox.GetMax() + xDiff, m_xBoundingBox.GetMin() + xDiff );

    }

    void SetBBToPoint( const GLToy_Vector_3& xPosition ) { m_xBoundingBox.SetToPoint( xPosition ); }
    void GrowBBByPoint( const GLToy_Vector_3& xPosition ) { m_xBoundingBox.GrowByPoint( xPosition ); }

    virtual bool IntersectWithRay(
        const GLToy_Ray& xRay,
        float* const pfParameter = 0,
        GLToy_Vector_3* const pxPosition = 0,
        GLToy_Vector_3* const pxNormal = 0 ) const
    {
        return xRay.IntersectsWithAABB( m_xBoundingBox, pfParameter, pxPosition, pxNormal );
    }

    virtual bool IntersectWithAABB( const GLToy_AABB& xAABB ) const
    {
        return m_xBoundingBox.IntersectsWithAABB( xAABB );
    }

//protected:

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

    virtual GLToy_Sphere GetBoundingSphere() const { return m_xBoundingSphere; }

    virtual void SetPosition( const GLToy_Vector_3& xPosition ) { m_xBoundingSphere = GLToy_Sphere( xPosition, m_xBoundingSphere.GetRadius() ); }
    virtual const GLToy_Vector_3& GetPosition() const { return m_xBoundingSphere.GetPosition(); }
    
    virtual void SetRadius( const float fRadius ) { return m_xBoundingSphere.SetRadius( fRadius ); }
    virtual float GetRadius() { return m_xBoundingSphere.GetRadius(); }

    void SetBoundingSphereToPoint( const GLToy_Vector_3& xPosition ) { m_xBoundingSphere.SetToPoint( xPosition ); }
    void GrowBoundingSphereByPoint( const GLToy_Vector_3& xPosition ) { m_xBoundingSphere.GrowByPoint( xPosition ); }

    virtual bool IntersectWithRay(
        const GLToy_Ray& xRay,
        float* const pfParameter = 0,
        GLToy_Vector_3* const pxPosition = 0,
        GLToy_Vector_3* const pxNormal = 0 ) const
    {
        return xRay.IntersectsWithSphere( m_xBoundingSphere, pfParameter, pxPosition, pxNormal );
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

    virtual GLToy_Sphere GetBoundingSphere() const { return m_xBoundingBox.GetBoundingSphere(); }

    GLToy_OBB& GetOBB() { return m_xBoundingBox; }
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
        float* const pfParameter = 0,
        GLToy_Vector_3* const pxPosition = 0,
        GLToy_Vector_3* const pxNormal = 0 ) const
    {
        return xRay.IntersectsWithOBB( m_xBoundingBox, pfParameter, pxPosition, pxNormal );
    }

    virtual bool IntersectWithAABB( const GLToy_AABB& xAABB ) const
    {
        return m_xBoundingBox.IntersectsWithAABB( xAABB );
    }

protected:

    GLToy_OBB m_xBoundingBox;

};

#endif
