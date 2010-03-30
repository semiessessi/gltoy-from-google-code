/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of GLToy.
//
// GLToy is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// GLToy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with GLToy.
// If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////////////////

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
