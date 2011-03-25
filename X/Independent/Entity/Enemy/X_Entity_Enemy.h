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

#ifndef __X_ENTITY_ENEMY_H_
#define __X_ENTITY_ENEMY_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Entity/GLToy_Entity.h>

// GLToy
#include <Core/Data Structures/GLToy_Array.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations
/////////////////////////////////////////////////////////////////////////////////////////////

class X_Enemy_Brain;
class X_Weapon;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class X_Enemy_Definition
{
	public:

		X_Enemy_Definition()
		{
			// TODO: Use some sensible defaults
			m_uBrain = 0;
			m_uMaterial = 0;
			m_uWeapon = 0;
			m_fSize = 0.0f;
			m_fSpeed = 0.0f;
			m_fScore = 0.0f;
			m_fKnockBack = 0.0f;
		}

		GLToy_Hash m_uBrain;
		GLToy_Hash m_uMaterial;
		GLToy_Hash m_uWeapon;
		float m_fSize;
		float m_fHealth;
		float m_fSpeed;
		float m_fScore;
		float m_fKnockBack;
		// TODO: Include other things 
};

class X_Entity_Enemy
: public GLToy_Entity_AABB
{

    typedef GLToy_Entity_AABB GLToy_Parent;

public:

    X_Entity_Enemy( const GLToy_Hash uHash, const u_int uType );
    virtual ~X_Entity_Enemy();

    virtual void Render() const;
    virtual void RenderDeferred() const;
	virtual void Update();

	const GLToy_Vector_2& GetDirection() { return m_xDirection; }
	void SetDirection( const GLToy_Vector_2& xDirection ) { m_xDirection = xDirection; }
	void AddImpulse( const GLToy_Vector_2& xDirection ) { m_xImpulse = xDirection + m_xImpulse; }

	void SetDefinition( const X_Enemy_Definition& xDefinition );

    static GLToy_Array< X_Entity_Enemy* >& GetList() { return s_xList; }

    GLToy_Vector_3 GetVelocity() const;

private:

    static GLToy_Array< X_Entity_Enemy* > s_xList;

	GLToy_Vector_2 m_xDirection;
	GLToy_Vector_2 m_xImpulse;
    GLToy_Hash m_uLight;

	X_Enemy_Definition m_xDefinition;

	X_Enemy_Brain* m_pxBrain;
	X_Weapon* m_pxWeapon;
};

#endif
