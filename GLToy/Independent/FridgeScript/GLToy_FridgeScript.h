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

#ifdef GLTOY_USE_FRIDGESCRIPT

#ifndef __GLTOY_FRIDGESCRIPT_H_
#define __GLTOY_FRIDGESCRIPT_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <FridgeScript/Platform_GLToy_FridgeScript.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_FridgeScript
{

public:

    ~GLToy_FridgeScript();

    static bool Initialise();
    static void Shutdown();

    static GLToy_FridgeScript* CreateFromFile( const char* const szFilename );
    static void RegisterAPI( char* szName, void* pfnCallback, u_int uParameterCount );

    void Execute();

protected:

    GLToy_FridgeScript();

    void CompileFromFile( const char* const szFilename );

    u_int m_uCodeID;
    static u_int s_uContextID;

};

#endif

#endif
