/////////////////////////////////////////////////////////////////////////////////////////////
//
// �Copyright 2010 Semi Essessi
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
#include <Demo/JD1_DemoScene_MovingParticles.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

JD1_DemoScene_MovingParticles::JD1_DemoScene_MovingParticles()
: GLToy_Parent()
, m_xBlitterFeedback( 1.5f )
, m_xBlur()
, m_xFadeout( 0.02f )
, m_xTexer()
{
}

void JD1_DemoScene_MovingParticles::Initialise()
{
    AppendComponent( static_cast< GLToy_Renderable* >( &m_xBlitterFeedback ), static_cast< GLToy_Updateable* >( &m_xBlitterFeedback ) );
    AppendComponent( static_cast< GLToy_Renderable* >( &m_xBlur ), static_cast< GLToy_Updateable* >( &m_xBlur ) );
    AppendComponent( static_cast< GLToy_Renderable* >( &m_xFadeout ), static_cast< GLToy_Updateable* >( &m_xFadeout ) );
    AppendComponent( static_cast< GLToy_Renderable* >( &m_xTexer ), static_cast< GLToy_Updateable* >( &m_xTexer ) );

    GLToy_Parent::Initialise();
}