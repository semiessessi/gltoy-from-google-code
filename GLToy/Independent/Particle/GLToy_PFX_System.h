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

#ifndef __GLTOY_PFX_SYSTEM_H_
#define __GLTOY_PFX_SYSTEM_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

template< class T > class GLToy_HashTree;
template< class T > class GLToy_List;
class GLToy_PFX;
class GLToy_PFXProperties;
struct GLToy_ParticleProperties;
struct GLToy_ParticleSourceProperties;

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

const u_int uGLTOY_BAD_PFXID = 0xFFFFFFFF;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_PFX_System
{

public:

    static bool Initialise();
    static void Shutdown();

    static void Render();
    static void Update();

    static u_int CreatePFX( const GLToy_Hash uHash );

private:

    static bool InitialisePFXProperties();
    static bool InitialiseSourceProperties();
    static bool InitialiseParticleProperties();

    static GLToy_List< GLToy_PFX* > s_xPFX;
    static GLToy_HashTree< GLToy_PFXProperties > s_xPFXProperties;
    static GLToy_HashTree< GLToy_ParticleSourceProperties > s_xSourceProperties;
    static GLToy_HashTree< GLToy_ParticleProperties > s_xParticleProperties;

};

#endif