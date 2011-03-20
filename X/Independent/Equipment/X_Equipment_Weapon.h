
// Thomas Young 22.12.10

#ifndef _X_Weapon_
#define _X_Weapon_

#include "Core/GLToy.h"

#define X_WEAPON_TYPE(X) public: static const u_int uTYPE_HASH = X

#define X_WEAPON_MAX_BOOST 4

class X_Entity_Enemy;
class X_Entity_Player;

class X_Weapon
{
	public:

		X_Weapon();
		~X_Weapon();

		virtual void StartShooting();
		virtual void StopShooting();
		bool IsShooting() const { return m_bIsShooting; }

		virtual void Update();

	protected:

		float GetTimeShooting() { return m_fShootTimer; }

		X_Entity_Player* GetPlayerEntity();

	private:

		bool m_bIsShooting;
		float m_fShootTimer;
};

// _________________________________________________________________________________________
// _________________________________________________________________________________________

class X_Player_Weapon : public X_Weapon
{
	public:

		X_Player_Weapon();
		~X_Player_Weapon();

		virtual void Boost();

	protected:

		int GetBoost() const { return m_iBoost; }

	private:

		int m_iBoost;
};

// _________________________________________________________________________________________
// _________________________________________________________________________________________

class X_Enemy_Weapon : public X_Weapon
{
	public:

		X_Enemy_Weapon( GLToy_Hash uEnemyHash );
		~X_Enemy_Weapon();

	protected:

		X_Entity_Enemy* GetEntity();

	private:

		GLToy_Hash m_uEnemyHash;
};

// _________________________________________________________________________________________
// _________________________________________________________________________________________

class X_Weapon_Factory
{
	public:

		static X_Weapon* CreateWeapon( u_int uType, GLToy_Hash uEnemyHash = 0 );

		static X_Player_Weapon* CreatePlayerWeapon( u_int uType );
		static X_Enemy_Weapon* CreateEnemyWeapon( u_int uType, GLToy_Hash uEnemyHash );
};

#endif

//eof
