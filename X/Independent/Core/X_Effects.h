
// TY 26.03.11

#ifndef _X_EFFECT_SYSTEM_H_
#define _X_EFFECT_SYSTEM_H_

#include "Core/Data Structures/GLToy_array.h"
#include "Maths/GLToy_Maths.h"

class X_Effect;

class X_Effect_System
{
	public:

		static void Initialise();
		static void Update();
		static void Render();
		static void Shutdown();

		static void CreateShockwave( const GLToy_Vector_3& xPosition, const float fSize );
		static void CreateExplosion( const GLToy_Vector_3& xPosition, const float fSize );

	private:

		static GLToy_Array< X_Effect* > s_xEffects;
};

#endif

//eof
