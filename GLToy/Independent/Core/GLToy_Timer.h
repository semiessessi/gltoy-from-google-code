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

#ifndef __GLTOY_TIMER_H_
#define __GLTOY_TIMER_H_

#include "Maths/GLToy_Maths.h"

static const float fGLTOY_MAX_FRAME_TIME = 0.5f;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Timer
{

    friend class Platform_GLToy_Timer;

public:
    
    static bool Initialise();

    static void Update();

    static GLToy_ForceInline const float& GetTime() { return s_fTimer; }
	#ifdef _DEBUG
	static GLToy_ForceInline float GetFrameTime() { return GLToy_Maths::Min( s_fFrameTime, fGLTOY_MAX_FRAME_TIME ); }
	#else
	static GLToy_ForceInline float GetFrameTime() { s_fFrameTime; }
	#endif
    static GLToy_ForceInline float GetFrameRate() { return 1 / s_fFrameTime; }
    static GLToy_ForceInline float GetSmoothedFrameRate() { return s_fSmoothedFrameRate; }

private:
    
    static bool Platform_Initialise();

    static float Platform_GetTimeSinceLastGet();

    static float s_fTimer;
    static float s_fFrameTime;
    static float s_fSmoothedFrameRate;
};

#endif
