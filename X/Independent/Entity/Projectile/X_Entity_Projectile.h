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

#ifndef __X_ENTITY_PROJECTILE_H_
#define __X_ENTITY_PROJECTILE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Entity/GLToy_Entity.h>

// GLToy
#include <Core/Data Structures/GLToy_Array.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D E F I N E S 
/////////////////////////////////////////////////////////////////////////////////////////////

static const u_int uX_ENTITY_PROJECTILE_NUM_TEXTURES = 5;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class X_Entity_Projectile
: public GLToy_Entity_AABB
{

    typedef GLToy_Entity_AABB GLToy_Parent;

public:

	X_Entity_Projectile( const GLToy_Hash uHash, const u_int uType );
    virtual ~X_Entity_Projectile();

    virtual void Render() const;
    virtual void RenderTransparent() const;
	virtual void Update();

	void SetIsFromPlayer( bool bFromPlayer ) { m_bFromPlayer = bFromPlayer; }
	bool IsFromPlayer() { return m_bFromPlayer; }

	void SetDirection( const GLToy_Vector_3& xDirection ) { m_xDirection = xDirection; }
	void SetTexture( const u_int uIndex );
	void SetSpeed( float fSpeed ) { m_fSpeed = fSpeed; }

	static GLToy_Array< X_Entity_Projectile* >& GetList() { return s_xList; }

private:

	static GLToy_Array< X_Entity_Projectile* > s_xList;

	GLToy_Vector_3 m_xDirection;
	u_int m_uTexture;
    GLToy_Hash m_uLight;
	bool m_bFromPlayer;
	float m_fSpeed;
};

#endif
