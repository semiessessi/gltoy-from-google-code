/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010, 2011 Semi Essessi
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

class GLToy_Physics_Object;

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
    , m_pxPhysicsObject( NULL )
    , m_pxAI( NULL )
    {
    }

    virtual void Render() const;
    virtual void Update();
    virtual void Spawn( const GLToy_Vector_3& xPosition, const GLToy_Matrix_3& xOrientation );

    virtual bool HasSpawned() const { return m_pxPhysicsObject != NULL; }


protected:

    GLToy_Physics_Object* m_pxPhysicsObject;
    FPSToy_AI* m_pxAI;

};

#endif
