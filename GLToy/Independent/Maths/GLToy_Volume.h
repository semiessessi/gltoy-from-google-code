/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010, 2011 Semi Essessi, Thomas Young
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

#ifndef __GLTOY_VOLUME_H_
#define __GLTOY_VOLUME_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parent
#include <Render/GLToy_Renderable.h>

// GLToy
#include <Core/Data Structures/GLToy_BitStream.h>
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_AABB;
class GLToy_Sphere;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class Derived >
class GLToy_Volume
{

public:

    GLToy_Volume()
    : m_xPosition( GLToy_Maths::ZeroVector3 )
    {
    }

    GLToy_Volume( const GLToy_Vector_3& xPosition )
    : m_xPosition( xPosition )
    {
    }

    GLToy_ForceInline const GLToy_Vector_3& GetPosition() const { return m_xPosition; }

    //virtual float GetSurfaceArea() const = 0;
    //virtual float GetVolume() const = 0;

    //virtual bool IsInside( const GLToy_Vector_3& xPosition ) const = 0;
    GLToy_ForceInline bool IsOutside( const GLToy_Vector_3& xPosition ) const
    {
        return !( static_cast< const Derived* const >( this )->IsInside( xPosition ) );
    }

    //bool HasBoundingSphere() const { return false; }
    //bool HasBoundingAABB() const { return false; }
    //GLToy_Sphere GetBoundingSphere() const;
    //GLToy_AABB GetBoundingAABB() const;

    //virtual void SetToPoint( const GLToy_Vector_3& xPosition ) = 0;
    //virtual void GrowByPoint( const GLToy_Vector_3& xPosition ) = 0;

    void ReadFromBitStream( const GLToy_BitStream& xStream );
    void WriteToBitStream( GLToy_BitStream& xStream ) const;

protected:

    GLToy_Vector_3 m_xPosition;

};

class GLToy_AABB
: public GLToy_Volume< GLToy_AABB >
//, public GLToy_Renderable
{

    typedef GLToy_Volume< GLToy_AABB > GLToy_Parent;

public:
    
    GLToy_AABB()
    : GLToy_Parent()
    , m_xPointMax( GLToy_Maths::ZeroVector3 )
    , m_xPointMin( GLToy_Maths::ZeroVector3 )
    {
    } 

    GLToy_AABB( const GLToy_Vector_3& xPosition, const float fHalfExtentX, const float fHalfExtentY, const float fHalfExtentZ )
    : GLToy_Parent( xPosition )
    , m_xPointMax( xPosition + GLToy_Vector_3( fHalfExtentX, fHalfExtentY, fHalfExtentZ ) )
    , m_xPointMin( xPosition - GLToy_Vector_3( fHalfExtentX, fHalfExtentY, fHalfExtentZ ) )
    {
    }

    GLToy_AABB( const GLToy_Vector_3& xPointMax, const GLToy_Vector_3& xPointMin )
    : GLToy_Parent( ( xPointMax + xPointMin ) * 0.5f )
    , m_xPointMax( xPointMax )
    , m_xPointMin( xPointMin )
    {
    }

    void Render() const;
    void RenderFlat() const;

    float GetSurfaceArea() const
    {
        const float fX = m_xPointMax[ 0 ] - m_xPointMin[ 0 ];
        const float fY = m_xPointMax[ 1 ] - m_xPointMin[ 1 ];
        const float fZ = m_xPointMax[ 2 ] - m_xPointMin[ 2 ];
        
        return 2.0f * ( fX * fY + fY * fZ + fZ * fX );
    }

    float GetVolume() const
    {
        return ( m_xPointMax[ 0 ] - m_xPointMin[ 0 ] )
            * ( m_xPointMax[ 1 ] - m_xPointMin[ 1 ] )
            * ( m_xPointMax[ 2 ] - m_xPointMin[ 2 ] );
    }

    bool IsInside( const GLToy_Vector_3& xPosition ) const
    {
        return ( xPosition[ 0 ] > m_xPointMin[ 0 ] )
            && ( xPosition[ 1 ] > m_xPointMin[ 1 ] )
            && ( xPosition[ 2 ] > m_xPointMin[ 2 ] )
            && ( xPosition[ 0 ] < m_xPointMax[ 0 ] )
            && ( xPosition[ 1 ] < m_xPointMax[ 1 ] )
            && ( xPosition[ 2 ] < m_xPointMax[ 2 ] );
    }

    bool HasBoundingAABB() const { return true; }
    bool HasBoundingSphere() const { return true; }

    GLToy_AABB GetBoundingAABB() const
    {
        return *this;
    }

    GLToy_Sphere GetBoundingSphere() const;

    void SetToPoint( const GLToy_Vector_3& xPosition )
    {
        m_xPosition = m_xPointMax = m_xPointMin = xPosition;
    }

    void GrowByPoint( const GLToy_Vector_3& xPosition )
    {
        m_xPointMax[ 0 ] = GLToy_Maths::Max( xPosition[ 0 ], m_xPointMax[ 0 ] );
        m_xPointMax[ 1 ] = GLToy_Maths::Max( xPosition[ 1 ], m_xPointMax[ 1 ] );
        m_xPointMax[ 2 ] = GLToy_Maths::Max( xPosition[ 2 ], m_xPointMax[ 2 ] );
        m_xPointMin[ 0 ] = GLToy_Maths::Min( xPosition[ 0 ], m_xPointMin[ 0 ] );
        m_xPointMin[ 1 ] = GLToy_Maths::Min( xPosition[ 1 ], m_xPointMin[ 1 ] );
        m_xPointMin[ 2 ] = GLToy_Maths::Min( xPosition[ 2 ], m_xPointMin[ 2 ] );

        m_xPosition = ( m_xPointMax + m_xPointMin ) * 0.5f;
    }

    void ReadFromBitStream( const GLToy_BitStream& xStream );
    void WriteToBitStream( GLToy_BitStream& xStream ) const;

	GLToy_ForceInline const GLToy_Vector_3& GetMax() const { return m_xPointMax; }
    GLToy_ForceInline const GLToy_Vector_3& GetMin() const { return m_xPointMin; }
    GLToy_Inline GLToy_Vector_3 GetExtents() const { return m_xPointMax - m_xPointMin; }
    GLToy_Inline GLToy_Vector_3 GetHalfExtents() const { return m_xPosition - m_xPointMin; }

    bool IntersectsWithAABB( const GLToy_AABB& xAABB ) const;

    GLToy_AABB operator +( const GLToy_Vector_3& xVector ) const
    {
        return GLToy_AABB( m_xPointMax + xVector, m_xPointMin + xVector );
    }

    GLToy_AABB operator -( const GLToy_Vector_3& xVector ) const
    {
        return GLToy_AABB( m_xPointMax - xVector, m_xPointMin - xVector );
    }

	GLToy_Plane GetPlane( int iIndex ) const;

//protected:

    GLToy_Vector_3 m_xPointMax;
    GLToy_Vector_3 m_xPointMin;

};

class GLToy_Sphere
: public GLToy_Volume< GLToy_Sphere >
//, public GLToy_Renderable
{
  
    typedef GLToy_Volume< GLToy_Sphere > GLToy_Parent;

public:
    
    GLToy_Sphere()
    : GLToy_Parent()
    , m_fRadius( 0.0f )
    {
    }

    GLToy_Sphere( const GLToy_Vector_3& xPosition, const float fRadius )
    : GLToy_Parent( xPosition )
    , m_fRadius( fRadius )
    {
    }

    void Render() const;

    float GetSurfaceArea() const
    {
        return 4.0f * GLToy_Maths::Pi * m_fRadius * m_fRadius;
    }

    float GetVolume() const
    {
        return ( 4.0f / 3.0f ) * GLToy_Maths::Pi * m_fRadius * m_fRadius * m_fRadius;
    }

    bool IsInside( const GLToy_Vector_3& xPosition ) const
    {
        return ( xPosition - m_xPosition ).Magnitude() < m_fRadius;
    }

    bool HasBoundingSphere() const { return true; }
    bool HasBoundingAABB() const { return true; }

    GLToy_Sphere GetBoundingSphere() const
    {
        return *this;
    }

    GLToy_AABB GetBoundingAABB() const
    {
        return GLToy_AABB( GetPosition(), m_fRadius, m_fRadius, m_fRadius );
    }

    void SetToPoint( const GLToy_Vector_3& xPosition )
    {
        m_xPosition = xPosition;
        m_fRadius = 0.0f;
    }

    void GrowByPoint( const GLToy_Vector_3& xPosition )
    {
        m_fRadius = GLToy_Maths::Max( m_fRadius, ( xPosition - m_xPosition ).Magnitude() );
    }

    void ReadFromBitStream( const GLToy_BitStream& xStream );
    void WriteToBitStream( GLToy_BitStream& xStream ) const;

    GLToy_ForceInline void SetRadius( const float fRadius ) { m_fRadius = fRadius; }
    GLToy_ForceInline float GetRadius() const { return m_fRadius; }

    bool IntersectsWithAABB( const GLToy_AABB& xAABB ) const;
    bool IntersectsWithSphere( const GLToy_Sphere& xSphere ) const;

protected:

    float m_fRadius;

};

class GLToy_OBB
: public GLToy_Volume< GLToy_OBB >
//, public GLToy_Renderable
{

    typedef GLToy_Volume< GLToy_OBB > GLToy_Parent;

public:
    
    GLToy_OBB()
    : GLToy_Parent()
    , m_xBox()
    , m_xOrientation( GLToy_Maths::IdentityMatrix3 )
    {
    } 

    GLToy_OBB( const GLToy_Vector_3& xPosition, const GLToy_Matrix_3& xOrientation, const float fHalfExtentX, const float fHalfExtentY, const float fHalfExtentZ )
    : GLToy_Parent( xPosition )
    , m_xBox( xPosition + GLToy_Vector_3( fHalfExtentX, fHalfExtentY, fHalfExtentZ ), xPosition - GLToy_Vector_3( fHalfExtentX, fHalfExtentY, fHalfExtentZ ) )
    , m_xOrientation( xOrientation )
    {
    }

    GLToy_OBB( const GLToy_AABB& xAABB, const GLToy_Matrix_3& xOrientation )
    : GLToy_Parent( xAABB.GetPosition() )
    , m_xBox( xAABB )
    , m_xOrientation( xOrientation )
    {
    }

    void Render() const;

    // surface area and volume are invariant under rotation...
    float GetSurfaceArea() const
    {        
        return m_xBox.GetSurfaceArea();
    }

    float GetVolume() const
    {
        return m_xBox.GetVolume();
    }

    bool IsInside( const GLToy_Vector_3& xPosition ) const
    {
        GLToy_Vector_3 xTestPosition = xPosition - GetPosition();
        GLToy_Matrix_3 xInverse = GetOrientation();
        xInverse.InvertTransformationMatrix();
        xTestPosition *= xInverse;
        
        const GLToy_Vector_3 xHalfExtents = m_xBox.GetHalfExtents();
        return GLToy_AABB( xHalfExtents, -xHalfExtents ).IsInside( xTestPosition );
    }

    bool HasBoundingSphere() const { return true; }

    GLToy_Sphere GetBoundingSphere() const
    {
        return m_xBox.GetBoundingSphere();
    }

    void SetToPoint( const GLToy_Vector_3& xPosition )
    {
        m_xBox.SetToPoint( xPosition );
    }

    void GrowByPoint( const GLToy_Vector_3& xPosition )
    {
        // TODO - move and rotate the point, then move it back and call m_xBox.GrowByPoint
        // needs matrix multiplication...
    }

    void ReadFromBitStream( const GLToy_BitStream& xStream );
    void WriteToBitStream( GLToy_BitStream& xStream ) const;

    GLToy_ForceInline const GLToy_Matrix_3& GetOrientation() const { return m_xOrientation; }
    GLToy_ForceInline void SetOrientation( const GLToy_Matrix_3& xOrientation ) { m_xOrientation = xOrientation; }

    GLToy_ForceInline const GLToy_AABB& GetUnrotatedBB() const { return m_xBox; }

    GLToy_ForceInline GLToy_Vector_3 GetExtents() const { return m_xBox.GetExtents(); }
    GLToy_ForceInline GLToy_Vector_3 GetHalfExtents() const { return m_xBox.GetHalfExtents(); }

    bool IntersectsWithAABB( const GLToy_AABB& xAABB ) const;

protected:

    GLToy_AABB m_xBox;
    GLToy_Matrix_3 m_xOrientation;

};

template < class Derived >
void GLToy_Volume< Derived >::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    xStream >> m_xPosition;
}

template < class Derived >
void GLToy_Volume< Derived >::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    xStream << m_xPosition;
}

#endif
