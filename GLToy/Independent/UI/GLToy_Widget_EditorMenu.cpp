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

#include <Core/GLToy.h>

// this file's header
#include <UI/GLToy_Widget_EditorMenu.h>

// GLToy
#include <Maths/GLToy_Maths.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const float fTAB_SIZE = 0.3f;
static const float fMENU_SIZE = 0.6f;

struct GLToy_EditorMenu_SecondLevelItem
{
    GLToy_Hash m_uTextureHash;
    GLToy_String m_szName;
    void ( * m_pfnButtonCallback )( void* const pData );
};

struct GLToy_EditorMenu_TopLevelItem
{
    GLToy_Hash m_uTextureHash;
    GLToy_String m_szName;
    u_int m_uSecondLevelSize;
    GLToy_EditorMenu_SecondLevelItem* m_pxSecondLevelItems;
};

GLToy_EditorMenu_SecondLevelItem g_axOptionItems[] =
{
    { GLToy_Hash_Constant( "Widgets/Base_Round_Grey" ), "Camera", NULL }
};

GLToy_EditorMenu_SecondLevelItem g_axEntityItems[] =
{
    { GLToy_Hash_Constant( "Widgets/Base_Round_Grey" ), "Model", NULL },
    { GLToy_Hash_Constant( "Widgets/Base_Round_Grey" ), "Physics", NULL }
};

GLToy_EditorMenu_TopLevelItem g_axTopLevelItems[] =
{
    { GLToy_Hash_Constant( "Widgets/Base_Round_Grey" ), "Options", sizeof( g_axOptionItems ) / sizeof( GLToy_EditorMenu_SecondLevelItem ), g_axOptionItems },
    { GLToy_Hash_Constant( "Widgets/Base_Round_Grey" ), "Entities", sizeof( g_axEntityItems ) / sizeof( GLToy_EditorMenu_SecondLevelItem ), g_axEntityItems }
};

GLToy_EditorMenu_TopLevelItem* g_pxCurrentTopItem = NULL;
GLToy_EditorMenu_SecondLevelItem* g_pxCurrentSecondItem = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Widget_EditorMenu::Render2D() const
{
    float fX = GetX();
    float fY = GetY();
    for( u_int u = 0; u < sizeof( g_axTopLevelItems ) / sizeof( GLToy_EditorMenu_TopLevelItem ); ++u )
    {

    }
}

void GLToy_Widget_EditorMenu::Update()
{
    for( u_int u = 0; u < sizeof( g_axTopLevelItems ) / sizeof( GLToy_EditorMenu_TopLevelItem ); ++u )
    {

    }
}
