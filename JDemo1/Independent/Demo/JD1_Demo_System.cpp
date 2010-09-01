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

#include <Core/JD1.h>

// This file's headers
#include <Demo/JD1_Demo_System.h>

// GLToy
#include <Core/Data Structures/GLToy_List.h>
#include <Core/GLToy_Timer.h>
#include <Maths/GLToy_Maths.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture.h>

// JD1
#include <Demo/JD1_DemoScene.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

bool JD1_Demo_System::s_bFlashFade = false;
float JD1_Demo_System::s_fFlashTime = 0.0f;
float JD1_Demo_System::s_fTimer = 0.0f;
GLToy_List< JD1_Demo_System::JD1_DemoQueueItem > JD1_Demo_System::s_xQueue;
GLToy_List< JD1_Demo_System::JD1_DemoQueueItem > JD1_Demo_System::s_xDeleteList;

static bool g_bFirst = true;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool JD1_Demo_System::Initialise()
{
    s_xQueue.Clear();
    s_xDeleteList.Clear();
    s_fTimer = 0.0f;
    g_bFirst = true;

    return true;
}

void JD1_Demo_System::Shutdown()
{
    GLToy_Iterate( JD1_DemoQueueItem, xIterator, &s_xQueue )
    {
        delete xIterator.Current().m_pxScene;
    }

    GLToy_Iterate( JD1_DemoQueueItem, xIterator, &s_xDeleteList )
    {
        delete xIterator.Current().m_pxScene;
    }

    s_xQueue.Clear();
    s_xDeleteList.Clear();

    g_bFirst = false;
}

void JD1_Demo_System::Render()
{
    if( s_xQueue.IsEmpty() )
    {
        return;
    }

    JD1_DemoQueueItem& xQueueItem = s_xQueue.Head();

    const float fTimeToTransition = xQueueItem.m_fRunTime - xQueueItem.m_fTransitionTime;
    if( ( xQueueItem.m_fRunTime > 0.0f )
        && ( s_fTimer > fTimeToTransition )
        && ( xQueueItem.m_eTransition != JD1_DEMO_CUT )
        && ( s_xQueue.GetCount() > 1 ) )
    {
        // TODO: handle transitions somehow, probably use another framebuffer...
        const float fTransitionAmount = ( s_fTimer - fTimeToTransition ) / xQueueItem.m_fTransitionTime;
        switch( xQueueItem.m_eTransition )
        {
            case JD1_DEMO_FLASH_WHITE:
            {
                GLToy_Render::SetOrthogonalProjectionMatrix();
                GLToy_Render::PushViewMatrix();
                GLToy_Render::SetIdentityViewMatrix();

                GLToy_Texture_System::BindWhite();

                GLToy_Render::DisableDepthWrites();
                GLToy_Render::DisableDepthTesting();
                GLToy_Render::EnableBlending();
                GLToy_Render::SetBlendFunction( BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA );

                GLToy_Render::StartSubmittingQuads();

                GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, fTransitionAmount ) );
                GLToy_Render::SubmitTexturedQuad2D( GLToy_Vector_2( -0.5f * GLToy_Render::Get2DWidth(), -1.0f ), GLToy_Vector_2( GLToy_Render::Get2DWidth(), 2.0f ) );

                GLToy_Render::EndSubmit();

                GLToy_Render::EnableDepthTesting();
                GLToy_Render::EnableDepthWrites();
                GLToy_Render::DisableBlending();

                GLToy_Render::SetPerspectiveProjectionMatrix();
                GLToy_Render::PopViewMatrix();
                break;
            }
        }
    }
    else if( xQueueItem.m_pxScene )
    {
        xQueueItem.m_pxScene->Render();

        if( s_bFlashFade && ( s_fTimer < s_fFlashTime ) )
        {
            const float fTransitionAmount = ( s_fFlashTime - s_fTimer ) / s_fFlashTime;
            GLToy_Render::SetOrthogonalProjectionMatrix();
            GLToy_Render::PushViewMatrix();
            GLToy_Render::SetIdentityViewMatrix();

            GLToy_Texture_System::BindWhite();

            GLToy_Render::DisableDepthWrites();
            GLToy_Render::DisableDepthTesting();
            GLToy_Render::EnableBlending();
            GLToy_Render::SetBlendFunction( BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA );

            GLToy_Render::StartSubmittingQuads();

            GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, fTransitionAmount ) );
            GLToy_Render::SubmitTexturedQuad2D( GLToy_Vector_2( -0.5f * GLToy_Render::Get2DWidth(), -1.0f ), GLToy_Vector_2( GLToy_Render::Get2DWidth(), 2.0f ) );

            GLToy_Render::EndSubmit();

            GLToy_Render::EnableDepthTesting();
            GLToy_Render::EnableDepthWrites();
            GLToy_Render::DisableBlending();

            GLToy_Render::SetPerspectiveProjectionMatrix();
            GLToy_Render::PopViewMatrix();
        }
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

    if( g_bFirst )
    {
        g_bFirst = false;
        if( xQueueItem.m_pxScene )
        {
            xQueueItem.m_pxScene->Start();
        }
    }

    if( ( xQueueItem.m_fRunTime > 0.0f )
        && ( s_fTimer > xQueueItem.m_fRunTime ) )
    {
        if( xQueueItem.m_pxScene )
        {
            xQueueItem.m_pxScene->Stop();
            if( xQueueItem.m_eTransition == JD1_DEMO_FLASH_WHITE )
            {
                s_bFlashFade = true;
                s_fFlashTime = xQueueItem.m_fTransitionTime;
            }
        }

        s_fTimer = 0.0f;

        s_xDeleteList.Append( xQueueItem );
        s_xQueue.RemoveHead();

        if( s_xQueue.IsEmpty() )
        {
            return;
        }

        JD1_DemoQueueItem& xNewQueueItem = s_xQueue.Head();

        if( xNewQueueItem.m_pxScene )
        {
            xNewQueueItem.m_pxScene->Start();
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
    GLToy_Assert( pxScene != NULL, "Null pointer to scene passed in to JD1_Demo_System::Queue!" );

    JD1_DemoQueueItem xItem =
    {
        pxScene,
        fRunTime,
        eTransition,
        fTransitionTime
    };

    s_xQueue.Append( xItem );

    pxScene->Initialise();
}
