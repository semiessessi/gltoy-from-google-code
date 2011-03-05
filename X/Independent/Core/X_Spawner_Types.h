
// TY 18.2.11

#ifndef _X_SPAWNER_TYPES
#define _X_SPAWNER_TYPES 1

#include "X_Spawner.h"

enum SPAWNER_TYPES
{
	eENEMY_SPAWNER_FIRST_TYPE = 0,

	eENEMY_SPAWNER_INTERVAL = eENEMY_SPAWNER_FIRST_TYPE,
	eENEMY_SPAWNER_SCATTER,
    eENEMY_SPAWNER_SWEEP,

	eENEMY_SPAWNER_NUM_TYPES,
};

class X_Spawner_Interval : public X_Enemy_Spawner
{
	X_SPAWNER_TYPE( eENEMY_SPAWNER_INTERVAL );

	public:

		X_Spawner_Interval( const X_Enemy_Definition& xDefinition );

		void Update();

	protected:

		float m_fTimer;
};

class X_Spawner_Sweep : public X_Enemy_Spawner
{
	X_SPAWNER_TYPE( eENEMY_SPAWNER_SWEEP );

	public:

		X_Spawner_Sweep( const X_Enemy_Definition& xDefinition );

		void Update();

	protected:

		float m_fTimer;
		float m_fSpawnTimer;
		bool m_bSpawning;
		bool m_bReverse;
};

class X_Spawner_Scatter : public X_Enemy_Spawner
{
	X_SPAWNER_TYPE( eENEMY_SPAWNER_SCATTER );

	public:

		X_Spawner_Scatter( const X_Enemy_Definition& xDefinition );

		void Update();

	protected:

		float m_fTimer;
		float m_fSpawnTimer;
};

#endif

//eof
