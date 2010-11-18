/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
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

#ifndef __GLTOY_ENTITY_H_
#define __GLTOY_ENTITY_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/GLToy_Bounded.h>
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
, public GLToy_Updateable
{

    typedef GLToy_Bounded GLToy_Parent;

public:

    GLToy_Entity( const GLToy_Hash uHash, const u_int uType )
    : m_bActive( true )
    , m_bFlaggedForDestruction( false )
    , m_fHealth( GLToy_Maths::LargeFloat )
    , m_uType( uType )
    , m_uHash( uHash )
    {
    }

    virtual ~GLToy_Entity() {}

    // overrides
    virtual void Render() const {}
    virtual void Update() {}

    virtual void ReadFromBitStream( const GLToy_BitStream& xStream );
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const;

    virtual void SetKeyValuePair( const GLToy_String& szKey, const GLToy_String& szValue ) {}

    // own virtuals
    virtual void Trigger( const GLToy_Hash uTriggerHash ) { GLToy_Assert( false, "Entity 0x%X is trying to trigger a non-triggerable entity 0x%X", uTriggerHash, m_uHash ); }

    virtual void Activate() { m_bActive = true; }
    virtual void Deactivate() { m_bActive = false; }

    virtual void Hurt( const GLToy_Hash uTriggerHash, const float fAmount ) { m_fHealth -= fAmount; }
    virtual void Kill() { m_fHealth = 0.0f; }
    GLToy_Inline bool IsDead() const { return m_fHealth == 0.0f; }
    virtual void Destroy() { m_bFlaggedForDestruction = true; }

    virtual void RenderAABB() const {}
    virtual void RenderOBB() const {}

    // accessors
    GLToy_Inline GLToy_Hash GetHash() const { return m_uHash; }
    GLToy_Inline u_int GetType() const { return m_uType; }
    GLToy_Inline float GetHealth() const { return m_fHealth; }

    GLToy_Inline void SetHealth( const float fValue ) { m_fHealth = fValue; }

    GLToy_Inline bool IsActive() const { return m_bActive; }
    GLToy_Inline bool IsFlaggedForDestruction() const { return m_bFlaggedForDestruction; }

protected:

    bool m_bActive;
    bool m_bFlaggedForDestruction;
    float m_fHealth;
    u_int m_uType;
    GLToy_Hash m_uHash;

};

class GLToy_Entity_Null
: public GLToy_Bounded_AABB
, public GLToy_Entity
{

    typedef GLToy_Entity GLToy_Parent;

public:

    GLToy_Entity_Null( const GLToy_Hash uHash, const u_int uType )
    : GLToy_Parent( uHash, uType )
    , GLToy_Bounded_AABB()
    {
    }

    virtual ~GLToy_Entity_Null() {}

};

// TODO - GLToy_Entity_OrientedSphere/OBB - I'm sure they will be needed eventually
class GLToy_Entity_Oriented_AABB
: public GLToy_Bounded_AABB
, public GLToy_Entity
{

    typedef GLToy_Entity GLToy_Parent;

public:

    GLToy_Entity_Oriented_AABB( const GLToy_Hash uHash, const u_int uType )
    : GLToy_Parent( uHash, uType )
    , GLToy_Bounded_AABB()
    {
    }

    virtual ~GLToy_Entity_Oriented_AABB() {}
    
    virtual void ReadFromBitStream( const GLToy_BitStream& xStream );
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const;

    virtual void RenderAABB() const;
    virtual void RenderOBB() const;

    virtual const GLToy_Matrix_3& GetOrientation() const { return m_xOrientation; }
    virtual void SetOrientation( const GLToy_Matrix_3& xOrientation ) { m_xOrientation = xOrientation; }

protected:

    GLToy_Matrix_3 m_xOrientation;
};

// plain unoriented sphere - probably want AABB version of this too
class GLToy_Entity_Sphere
: public GLToy_Bounded_Sphere
, public GLToy_Entity
{

    typedef GLToy_Entity GLToy_Parent;

public:

    GLToy_Entity_Sphere( const GLToy_Hash uHash, const u_int uType )
    : GLToy_Parent( uHash, uType )
    , GLToy_Bounded_Sphere()
    {
    }

    virtual ~GLToy_Entity_Sphere() {}

    virtual void ReadFromBitStream( const GLToy_BitStream& xStream );
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const;

    virtual void RenderAABB() const { m_xBoundingSphere.GetBoundingAABB().Render(); }

};

#endif
