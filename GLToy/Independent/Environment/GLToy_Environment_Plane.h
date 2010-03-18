#ifndef __GLTOY_ENVIRONMENT_PLANE_H_
#define __GLTOY_ENVIRONMENT_PLANE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parent
#include <Environment/GLToy_Environment.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Environment_Plane
: public GLToy_Environment
{

    typedef GLToy_Environment GLToy_Parent;

public:

    GLToy_Environment_Plane( const GLToy_Plane& xPlane, const GLToy_String& szTextureName )
    : GLToy_Parent()
    , m_xPlane( xPlane )
    , m_uTextureHash( szTextureName.GetHash() )
    {
    }

    virtual void ReadFromBitStream( const GLToy_BitStream& xStream );
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const;

    virtual void Initialise();
    virtual void Shutdown();    
    
    virtual void Render() const;
    virtual void Update();

    virtual int GetType() const;

protected:

    GLToy_Plane m_xPlane;
    GLToy_Hash m_uTextureHash;

};

#endif