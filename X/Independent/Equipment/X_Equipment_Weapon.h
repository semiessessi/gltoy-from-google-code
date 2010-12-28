
// Thomas Young 22.12.10

#ifndef _X_EQUIPMENT_WEAPON_
#define _X_EQUIPMENT_WEAPON_

#include <Maths/GLToy_Vector.h>

class X_Equipment_Weapon
{
	public:

		X_Equipment_Weapon();
		~X_Equipment_Weapon();

		void RandomGenerate( float fDamageMax );
		void Boost( float fBoost );

		float GetDamage();
		float GetSpeed();
		float GetRate();
		float GetSize();
		float GetSpread();
		int GetNumProjectiles();
		bool IsWavey();
		

	private:

		GLToy_Vector_2 m_xDamage; // Min and max damage per projectile
		GLToy_Vector_2 m_xSpeed;  // Min and max speed per projectile
		GLToy_Vector_2 m_xRate;   // Min and max rate of fire
		float m_fSize;            // Projectile size
		float m_fSpread;          // Max Spread from centre (radians)
		int m_iProjectiles;       // Number of projectiles created per shot
		bool m_bWavey;            // Use a sinusoidal firing pattern instead of random
		
		float m_fBoost;
};

#endif

//eof
