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

#include <Core/X.h>

// This file's header
#include <Render/X_Render.h>

// GLToy
#include <Core/State/GLToy_State_System.h>
#include <Render/GLToy_Texture_System.h>

// X
#include <Render/X_Parallax_Background.h>
#include <Render/X_Starfield.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

static X_Starfield s_xStarfield;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool X_Render::Initialise()
{
    GLToy_Texture_System::CreateTexture( GLToy_Hash_Constant( "Background_Star_1.png" ) );
    GLToy_Texture_System::CreateTexture( GLToy_Hash_Constant( "Background_Star_2.png" ) );
    GLToy_Texture_System::CreateTexture( GLToy_Hash_Constant( "Background_Star_3.png" ) );
    GLToy_Texture_System::CreateTexture( GLToy_Hash_Constant( "Background_Star_4.png" ) );
    GLToy_Texture_System::CreateTexture( GLToy_Hash_Constant( "Background_Test.png" ) );
    X_Starfield::InitialiseData();
    return true;
}

void X_Render::Shutdown()
{
}

void X_Render::Render()
{
    if( GLToy_State_System::GetState() == GLToy_Hash_Constant( "Game" ) )
    {
        X_Parallax_Background xBackground( GLToy_Hash_Constant( "Background_Test.png" ), GLToy_Hash_Constant( "Background_Test.png" ), 1.0f, 2.0f );
        xBackground.Bind();
        xBackground.Render();
        
        X_Starfield::Update();
        s_xStarfield.Render();
    }
}
