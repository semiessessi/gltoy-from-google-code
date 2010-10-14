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
#include <Demo/JD1_DemoScene_Dragon.h>

// GLToy
#include <Render/GLToy_Render.h>
#include <Render/Font/GLToy_Font.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void JD1_DemoScene_Dragon::Initialise()
{
}

void JD1_DemoScene_Dragon::Render() const
{
    GLToy_Render::SetClearFrame( true );

    const GLToy_Font* const pxFont = GLToy_Font_System::FindFont( GLToy_Hash_Constant( "Console" ) );

    GLToy_Render::SetOrthogonalProjectionMatrix();
    GLToy_Render::PushViewMatrix();
    GLToy_Render::SetIdentityViewMatrix();
    GLToy_Render::DisableDepthWrites();
    GLToy_Render::DisableDepthTesting();

    pxFont->RenderString( "Dragon goes here...", 0.0f, 0.0f, GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );

    GLToy_Render::EnableDepthTesting();
    GLToy_Render::EnableDepthWrites();
    GLToy_Render::SetPerspectiveProjectionMatrix();
    GLToy_Render::PopViewMatrix();
}
