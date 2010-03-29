#ifndef __GLTOY_ENTITY_MODELSTATIC_H_
#define __GLTOY_ENTITY_MODELSTATIC_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Entity/GLToy_Entity.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Model;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Entity_ModelStatic
: public GLToy_Entity_Oriented_AABB
{

    typedef GLToy_Entity_Oriented_AABB GLToy_Parent;

public:

    GLToy_Entity_ModelStatic( const GLToy_Hash uHash, const u_int uType )
    : GLToy_Parent( uHash, uType )
    , m_pxModel( NULL )
    , m_uModelHash( uGLTOY_BAD_HASH )
    {
    }

    virtual ~GLToy_Entity_ModelStatic() {}

    virtual void ReadFromBitStream( const GLToy_BitStream& xStream );
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const;

    virtual void Render() const;

    void SetModel( const GLToy_Hash uHash );
    void SetModel( const GLToy_String& szName );

protected:

    GLToy_Model* m_pxModel;
    GLToy_Hash m_uModelHash;

};

#endif