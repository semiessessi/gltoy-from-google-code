
// TY 18.2.11

#ifndef _X_SPAWNER_TYPES
#define _X_SPAWNER_TYPES 1

#include "X_Spawner.h"

// TODO: GLToy_Hash_Constant does not represent an actual constant so cannot be used in this way
static const GLToy_Hash xENEMY_SPAWNER_INTERVAL = 01234; // GLToy_Hash_Constant( "X_Spawner_Interval" );

class X_Spawner_Interval : public X_Enemy_Spawner
{
	X_SPAWNER_TYPE( xENEMY_SPAWNER_INTERVAL );

	public:

		X_Spawner_Interval( const X_Enemy_Definition& xDefinition );

		void Update();

	protected:

		float m_fTimer;
};

#endif

//eof
