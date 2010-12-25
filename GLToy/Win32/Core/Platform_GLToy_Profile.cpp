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

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Core/GLToy_Profile.h>

// Win32
#include <windows.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

static LARGE_INTEGER xProfilePerformanceCount;
static LARGE_INTEGER xProfilePerformanceFrequency;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Profile::Platform_Initialise()
{
	SetThreadAffinityMask( GetCurrentThread(), 1 ); // make sure we only run this (main) thread on the first CPU - to avoid obscure bugs in CPU/BIOS etc
    QueryPerformanceCounter( &xProfilePerformanceCount );

    return true;
}

float GLToy_Profile::Platform_GetTimeSinceLastGet()
{
    // this can change... so we had better update it
    QueryPerformanceFrequency( &xProfilePerformanceFrequency );

    LARGE_INTEGER xNewPerformanceCount;
    QueryPerformanceCounter( &xNewPerformanceCount );

    float fTime = static_cast< float >( xNewPerformanceCount.QuadPart - xProfilePerformanceCount.QuadPart )
        / static_cast< float >( xProfilePerformanceFrequency.QuadPart );

    xProfilePerformanceCount = xNewPerformanceCount;

    return fTime;
}
