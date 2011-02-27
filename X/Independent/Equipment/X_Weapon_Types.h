
// Thomas Young 22.12.10

#ifndef _X_WEAPON_TYPES_
#define _X_WEAPON_TYPES_

#include "Core/GLToy.h"

#include "X_Equipment_Weapon.h"

//class X_Entity_Enemy;
//class X_Entity_Player;

enum PLAYER_WEAPON_TYPES
{
	ePLAYER_FIRST_WEAPON_TYPE = 0,
	
	ePLAYER_WEAPON_VULCAN = ePLAYER_FIRST_WEAPON_TYPE
};

// ______________________________  Player Weapon Types  ____________________________________
// _________________________________________________________________________________________


class X_Player_Weapon_Vulcan : public X_Player_Weapon
{
	X_BRAIN_TYPE( ePLAYER_WEAPON_VULCAN );

	public:

		X_Player_Weapon_Vulcan();
		~X_Player_Weapon_Vulcan();

		virtual void Update();

		virtual void StartShooting();

	private:

		float m_fShootTimer;
};

// ______________________________  Enemy Weapon Types  _____________________________________
// _________________________________________________________________________________________



#endif

//eof
