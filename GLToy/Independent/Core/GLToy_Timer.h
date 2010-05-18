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

#ifndef __GLTOY_TIMER_H_
#define __GLTOY_TIMER_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Timer
{

    friend class Platform_GLToy_Timer;

public:
    
    static bool Initialise();

    static void Update();

    GLToy_Inline static float GetTime() { return s_fTimer; }
    GLToy_Inline static float GetFrameTime() { return s_fFrameTime; }
    GLToy_Inline static float GetFrameRate() { return 1 / s_fFrameTime; }
    GLToy_Inline static float GetSmoothedFrameRate() { return s_fSmoothedFrameRate; }

private:
    
    static bool Platform_Initialise();

    static float Platform_GetTimeSinceLastGet();

    static float s_fTimer;
    static float s_fFrameTime;
    static float s_fSmoothedFrameRate;
};

#endif
