#ifndef __GLTOY_PLANE_H_
#define __GLTOY_PLANE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/GLToy_Serialisable.h>

// GLToy
#include <Maths/GLToy_Vector.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Plane
: public GLToy_Serialisable
{
public:
    GLToy_Plane()
    : m_xNormal( 0.0f, 0.0f, 0.0f )
    , m_fDistance( 0.0f )
    {
    }

    GLToy_Plane( const GLToy_Vector_3& xNormal, const float fDistance )
    : m_xNormal( xNormal )
    , m_fDistance( fDistance )
    {
    }

    GLToy_Inline bool IsOnPositiveSide( const GLToy_Vector_3& xPosition ) const
    {
        return xPosition * m_xNormal > -m_fDistance;
    }

    GLToy_Inline double SignedDistance( const GLToy_Vector_3& xPosition ) const
    {
        return xPosition * m_xNormal + m_fDistance;
    }    

    virtual void ReadFromBitStream( const GLToy_BitStream& xStream );
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const;

    GLToy_Inline const GLToy_Vector_3& GetNormal() const { return m_xNormal; }
    GLToy_Inline float GetDistance() const { return m_fDistance; }

protected:

    GLToy_Vector_3 m_xNormal;
    float m_fDistance;
};

#endif