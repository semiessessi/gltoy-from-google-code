/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of FPSToy.
//
// FPSToy is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// FPSToy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with FPSToy.
// If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __FPSTOY_ENTITY_ACTOR_H_
#define __FPSTOY_ENTITY_ACTOR_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parent
#include <Entity/Model/GLToy_Entity_ModelAnimated.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Physics_Controller;

class FPSToy_AI;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class FPSToy_Entity_Actor
: public GLToy_Entity_ModelAnimated
{

    typedef GLToy_Entity_ModelAnimated GLToy_Parent;

public:

    FPSToy_Entity_Actor( const GLToy_Hash uHash, const u_int uType )
    : GLToy_Parent( uHash, uType )
    , m_pxPhysicsController( NULL )
    , m_pxAI( NULL )
    {
    }

    virtual void Render() const;
    virtual void Update();
    virtual void Spawn( const GLToy_Vector_3& xPosition, const GLToy_Matrix_3& xOrientation );

    virtual bool IsPlayer() { return false; }    
    virtual bool HasSpawned() const { return m_pxPhysicsController != NULL; }


protected:

    GLToy_Physics_Controller* m_pxPhysicsController;
    FPSToy_AI* m_pxAI;

};

#endif