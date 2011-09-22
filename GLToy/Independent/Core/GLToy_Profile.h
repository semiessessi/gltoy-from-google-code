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

#ifndef __GLTOY_PROFILE_H_
#define __GLTOY_PROFILE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Profile
{

public:
    
    static bool Initialise() { return Platform_Initialise(); }

    static void StartProfileTimer() { Platform_GetTimeSinceLastGet(); }
    static float EndProfileTimer() { return Platform_GetTimeSinceLastGet(); }
    static float GetProfileTimer() { return Platform_GetProfileTimer(); }

private:

    static bool Platform_Initialise();

    static float Platform_GetProfileTimer();
    static float Platform_GetTimeSinceLastGet();

};

#ifdef GLTOY_FINAL
#define GLToy_Profile_StartTimer( xName )
#define GLToy_Profile_EndTimer( xName ) ( 0.0f )
#define GLToy_GetProfileTimer() ( 0.0f )
#else
#define GLToy_Profile_StartTimer( xName ) const float fStartTime_##xName = GLToy_Profile::GetProfileTimer()
#define GLToy_Profile_EndTimer( xName ) ( GLToy_Profile::GetProfileTimer() - fStartTime_##xName ); 
#define GLToy_GetProfileTimer() ( GLToy_Profile::GetProfileTimer() )
#endif

#endif
