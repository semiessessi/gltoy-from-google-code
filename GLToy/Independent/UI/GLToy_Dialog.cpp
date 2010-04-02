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
#include <UI/GLToy_Dialog.h>

// GLToy
#include <Maths/GLToy_Maths.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_RenderFunctor.h>
#include <Render/GLToy_Texture.h>
#include <String/GLToy_String.h>
#include <UI/GLToy_UI_System.h>
#include <UI/GLToy_Widget.h>
#include <UI/GLToy_Widget_Label.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const float fDIALOG_TEXT_OFFSET = 0.05f;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Dialog::GLToy_Dialog(
    const GLToy_DialogStyle ucStyle,
    const float fX, const float fY,
    const float fWidth, const float fHeight )
: m_ucStyle( ucStyle )
, m_xPosition( fX, fY )
, m_xSize( fWidth, fHeight )
, m_xWidgets()
{
}

GLToy_Dialog::~GLToy_Dialog()
{
    m_xWidgets.DeleteAll();
}

void GLToy_Dialog::Render2D() const
{
    // dialogs need to be drawn in order, and with clipping so we clear the depth buffer
    // and use it like a mask for the widgets and other contents
    const float fWidgetZ = 1.0f;
    GLToy_Render::ClearDepth( fWidgetZ - 1.0f );
    GLToy_Render::SetDepthFunction( DEPTH_GREATER );
    GLToy_Texture_System::BindWhite();

    GLToy_Render::StartSubmittingQuads();

    GLToy_Render::SubmitColour( GLToy_Vector_4( 0.7f, 0.7f, 0.7f, 0.4f ) );

    GLToy_Render::SubmitVertex( GetX(), GetY(), fWidgetZ + 1.0f );
    GLToy_Render::SubmitVertex( GetX(), GetY(), fWidgetZ + 1.0f );
    GLToy_Render::SubmitVertex( GetX(), GetY(), fWidgetZ + 1.0f );
    GLToy_Render::SubmitVertex( GetX(), GetY(), fWidgetZ + 1.0f );

    GLToy_Render::EndSubmit();

    GLToy_Render::PushViewMatrix();
    GLToy_Render::Translate( GLToy_Vector_3( 0.0f, 0.0f, -fWidgetZ ) );

    m_xWidgets.Traverse( GLToy_IndirectRender2DFunctor< GLToy_Widget >() );

    GLToy_Render::PopViewMatrix();
}

void GLToy_Dialog::Update()
{
    // TODO - optimise, this is quite inefficient...
    if( m_ucStyle & DIALOG_STYLE_CENTRE )
    {
        SetPosition(
            GLToy_Render::GetMinX() + ( GLToy_Render::Get2DWidth() - GetWidth() ) * 0.5f,
            1.0f - ( 2.0f - GetHeight() ) * 0.5f );
    }
}

void GLToy_Dialog::AddText( const GLToy_String& szString )
{
    GLToy_Widget_Label* pxLabel = new GLToy_Widget_Label( WIDGET_LABEL, fDIALOG_TEXT_OFFSET, fDIALOG_TEXT_OFFSET );
    pxLabel->SetString( szString );
    AddWidget( pxLabel );
}

void GLToy_Dialog::AddWidget( GLToy_Widget* const pxWidget )
{
    m_xWidgets.Append( pxWidget );
}