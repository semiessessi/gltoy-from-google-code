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