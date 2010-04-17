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

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Particle/GLToy_PFX_System.h>

// GLToy
#include <Core/GLToy_UpdateFunctor.h>
#include <Particle/GLToy_ParticleSource.h>
#include <Particle/GLToy_PFX.h>
#include <Render/GLToy_RenderFunctor.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_List< GLToy_PFX* > GLToy_PFX_System::s_xPFX;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_PFX_System::Initialise()
{
    return true;
}

void GLToy_PFX_System::Shutdown()
{
    s_xPFX.DeleteAll();
}

void GLToy_PFX_System::Render()
{
    s_xPFX.Traverse( GLToy_PointerRenderFunctor< GLToy_PFX* >() );
}

void GLToy_PFX_System::Update()
{
    s_xPFX.Traverse( GLToy_PointerUpdateFunctor< GLToy_PFX* >() );
}

u_int GLToy_PFX_System::CreatePFX( const GLToy_Hash uHash )
{
    return uGLTOY_BAD_PFXID;
}