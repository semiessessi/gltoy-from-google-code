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
#include <Core/State/GLToy_State_Splash.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Render/GLToy_Camera.h>
#include <Environment/GLToy_Environment_System.h>
#include <Entity/GLToy_Entity_System.h>
#include <UI/GLToy_UI_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

const GLToy_String szGLTOY_STRING = "GLToy is LGPL software, you should have recieved a copy of the source code and license with this software, if not, visit http://code.google.com/p/gltoy";
const GLToy_String szHAVOK_STRING = "GLToy uses Havok®.  ©Copyright 1999-2008 Havok.com Inc. (and its Licensors). All Rights Reserved.  See www.havok.com for details.";
const GLToy_String szOPENGL_STRING = "OpenGL® and the oval logo are trademarks or registered trademarks of Silicon Graphics, Inc. in the United States and/or other countries worldwide.";

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_State_Splash::Initialise()
{
    GLToy_UI_System::ShowPointer( false );
    GLToy_Camera::SetFlyCamEnabled( false );

    GLToy_Entity_System::SetRender( false );
    GLToy_Environment_System::SetRender( false );

    m_fTimer = 0.0f;
}

void GLToy_State_Splash::Update()
{
    m_fTimer += GLToy_Timer::GetFrameTime();
}

bool GLToy_State_Splash::IsDone() const
{
    return m_fTimer > 2.0f;
}
