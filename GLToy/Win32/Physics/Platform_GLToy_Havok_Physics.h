/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __PLATFORM_GLTOY_HAVOK_PHYSICS_H_
#define __PLATFORM_GLTOY_HAVOK_PHYSICS_H_

#ifdef GLTOY_USE_HAVOK_PHYSICS

/////////////////////////////////////////////////////////////////////////////////////////////
// M A C R O S
/////////////////////////////////////////////////////////////////////////////////////////////

//#ifdef _DEBUG
#define HK_DEBUG
//#endif

#undef const_cast

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Common/Base/KeyCode.h>
#define HK_CLASSES_FILE <Common/Serialize/Classlist/hkKeyCodeClasses.h>
#include <Common/Serialize/Util/hkBuiltinTypeRegistry.cxx>

/////////////////////////////////////////////////////////////////////////////////////////////
// L I B R A R I E S
/////////////////////////////////////////////////////////////////////////////////////////////

// TODO - work out a way to do this properly so the release libraries are used...

#ifdef _DEBUG

#pragma comment( lib, "hkbase" )
#pragma comment( lib, "hkserialize" )
#pragma comment( lib, "hkcompat" )
#pragma comment( lib, "hkscenedata" )
#pragma comment( lib, "hkinternal" )
#pragma comment( lib, "hkgeometryutilities" )
#pragma comment( lib, "hkvisualize" )

#pragma comment( lib, "hkpconstraintsolver" )
#pragma comment( lib, "hkpcollide" )
#pragma comment( lib, "hkpdynamics" )
#pragma comment( lib, "hkpinternal" )
#pragma comment( lib, "hkputilities" )
#pragma comment( lib, "hkpvehicle" )

#pragma comment( lib, "hkaanimation" )
#pragma comment( lib, "hkainternal" )
#pragma comment( lib, "hkaragdoll" )

#else

#pragma comment( lib, "hkbase" )
#pragma comment( lib, "hkserialize" )
#pragma comment( lib, "hkcompat" )
#pragma comment( lib, "hkscenedata" )
#pragma comment( lib, "hkinternal" )
#pragma comment( lib, "hkgeometryutilities" )
#pragma comment( lib, "hkvisualize" )

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

#endif
