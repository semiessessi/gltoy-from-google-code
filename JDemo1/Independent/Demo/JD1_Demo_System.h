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

#ifndef __JD1_DEMO_SYSTEM_H_
#define __JD1_DEMO_SYSTEM_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class T > class GLToy_List;
class JD1_DemoScene;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

enum JD1_Demo_Transition
{
	JD1_DEMO_CUT
};

class JD1_Demo_System
{

public:

    static bool Initialise();
	static void Shutdown();

    static void Render();
	static void Update();

    static float GetDemoTimer() { return s_fTimer; }
    static void Queue( JD1_DemoScene* const pxScene, const float fRunTime = -1.0f, const JD1_Demo_Transition eTransition = JD1_DEMO_CUT, const float fTransitionTime = 0.0f );

private:

    struct JD1_DemoQueueItem
    {

        JD1_DemoScene* m_pxScene;
        float m_fRunTime;
        JD1_Demo_Transition m_eTransition;
        float m_fTransitionTime;

    };

    static float s_fTimer;
    static GLToy_List< JD1_DemoQueueItem > s_xQueue;

};

#endif