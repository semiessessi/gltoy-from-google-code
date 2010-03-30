/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009, 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of FSDemo.
//
// FSDemo is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// FSDemo is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with FSDemo.
// If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __FSDEMO_FRIDGESCRIPT_H_
#define __FSDEMO_FRIDGESCRIPT_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <FridgeScript/Platform_FSDemo_FridgeScript.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class FSDemo_FridgeScript
{

public:

    ~FSDemo_FridgeScript();

    static bool Initialise();
    static void Shutdown();

    static FSDemo_FridgeScript* CreateFromFile( const char* const szFilename );
    static void RegisterAPI( char* szName, void* pfnCallback, u_int uParameterCount );

    void Execute();

protected:

    FSDemo_FridgeScript();

    void CompileFromFile( const char* const szFilename );

    u_int m_uCodeID;
    static u_int s_uContextID;

};

#endif