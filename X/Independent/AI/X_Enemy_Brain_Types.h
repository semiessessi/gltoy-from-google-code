
#include "Core\GLToy_Hash.h"
#include "X_Enemy_Brain.h"

// TODO: GLToy_Hash_Constant does not represent an actual constant so cannot be used in this way
static const GLToy_Hash xENEMY_BRAIN_SUICIDE = 01234; // GLToy_Hash_Constant( "X_Brain_Suicide" );
static const GLToy_Hash xENEMY_BRAIN_SWARM = 11234;

class X_Enemy_Brain_Suicide : public X_Enemy_Brain
{
	X_BRAIN_TYPE( xENEMY_BRAIN_SUICIDE );

	public:

		X_Enemy_Brain_Suicide( GLToy_Hash uEnemy );
		~X_Enemy_Brain_Suicide();

		virtual void Update();

};

class X_Enemy_Brain_Swarm : public X_Enemy_Brain
{
	X_BRAIN_TYPE( xENEMY_BRAIN_SWARM );

	public:

		X_Enemy_Brain_Swarm( GLToy_Hash uEnemy );
		~X_Enemy_Brain_Swarm();

		virtual void Update();

};


