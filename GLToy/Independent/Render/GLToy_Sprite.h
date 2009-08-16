#ifndef __GLTOY_SPRITE_H_
#define __GLTOY_SPRITE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parent
#include <Render/GLToy_Renderable.h>

// GLToy
#include <Maths/GLToy_Vector.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Sprite
{

public:

	GLToy_Sprite();
	virtual ~GLToy_Sprite();

	virtual void SetPosition();

	virtual void Initialise();
	virtual void Shutdown();

	virtual void Render();

protected:

	GLToy_Vector_3 m_xPosition;
	GLToy_Hash m_uTextureID;

};

#endif
