/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
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

#ifndef __GLTOY_STATE_SYSTEM_H_
#define __GLTOY_STATE_SYSTEM_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Core/GLToy_Hash.h>
#include <String/GLToy_String.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class T > class GLToy_Array;
template < class T > class GLToy_HashTree;
class GLToy_State;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_State_System
{

public:

    static bool Initialise();
    static void Shutdown();

    static void Render();
    static void Render2D();
    static void Update();

    static void RegisterState( GLToy_State* const pxState, const GLToy_Hash uStateHash );
    static bool ChangeState( const GLToy_Hash uStateHash );
    static void ChangeState_Console( const GLToy_String& szName );

    static void SetNextState( const GLToy_Hash uStateHash ) { s_uNextState = uStateHash; }

private:

    static GLToy_HashTree< GLToy_State* > s_xStates;
    static GLToy_State* s_pxCurrentState;
    static GLToy_Hash s_uNextState;

};

#endif
