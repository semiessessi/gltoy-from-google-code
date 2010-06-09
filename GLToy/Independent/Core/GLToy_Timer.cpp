/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009, 2010 Semi Essessi
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
