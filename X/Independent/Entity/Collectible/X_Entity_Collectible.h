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

#ifndef __X_ENTITY_COLLECTIBLE_H_
#define __X_ENTITY_COLLECTIBLE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Entity/GLToy_Entity.h>

// GLToy
#include <Core/Data Structures/GLToy_Array.h>
#include <Core/GLToy_Hash.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D E F I N E S
/////////////////////////////////////////////////////////////////////////////////////////////

// TODO: Use correct hash
#define X_COLLECTIBLE_TYPE_LIFE   0x97154637 // GLToy_Hash_Constant( "Collect_Life" )
#define X_COLLECTIBLE_TYPE_WEAPON 0x72947394 // GLToy_Hash_Constant( "Collect_Weapon" )
#define X_COLLECTIBLE_TYPE_BOOST  0x33581769 // GLToy_Hash_Constant( "Collect_Boost" )

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class X_Entity_Collectible
: public GLToy_Entity_Sphere
{

    typedef GLToy_Entity_Sphere GLToy_Parent;

public:

    X_Entity_Collectible( const GLToy_Hash uHash, const u_int uType );
	virtual ~X_Entity_Collectible();

    virtual void Render() const;
	virtual void RenderDeferred() const;
	virtual void Update();

	void SetCollectType( GLToy_Hash xCollectType ) { m_xCollectType = xCollectType; }
	GLToy_Hash GetCollectType() const { return m_xCollectType; }

	static GLToy_Array< X_Entity_Collectible* >& GetList() { return s_xList; }
	
private:

    static GLToy_Array< X_Entity_Collectible* > s_xList;

	GLToy_Hash m_xCollectType;
};

#endif
