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

// This file's header
#include <Demo/JD1_DemoScene_AVS.h>

// GLToy
#include <Render/GLToy_Camera.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void JD1_DemoScene_AVS::Initialise()
{
    GLToy_Iterate( JD1_AVS_Component, xIterator, &m_xComponents )
    {
        GLToy_Renderable* const pxRenderable = xIterator.Current().First();
        pxRenderable->Initialise();
    }

    GLToy_Camera::SetPosition( GLToy_Maths::ZeroVector3 );
}

void JD1_DemoScene_AVS::Shutdown()
{
    GLToy_Iterate( JD1_AVS_Component, xIterator, &m_xComponents )
    {
        GLToy_Renderable* const pxRenderable = xIterator.Current().First();
        pxRenderable->Shutdown();
    }
}

void JD1_DemoScene_AVS::Render() const
{
    GLToy_ConstIterate( JD1_AVS_Component, xIterator, &m_xComponents )
    {
        const GLToy_Renderable* const pxRenderable = xIterator.Current().First();
        pxRenderable->Render();
    }
}

void JD1_DemoScene_AVS::Update()
{
    GLToy_Iterate( JD1_AVS_Component, xIterator, &m_xComponents )
    {
        GLToy_Updateable* const pxUpdateable = xIterator.Current().Second();
        pxUpdateable->Update();
    }
}