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

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/JD1.h>

// This file's headers
#include <Demo/JD1_Demo_System.h>

// GLToy
#include <Core/Data Structures/GLToy_List.h>
#include <Core/GLToy_Timer.h>

// JD1
#include <Demo/JD1_DemoScene.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

float JD1_Demo_System::s_fTimer = 0.0f;
GLToy_List< JD1_Demo_System::JD1_DemoQueueItem > JD1_Demo_System::s_xQueue;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool JD1_Demo_System::Initialise()
{
    s_xQueue.DeleteAll();
    s_fTimer = 0.0f;

    return true;
}

void JD1_Demo_System::Shutdown()
{
    s_xQueue.DeleteAll();
}

void JD1_Demo_System::Render()
{
    if( s_xQueue.IsEmpty() )
    {
        return;
    }

    JD1_DemoQueueItem& xQueueItem = s_xQueue.Head();

    const float fTimeToTransition = xQueueItem.m_fRunTime - xQueueItem.m_fTransitionTime;
    if( ( s_fTimer > fTimeToTransition )
        && ( xQueueItem.m_eTransition != JD1_DEMO_CUT )
        && ( s_xQueue.GetCount() > 1 ) )
    {
        // TODO: handle transitions somehow, probably use another framebuffer...
    }
    else if( xQueueItem.m_pxScene )
    {
        xQueueItem.m_pxScene->Render();
    }
}

void JD1_Demo_System::Update()
{
    s_fTimer += GLToy_Timer::GetFrameTime();

    if( s_xQueue.IsEmpty() )
    {
        return;
    }

    JD1_DemoQueueItem& xQueueItem = s_xQueue.Head();

    if( s_fTimer > xQueueItem.m_fRunTime )
    {
        if( xQueueItem.m_pxScene )
        {
            xQueueItem.m_pxScene->Stop();
        }

        s_fTimer = 0.0f;

        s_xQueue.RemoveHead();
        if( s_xQueue.IsEmpty() )
        {
            return;
        }

        xQueueItem = s_xQueue.Head();

        if( xQueueItem.m_pxScene )
        {
            xQueueItem.m_pxScene->Start();
        }
    }

    if( xQueueItem.m_pxScene )
    {
        xQueueItem.m_pxScene->Update();
    }
}

void JD1_Demo_System::Queue(
    JD1_DemoScene* const pxScene,
    const float fRunTime,
    const JD1_Demo_Transition eTransition,
    const float fTransitionTime )
{
    JD1_DemoQueueItem xItem =
    {
        pxScene,
        fRunTime,
        eTransition,
        fTransitionTime
    };

    s_xQueue.Append( xItem );
}
