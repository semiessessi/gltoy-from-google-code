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

const GLToy_String szGLTOY_STRING1 = "GLToy is provided under the MIT license, you should have recieved a copy of";
const GLToy_String szGLTOY_STRING2 = "the license with this software, if not, visit http://code.google.com/p/gltoy";
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
    GLToy_Camera::SetLocked( true );

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
