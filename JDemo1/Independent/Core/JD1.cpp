/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010 Semi Essessi
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

// This file's header
#include <Core/JD1.h>

// GLToy
#include <Core/State/GLToy_State_System.h>
#include <Render/GLToy_Render.h>

// JD1
#include <Demo/JD1_Demo_System.h>
#include <Demo/JD1_DemoScene_Intro.h>
#include <Demo/JD1_DemoScene_MovingParticles.h>
#include <Demo/JD1_DemoScene_Dragon.h>
#include <Demo/JD1_DemoScene_TexerFlow.h>
#include <Demo/JD1_DemoScene_Tunnel.h>
#include <Demo/JD1_DemoScene_P9_Birth.h>
#include <Sound/JD1_Sound_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool JD1::Initialise()
{
    GLToy::ChangeWindowTitle( "JDemo 1" );

    GLToy_InitialiserCall( JD1_Sound_System );
    GLToy_InitialiserCall( JD1_Demo_System );

    GLToy_State_System::ChangeState( GLToy_Hash_Constant( "FixedCamera" ) );

    //JD1_Demo_System::Queue( new JD1_DemoScene_Intro(), 2.0f );
    //JD1_Demo_System::Queue( new JD1_DemoScene_MovingParticles(), 10.0f );
    //JD1_Demo_System::Queue( new JD1_DemoScene_Dragon(), 10.0f );
    JD1_Demo_System::Queue( new JD1_DemoScene_P9_Birth() );
    //JD1_Demo_System::Queue( new JD1_DemoScene_Tunnel() );
    //JD1_Demo_System::Queue( new JD1_DemoScene_AVS_Test() );
    //JD1_Demo_System::Queue( new JD1_DemoScene_TexerFlow(), 20.0f );

    return true;
}

void JD1::Shutdown()
{
    JD1_Demo_System::Shutdown();
    JD1_Sound_System::Shutdown();
}

void JD1::Update()
{
    JD1_Demo_System::Update();
    JD1_Sound_System::Update();
}
