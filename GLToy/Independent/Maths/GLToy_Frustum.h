#ifndef __GLTOY_FRUSTUM_H_
#define __GLTOY_FRUSTUM_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Maths/GLToy_Volume.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Frustum
: public GLToy_Volume
{

    typedef GLToy_Volume GLToy_Parent;

public:

    virtual float GetSurfaceArea() const
    {
        return 0.0f;
    }

    virtual float GetVolume() const
    {
        return 0.0f;
    }

    virtual bool IsInside( const GLToy_Vector_3& xPosition ) const
    {
        return true;
    }

    virtual void SetToPoint( const GLToy_Vector_3& xPosition ) {}
    virtual void GrowByPoint( const GLToy_Vector_3& xPosition ) {}

    virtual void ReadFromBitStream( const GLToy_BitStream& xStream );
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const;

protected:

};

#endif