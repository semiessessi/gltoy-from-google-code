/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009, 2010 Semi Essessi
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

// This file's headers
#include <Core/GLToy_Timer.h>
#include <Core/Platform_GLToy_Timer.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

float GLToy_Timer::s_fTimer = 0.0f;
float GLToy_Timer::s_fFrameTime = 0.0f;
float GLToy_Timer::s_fSmoothedFrameRate = 0.0f;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Timer::Initialise()
{
    return Platform_Initialise();
}

void GLToy_Timer::Update()
{
    s_fFrameTime = Platform_GetTimeSinceLastGet();

    s_fSmoothedFrameRate = 0.1f * GetFrameRate() + 0.9f * s_fSmoothedFrameRate;
    
    s_fTimer += s_fFrameTime;
}

bool GLToy_Timer::Platform_Initialise()
{
    return Platform_GLToy_Timer::Initialise();
}

float GLToy_Timer::Platform_GetTimeSinceLastGet()
{
    return Platform_GLToy_Timer::GetTimeSinceLastGet();
}
