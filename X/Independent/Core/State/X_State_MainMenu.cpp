/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010, 2011 Semi Essessi
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
#include <Core/State/X_State_MainMenu.h>

// GLToy
#include <Core/State/GLToy_State_System.h>
#include <Entity/GLToy_Entity_System.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture_System.h>
#include <UI/GLToy_UI_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void X_State_MainMenu_NewCallback( void* const pData )
{
    GLToy_State_System::ChangeState( GLToy_Hash_Constant( "Game" ) );
}

void X_State_MainMenu_QuitCallback( void* const pData )
{
    GLToy_UI_System::ShowQuitDialog();
}

void X_State_MainMenu::Initialise()
{
    GLToy_Entity_System::DestroyEntities();

    GLToy_Texture_System::CreateTexture( "Widgets/File.png" );
    GLToy_Texture_System::CreateTexture( "Widgets/Shutdown.png" );

    GLToy_Entity_System::SetRender( false );

    GLToy_UI_System::ShowPointer( true );

    const float fBaseX = GLToy_Render::GetMinX() + 0.05f;
    GLToy_UI_System::CreateLabel( "X", fBaseX, 0.85f );
    
    GLToy_UI_System::CreateImageButton(
        "Widgets/File.png", "New Game",
        X_State_MainMenu_NewCallback,
        fBaseX, 0.45f, 0.2f, 0.2f );
    
    GLToy_UI_System::CreateImageButton(
        "Widgets/Shutdown.png", "Quit",
        X_State_MainMenu_QuitCallback,
        fBaseX, 0.15f, 0.2f, 0.2f );
}

void X_State_MainMenu::Shutdown()
{
    GLToy_UI_System::ClearWidgets();
}
