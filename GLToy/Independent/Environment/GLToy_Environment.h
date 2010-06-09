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
#include <String/GLToy_String.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Physics_Object;
class GLToy_Ray;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_EnvironmentLeaf
: public GLToy_Renderable
{
};

class GLToy_Environment
: protected GLToy_BSPTree< GLToy_EnvironmentLeaf >
, public GLToy_Renderable
, public GLToy_Serialisable
, public GLToy_Updateable
{

    friend class GLToy_Environment_System;

    typedef GLToy_BSPTree< GLToy_EnvironmentLeaf > GLToy_DataParent;

public:

    virtual void ReadFromBitStream( const GLToy_BitStream& xStream );
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const;
    
    virtual void Initialise() = 0;
    virtual void Shutdown() = 0;

    virtual int GetType() const = 0;

    virtual void Render() const;
    virtual void Update();

    // raytrace function - takes a ray and returns the intersection parameter or a negative value if there is no intersection
    virtual float Trace( const GLToy_Ray& xRay, const float fLimitingDistance = -1.0f ) const = 0;

protected:

    GLToy_Environment()
    : GLToy_BSPTree< GLToy_EnvironmentLeaf >()
    , m_pxPhysicsObject( NULL )
    {
    }

    GLToy_Physics_Object* m_pxPhysicsObject;
    
};

#endif
