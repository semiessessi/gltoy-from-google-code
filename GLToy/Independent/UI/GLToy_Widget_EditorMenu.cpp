/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of GLToy.
//
// GLToy is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// GLToy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with GLToy.
// If not, see <http://www.gnu.org/licenses/>.
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
