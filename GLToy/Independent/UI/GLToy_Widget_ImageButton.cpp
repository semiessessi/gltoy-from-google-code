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

#include <Core/GLToy.h>

// this file's header
#include <UI/GLToy_Widget_ImageButton.h>

// GLToy
#include <Input/GLToy_Input.h>
#include <Maths/GLToy_Maths.h>
#include <Render/Font/GLToy_Font.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture.h>
#include <UI/GLToy_Dialog.h>
#include <UI/GLToy_UI_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Widget_ImageButton::Render2D() const
{
    GLToy_Render::EnableBlending();
    GLToy_Render::SetBlendFunction( BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA );

    GLToy_Texture_System::BindTexture( m_uTextureHash );

    GLToy_Render::StartSubmittingQuads();
    GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );
    GLToy_Render::SubmitTexturedQuad2D( GetX(), GetY(), GetX() + GetWidth(), GetY() + GetHeight() );
    GLToy_Render::EndSubmit();

    GLToy_Font* const pxFont = GLToy_Font_System::FindFont( GetFont() );
    if( !pxFont )
    {
        return;
    }

    pxFont->RenderString( GetLabelString(), GetX() + GetWidth() + 0.05f, GetY() + 0.5f * ( GetHeight() - pxFont->GetHeight() ),
        m_bHighlightFlag
        ? GLToy_Vector_4( 0.4f * GLToy_UI_System::GetPulse(), 0.9f  * GLToy_UI_System::GetPulse(), 0.4f * GLToy_UI_System::GetPulse(), 1.0f )
            : GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );
}

void GLToy_Widget_ImageButton::Update()
{
    const GLToy_Vector_2 xOffset = GetParent() ? GetParent()->GetPosition() : GLToy_Maths::ZeroVector2;
    const GLToy_Vector_2 xMousePosition = GLToy_UI_System::GetMousePosition() - xOffset;
    if( ( xMousePosition[ 0 ] > GetX() )
        && ( xMousePosition[ 1 ] > GetY() )
        && ( xMousePosition[ 0 ] < ( GetX() + GetWidth() ) )
        && ( xMousePosition[ 1 ] < ( GetY() + GetHeight() ) ) )
    {
        // we are under the pointer
        m_bHighlightFlag = true;
        if( GLToy_Input_System::IsMouseLeftButtonDown() )
        {
            // we are held down
            m_bClickFlag = true;
        }
        else
        {
            if( m_bClickFlag && m_pfnCallback )
            {
                m_pfnCallback( 0 );
            }

            m_bClickFlag = false;
        }
    }
    else
    {
        m_bHighlightFlag = false;
        m_bClickFlag = false;
    }
}
