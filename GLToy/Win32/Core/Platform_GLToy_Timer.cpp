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
#include <Core/Platform_GLToy_Timer.h>

// Win32
#include <windows.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

static LARGE_INTEGER xPerformanceCount;
static LARGE_INTEGER xPerformanceFrequency;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////


bool Platform_GLToy_Timer::Initialise()
{
    QueryPerformanceCounter( &xPerformanceCount );

    return true;
}

float Platform_GLToy_Timer::GetTimeSinceLastGet()
{
    // this can change... so we had better update it
    QueryPerformanceFrequency( &xPerformanceFrequency );

    LARGE_INTEGER xNewPerformanceCount;
    QueryPerformanceCounter( &xNewPerformanceCount );

    float fTime = static_cast<float>( xNewPerformanceCount.QuadPart - xPerformanceCount.QuadPart )
        / static_cast<float>( xPerformanceFrequency.QuadPart );

    xPerformanceCount = xNewPerformanceCount;

    return fTime;
}