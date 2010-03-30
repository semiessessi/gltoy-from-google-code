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

#ifndef __GLTOY_ENTITY_MODELANIMATED_H_
#define __GLTOY_ENTITY_MODELANIMATED_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Entity/Model/GLToy_Entity_ModelStatic.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_AnimationStack;
class GLToy_Model;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Entity_ModelAnimated
: public GLToy_Entity_ModelStatic
{

    typedef GLToy_Entity_ModelStatic GLToy_Parent;

public:

    GLToy_Entity_ModelAnimated( const GLToy_Hash uHash, const u_int uType );
    virtual ~GLToy_Entity_ModelAnimated();

    virtual void Render() const;
    virtual void Update();

    void PlayAnimation( const u_int uAnimID = 0, const float fTweenInTime = 0.0f, const float fTweenOutTime = 0.0f );

protected:

    GLToy_AnimationStack* m_pxAnimStack;

};

#endif