/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of GLToy.
//
// GLToy is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// GLToy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with GLToy.
// If not, see <http://www.gnu.org/licenses/>.
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
    {
    }
    
};

#endif