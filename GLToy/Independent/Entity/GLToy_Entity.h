#ifndef __GLTOY_ENTITY_H_
#define __GLTOY_ENTITY_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/GLToy_Bounded.h>
#include <Core/GLToy_Serialisable.h>
#include <Core/GLToy_Updateable.h>
#include <Render/GLToy_Renderable.h>

// GLToy
#include <Core/GLToy_Hash.h>
#include <Entity/GLToy_EntityTypes.h>
#include <Maths/GLToy_Maths.h>
#include <Maths/GLToy_Matrix.h>
#include <Maths/GLToy_Vector.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Entity
: public virtual GLToy_Bounded
, public GLToy_Renderable
, public GLToy_Serialisable
, public GLToy_Updateable
{

    typedef GLToy_Bounded GLToy_Parent;

public:

    GLToy_Entity( const GLToy_Hash uHash, const GLToy_EntityType eType )
    : GLToy_Parent()
    , m_bActive( false )
    , m_eType( eType )
    , m_uHash( uHash )
    {
    }

    virtual ~GLToy_Entity() {}

    // overrides
    virtual void Render() const {}
    virtual void Update() {}

    virtual void ReadFromBitStream( const GLToy_BitStream& xStream );
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const;

    // own virtuals
    virtual void Trigger( const GLToy_Hash uTriggerHash ) = 0;

    virtual void Activate() { m_bActive = true; }
    virtual void Deactivate() { m_bActive = false; }

    GLToy_Hash GetHash() const { return m_uHash; }
    GLToy_Inline GLToy_EntityType GetType() const { return m_eType; }

    bool IsActive() const { return m_bActive; }

protected:

    bool m_bActive;
    GLToy_EntityType m_eType;
    GLToy_Hash m_uHash;

};

class GLToy_Entity_Null
: public GLToy_Bounded_AABB
, public GLToy_Entity
{

    typedef GLToy_Entity GLToy_Parent;

public:

    GLToy_Entity_Null( const GLToy_Hash uHash, const GLToy_EntityType eType )
    : GLToy_Parent( uHash, eType )
    , GLToy_Bounded_AABB()
    {
    }

    virtual void Trigger( const GLToy_Hash uTriggerHash ) { GLToy_Assert( false, "Entity 0x%X is trying to trigger null entity 0x%X - everything is wrong here", uTriggerHash, m_uHash ); }

};

// TODO - GLToy_Entity_OrientedSphere - I'm sure it will be needed eventually
class GLToy_Entity_Oriented_BB
: public GLToy_Bounded_OBB
, public GLToy_Entity
{

    typedef GLToy_Entity GLToy_Parent;

public:

    GLToy_Entity_Oriented_BB( const GLToy_Hash uHash, const GLToy_EntityType eType )
    : GLToy_Parent( uHash, eType )
    , GLToy_Bounded_OBB()
    , m_xOrientation( GLToy_Maths::IdentityMatrix3 )
    {
    }

    virtual void Trigger( const GLToy_Hash uTriggerHash ) { GLToy_Assert( false, "Entity 0x%X is trying to trigger null entity 0x%X - everything is wrong here", uTriggerHash, m_uHash ); }

    GLToy_Inline const GLToy_Matrix_3& GetOrientation() const { return m_xOrientation; }
    GLToy_Inline void SetOrientation( const GLToy_Matrix_3& xOrientation ) { m_xOrientation = xOrientation; }

protected:

    GLToy_Matrix_3 m_xOrientation;
};

#endif