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
    GLToy_Inline static float GetFrameTimer() { return s_fFrameTime; }
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