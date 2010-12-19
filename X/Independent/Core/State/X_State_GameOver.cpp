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

#include <Core/X.h>

// This file's header
#include <Core/State/X_State_GameOver.h>

// GLToy
#include <Core/State/GLToy_State_System.h>
#include <Entity/GLToy_Entity_System.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture.h>
#include <UI/GLToy_UI_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void X_State_GameOver_MainMenuCallback( void* const pData )
{
    GLToy_State_System::ChangeState( GLToy_Hash_Constant( "MainMenu" ) );
}

void X_State_GameOver::Initialise()
{
    GLToy_Entity_System::DestroyEntities();

    GLToy_Texture_System::CreateTexture( "Widgets/Base_Round_Gray.png" );

    GLToy_Entity_System::SetRender( false );

    GLToy_UI_System::ShowPointer( true );

    const float fBaseX = GLToy_Render::GetMinX() + 0.05f;
    GLToy_UI_System::CreateLabel( "G A M E   O V E R", fBaseX, 0.85f );
    
    GLToy_UI_System::CreateImageButton(
        "Widgets/Base_Round_Gray.png", "Main Menu",
        X_State_GameOver_MainMenuCallback,
        fBaseX, 0.45f, 0.2f, 0.2f );
}

void X_State_GameOver::Shutdown()
{
    GLToy_UI_System::ClearWidgets();
}
