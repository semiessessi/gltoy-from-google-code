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

#ifndef __X_ENTITY_PLAYER_H_
#define __X_ENTITY_PLAYER_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Entity/GLToy_Entity.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D E F I N E S
/////////////////////////////////////////////////////////////////////////////////////////////

#define X_PLAYER_GUN_TYPE_SINGLE 0x33333333 // TODO use correct hash
#define X_PLAYER_GUN_TYPE_TRIPLE 0x44444444
#define X_PLAYER_GUN_TYPE_FIVER  0x55555555

/////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations
/////////////////////////////////////////////////////////////////////////////////////////////

class X_Entity_Collectible;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class X_Entity_Player
: public GLToy_Entity_Sphere
{

    typedef GLToy_Entity_Sphere GLToy_Parent;

public:

    X_Entity_Player( const GLToy_Hash uHash, const u_int uType );
    virtual ~X_Entity_Player();

    virtual void Render() const;
	virtual void Update();

	GLToy_ForceInline void SetMovement( const GLToy_Vector_2& xMovement ) { m_xMovement = xMovement; }

    u_int GetLives() const { return m_uLives; }
	u_int GetGunType() const { return m_uGunType; }

	static GLToy_Array< X_Entity_Player* >& GetList() { return s_xList; }

protected:

	GLToy_Vector_2 m_xMovement;
    u_int m_uLives;
	u_int m_uGunType;
    
private:

	void Collect( const X_Entity_Collectible* pxCollectible );

    static GLToy_Array< X_Entity_Player* > s_xList;

};

#endif
