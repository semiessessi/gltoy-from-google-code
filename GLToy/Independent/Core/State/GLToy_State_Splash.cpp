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

// This file's header
#include <Core/State/GLToy_State_Splash.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Environment/GLToy_Environment_System.h>
#include <Entity/GLToy_Entity_System.h>
#include <Render/Font/GLToy_Font.h>
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture.h>
#include <UI/GLToy_UI_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

const GLToy_String szGLTOY_STRING1 = "GLToy is LGPL software, you should have recieved a copy of the source code";
const GLToy_String szGLTOY_STRING2 = "and license with this software, if not, visit http://code.google.com/p/gltoy";
const GLToy_String szHAVOK_STRING1 = "GLToy uses Havok®.  ©Copyright 1999-2008 Havok.com Inc. (and its Licensors).";
const GLToy_String szHAVOK_STRING2 = "All Rights Reserved.  See www.havok.com for details.";
const GLToy_String szOPENGL_STRING1 = "OpenGL® and the oval logo are trademarks or registered trademarks of ";
const GLToy_String szOPENGL_STRING2 = "Silicon Graphics, Inc. in the United States and/or other countries worldwide.";

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_State_Splash::Initialise()
{
    GLToy_UI_System::ShowPointer( false );
    GLToy_Camera::SetFlyCamEnabled( false );

    GLToy_Entity_System::SetRender( false );
    GLToy_Environment_System::SetRender( false );

    m_fTimer = 0.0f;
}

void GLToy_State_Splash::Render2D() const
{

#ifdef GLTOY_USE_HAVOK_PHYSICS

    GLToy_Texture_System::BindTexture( "Widgets/SplashBanner.png" );

#else

    GLToy_Texture_System::BindTexture( "Widgets/SplashBanner_NoHavok.png" );

#endif

    GLToy_Render::StartSubmittingQuads();
    GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );
    GLToy_Render::SubmitTexturedQuad2D( -1.0f, -0.6f, 1.0f, -0.35f );
    GLToy_Render::EndSubmit();

    GLToy_Font* pxFont = GLToy_Font_System::FindFont( GLToy_Hash_Constant( "Console" ) );
    if( !pxFont )
    {
        return;
    }

    const float afX[] =
    {
        -( static_cast< float >( szGLTOY_STRING1.GetLength() ) * pxFont->GetWidth() * 0.5f ),
        -( static_cast< float >( szGLTOY_STRING2.GetLength() ) * pxFont->GetWidth() * 0.5f ),
        -( static_cast< float >( szHAVOK_STRING1.GetLength() ) * pxFont->GetWidth() * 0.5f ),
        -( static_cast< float >( szHAVOK_STRING2.GetLength() ) * pxFont->GetWidth() * 0.5f ),
        -( static_cast< float >( szOPENGL_STRING1.GetLength() ) * pxFont->GetWidth() * 0.5f ),
        -( static_cast< float >( szOPENGL_STRING2.GetLength() ) * pxFont->GetWidth() * 0.5f ),
    };

    pxFont->RenderString( szGLTOY_STRING1, afX[ 0 ], -0.7f );
    pxFont->RenderString( szGLTOY_STRING2, afX[ 1 ], -0.75f );
    pxFont->RenderString( szHAVOK_STRING1, afX[ 2 ], -0.8f );
    pxFont->RenderString( szHAVOK_STRING2, afX[ 3 ], -0.85f );
    pxFont->RenderString( szOPENGL_STRING1, afX[ 4 ], -0.9f );
    pxFont->RenderString( szOPENGL_STRING2, afX[ 5 ], -0.95f );
}

void GLToy_State_Splash::Update()
{
    m_fTimer += GLToy_Timer::GetFrameTime();
}

bool GLToy_State_Splash::IsDone() const
{
#ifdef GLTOY_DEBUG
    return true;
#else
    return m_fTimer > 3.0f;
#endif
}
