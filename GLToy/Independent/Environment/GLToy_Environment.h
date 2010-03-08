#ifndef __GLTOY_ENVIRONMENT_H_
#define __GLTOY_ENVIRONMENT_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/Data Structures/GLToy_BSPTree.h>
#include <Core/GLToy_Serialisable.h>
#include <Core/GLToy_Updateable.h>
#include <Model/GLToy_Model.h>
#include <Render/GLToy_Renderable.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_EnvironmentStrip
: public GLToy_ModelStrip
{

    typedef GLToy_ModelStrip GLToy_Parent;

public:

    virtual void ReadFromBitStream( const GLToy_BitStream& xStream );
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const;

protected:

    union
    {
        struct
        {
            u_int m_bPortal             : 1;
            u_int m_bTransparent        : 1;
            u_int m_bInvisible          : 1;
            u_int m_bCollidable         : 1;
        };

        u_int m_uFlags;
    };

};

class GLToy_EnvironmentLeaf
: public GLToy_Model
{

    typedef GLToy_Model GLToy_Parent;

public:

    GLToy_EnvironmentLeaf() : GLToy_Parent() {}

protected:

};

class GLToy_Environment
: protected GLToy_BSPTree< GLToy_EnvironmentLeaf >
, public GLToy_Renderable
, public GLToy_Serialisable
, public GLToy_Updateable
{

    friend class GLToy_Environment_System;

public:

    virtual void ReadFromBitStream( const GLToy_BitStream& xStream );
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const;
    
    virtual void Render() const;
    virtual void Update();

protected:

    GLToy_Environment()
    : GLToy_BSPTree< GLToy_EnvironmentLeaf >()
    {
    }
    
};

#endif