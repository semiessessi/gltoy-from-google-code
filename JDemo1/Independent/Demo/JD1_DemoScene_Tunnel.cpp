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
#include <Demo/JD1_DemoScene_Tunnel.h>

// GLToy
#include <Render/GLToy_Raytrace_Fullscreen.h>
#include <Render/GLToy_Texture.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void JD1_DemoScene_Tunnel::Initialise()
{
    GLToy_Texture_System::CreateTexture( GLToy_Hash_Constant( "generic/grid1.png" ) );
}

void JD1_DemoScene_Tunnel::Shutdown()
{
}

void JD1_DemoScene_Tunnel::Render() const
{
    GLToy_Raytrace_Fullscreen xRaytrace( GLToy_Hash_Constant( "JD1_Raytrace_Tunnel" ) );

    xRaytrace.BindTexture( "xTexture", GLToy_Hash_Constant( "generic/grid1.png" ) );
    xRaytrace.Render();
}

void JD1_DemoScene_Tunnel::Update()
{
}
