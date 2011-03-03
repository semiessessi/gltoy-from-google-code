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
		}

		GLToy_Hash m_uBrain;
		// TODO: Include other things like the texture etc...
};

class X_Entity_Enemy
: public GLToy_Entity_Sphere
{

    typedef GLToy_Entity_Sphere GLToy_Parent;

public:

    X_Entity_Enemy( const GLToy_Hash uHash, const u_int uType );
    virtual ~X_Entity_Enemy();

    virtual void Render() const;
    virtual void RenderDeferred() const;
	virtual void Update();

	const GLToy_Vector_2& GetDirection() { return m_xDirection; }
	void SetDirection( const GLToy_Vector_2& xDirection ) { m_xDirection = xDirection; }

	float GetSpeed() { return m_fSpeed; }
	void SetSpeed( float fSpeed ) { m_fSpeed = fSpeed; }

	void SetDefinition( const X_Enemy_Definition& xDefinition );

    static GLToy_Array< X_Entity_Enemy* >& GetList() { return s_xList; }

    GLToy_Vector_3 GetVelocity() const { return GLToy_Vector_3( m_xDirection * m_fSpeed, 0.0f ); }
	
	void TightenCollision( const GLToy_Hash uMaterialHash );

private:

    static GLToy_Array< X_Entity_Enemy* > s_xList;

	GLToy_Vector_2 m_xDirection;
	float m_fSpeed;
    GLToy_Hash m_uLight;

	X_Enemy_Definition m_xDefinition;

	X_Enemy_Brain* m_pxBrain;
};

#endif
