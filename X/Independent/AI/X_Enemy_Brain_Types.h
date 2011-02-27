
#include "Core\GLToy_Hash.h"
#include "X_Enemy_Brain.h"

enum BRAIN_TYPES
{
	eENEMY_BRAIN_FIRST_TYPE = 0,

	eENEMY_BRAIN_SUICIDE = eENEMY_BRAIN_FIRST_TYPE,
	eENEMY_BRAIN_SWARM,
	eENEMY_BRAIN_DIVE,

	eENEMY_BRAIN_NUM_TYPES,
};

class X_Enemy_Brain_Suicide : public X_Enemy_Brain
{
	X_BRAIN_TYPE( eENEMY_BRAIN_SUICIDE );

	public:

		X_Enemy_Brain_Suicide( GLToy_Hash uEnemy );
		~X_Enemy_Brain_Suicide();

		virtual void Update();

};

class X_Enemy_Brain_Swarm : public X_Enemy_Brain
{
	X_BRAIN_TYPE( eENEMY_BRAIN_SWARM );

	public:

		X_Enemy_Brain_Swarm( GLToy_Hash uEnemy );
		~X_Enemy_Brain_Swarm();

		virtual void Update();

	protected:

		GLToy_Hash m_uCurrentTarget;

};

class X_Enemy_Brain_Dive : public X_Enemy_Brain
{
	X_BRAIN_TYPE( eENEMY_BRAIN_DIVE );

	public:

		X_Enemy_Brain_Dive( GLToy_Hash uEnemy );
		~X_Enemy_Brain_Dive();

		virtual void Update();
};

// eof
