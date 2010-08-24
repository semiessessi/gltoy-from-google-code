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

#ifndef __JD1_DEMOSCENE_AVS_TEST_H_
#define __JD1_DEMOSCENE_AVS_TEST_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Demo/JD1_DemoScene_AVS.h>

// GLToy
#include <Render/GLToy_Camera.h>

// JD1
#include <Render/Trans/JD1_Trans_BlitterFeedback.h>
#include <Render/Trans/JD1_Trans_Fadeout.h>
#include <Render/SuperScope/JD1_SuperScope_Dragon.h>
#include <Render/SuperScope/JD1_SuperScope_Morph.h>
#include <Render/Texer/JD1_Texer_SimpleComplexity.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class JD1_DemoScene_AVS_Test
: public JD1_DemoScene_AVS
{
    
    typedef JD1_DemoScene_AVS GLToy_Parent;

public:

    JD1_DemoScene_AVS_Test()
    : GLToy_Parent()
    , m_xTest()
    , m_xFadeout( 0.5f )
    , m_xBlitterFeedback()
    {
    }

    virtual ~JD1_DemoScene_AVS_Test()
    {
    }

    virtual void Initialise()
    {
        AppendComponent( static_cast< GLToy_Renderable* >( &m_xBlitterFeedback ), static_cast< GLToy_Updateable* >( &m_xBlitterFeedback ) );
        AppendComponent( static_cast< GLToy_Renderable* >( &m_xFadeout ), static_cast< GLToy_Updateable* >( &m_xFadeout ) );
        AppendComponent( static_cast< GLToy_Renderable* >( &m_xTest ), static_cast< GLToy_Updateable* >( &m_xTest ) );

        GLToy_Parent::Initialise();
    }

protected:

    JD1_Texer_SimpleComplexity m_xTest;
    JD1_Trans_Fadeout m_xFadeout;
    JD1_Trans_BlitterFeedback m_xBlitterFeedback;

};

#endif
