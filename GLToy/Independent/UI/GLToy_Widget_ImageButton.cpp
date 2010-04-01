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
#include <UI/GLToy_Widget_ImageButton.h>

// GLToy
#include <Input/GLToy_Input.h>
#include <Maths/GLToy_Maths.h>
#include <Render/Font/GLToy_Font.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture.h>
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
	const GLToy_Vector_2& xMousePosition = GLToy_UI_System::GetMousePosition();
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
