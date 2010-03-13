#ifndef __RTTOY_ENVIRONMENT_PLANE_H_
#define __RTTOY_ENVIRONMENT_PLANE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parent
#include <Environment/GLToy_Environment.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class RTToy_Environment_Plane
: public GLToy_Environment
{

    typedef GLToy_Environment GLToy_Parent;

public:

    RTToy_Environment_Plane( const GLToy_Plane& xPlane )
    : GLToy_Parent()
    , m_xPlane( xPlane )
    {
    }

    virtual void ReadFromBitStream( const GLToy_BitStream& xStream );
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const;
    
    virtual void Render() const;
    virtual void Update();

protected:

    GLToy_Plane m_xPlane;

};

#endif