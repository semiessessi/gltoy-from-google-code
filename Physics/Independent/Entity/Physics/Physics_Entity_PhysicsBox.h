/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of Physics.
//
// Physics is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// Physics is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with Physics.
// If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////////////////

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

class Physics_Physics_Object;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class Physics_Entity_PhysicsBox
: public GLToy_Entity_Oriented_AABB // TODO - need that GLToy_Entity_Oriented_OBB really...
{

    typedef GLToy_Entity_Oriented_AABB GLToy_Parent;

public:

    Physics_Entity_PhysicsBox( const GLToy_Hash uHash, const u_int uType );
    virtual ~Physics_Entity_PhysicsBox() {}

    void Spawn( const GLToy_AABB& xBB, const GLToy_Vector_3& xVelocity );

    virtual void Render() const;

protected:

    Physics_Physics_Object* m_pxPhysicsObject;

};

#endif