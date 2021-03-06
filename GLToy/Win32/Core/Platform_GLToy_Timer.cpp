/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2009, 2010 Semi Essessi
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
	SetThreadAffinityMask( GetCurrentThread(), 1 ); // make sure we only run this (main) thread on the first CPU - to avoid obscure bugs in CPU/BIOS etc
    QueryPerformanceCounter( &xPerformanceCount );

    return true;
}

float Platform_GLToy_Timer::GetTimeSinceLastGet()
{
    // this can change... so we had better update it
    QueryPerformanceFrequency( &xPerformanceFrequency );

    LARGE_INTEGER xNewPerformanceCount;
    QueryPerformanceCounter( &xNewPerformanceCount );

    float fTime = static_cast< float >( xNewPerformanceCount.QuadPart - xPerformanceCount.QuadPart )
        / static_cast< float >( xPerformanceFrequency.QuadPart );

    xPerformanceCount = xNewPerformanceCount;

    return fTime;
}
