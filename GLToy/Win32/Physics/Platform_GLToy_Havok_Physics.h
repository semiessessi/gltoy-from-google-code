#ifdef GLTOY_USE_HAVOK_PHYSICS

#ifndef __PLATFORM_GLTOY_HAVOK_PHYSICS_H_
#define __PLATFORM_GLTOY_HAVOK_PHYSICS_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// M A C R O S
/////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define HK_DEBUG
#endif

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Common/Base/KeyCode.h>
#define HK_CLASSES_FILE <Common/Serialize/Classlist/hkKeyCodeClasses.h>
#include <Common/Serialize/Util/hkBuiltinTypeRegistry.cxx>

/////////////////////////////////////////////////////////////////////////////////////////////
// L I B R A R I E S
/////////////////////////////////////////////////////////////////////////////////////////////

#pragma comment( lib, "hkbase" )
#pragma comment( lib, "hkserialize" )
#pragma comment( lib, "hkcompat" )
#pragma comment( lib, "hkscenedata" )
#pragma comment( lib, "hkinternal" )
#pragma comment( lib, "hkgeometryutilities" )

#pragma comment( lib, "hkpconstraintsolver" )
#pragma comment( lib, "hkpcollide" )
#pragma comment( lib, "hkpdynamics" )
#pragma comment( lib, "hkpinternal" )
#pragma comment( lib, "hkputilities" )
#pragma comment( lib, "hkpvehicle" )

#pragma comment( lib, "hkaanimation" )
#pragma comment( lib, "hkainternal" )
#pragma comment( lib, "hkaragdoll" )

#endif

#endif
