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
#include <UI/GLToy_Dialog.h>

// GLToy
#include <Core/GLToy_UpdateFunctor.h>
#include <Maths/GLToy_Maths.h>
#include <Render/Font/GLToy_Font.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_RenderFunctor.h>
#include <Render/GLToy_Texture.h>
#include <String/GLToy_String.h>
#include <UI/GLToy_UI_System.h>
#include <UI/GLToy_Widget.h>
#include <UI/GLToy_Widget_ImageButton.h>
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
, m_bDelete( false )
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
    const float fWidgetZ = 2.0f;
    
    GLToy_Render::DisableDepthTesting();
    GLToy_Render::EnableDepthWrites();
    GLToy_Render::EnableBlending();
    GLToy_Render::SetBlendFunction( BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA );
    
    GLToy_Render::ClearDepth( fWidgetZ - 1.0f );
    
    GLToy_Texture_System::BindWhite();

    GLToy_Render::StartSubmittingQuads();

    GLToy_Render::SubmitColour( GLToy_Vector_4( 0.4f, 0.4f, 0.4f, 0.85f ) );

    GLToy_Render::SubmitVertex( GetX(), GetY(), fWidgetZ + 1.0f );
    GLToy_Render::SubmitVertex( GetX() + GetWidth(), GetY(), fWidgetZ + 1.0f );
    GLToy_Render::SubmitVertex( GetX() + GetWidth(), GetY() + GetHeight(), fWidgetZ + 1.0f );
    GLToy_Render::SubmitVertex( GetX(), GetY() + GetHeight(), fWidgetZ + 1.0f );

    GLToy_Render::EndSubmit();

    GLToy_Render::EnableDepthTesting();
    GLToy_Render::SetDepthFunction( DEPTH_LEQUAL );

    GLToy_Render::PushViewMatrix();
    GLToy_Render::Translate( GLToy_Vector_3( GetX(), GetY(), fWidgetZ ) );

    GLToy_IndirectRender2DFunctor< GLToy_Widget > xFunctor;
    m_xWidgets.Traverse( xFunctor );

    GLToy_Render::PopViewMatrix();
}

void GLToy_Dialog::Update()
{
    // TODO - optimise, this is quite inefficient...
    if( m_ucStyle & DIALOG_STYLE_CENTRE )
    {
        SetPosition( -GetWidth() * 0.5f, -GetHeight() * 0.5f );
    }

    GLToy_IndirectUpdateFunctor< GLToy_Widget > xFunctor;
    m_xWidgets.Traverse( xFunctor );

    if( m_bDelete )
    {
        delete this;
    }
}

void GLToy_Dialog::Expand( const float fWidth, const float fHeight )
{
    GLToy_Iterate( GLToy_Widget*, xIterator, &m_xWidgets )
    {
        xIterator.Current()->SetPosition(
            xIterator.Current()->GetX(),
            xIterator.Current()->GetY() + fHeight );
    }

    m_xSize[ 0 ] += fWidth; m_xSize[ 1 ] += fHeight;
}

void GLToy_Dialog::AddText( const GLToy_String& szString )
{
    GLToy_Widget_Label* pxLabel = new GLToy_Widget_Label( WIDGET_LABEL, fDIALOG_TEXT_OFFSET, fDIALOG_TEXT_OFFSET );
    pxLabel->SetString( szString );
    pxLabel->SetFont( "Console" );

    AddWidget( pxLabel );
}

void GLToy_Dialog::AddWidget( GLToy_Widget* const pxWidget )
{
    m_xWidgets.Append( pxWidget );
    pxWidget->SetParent( this );
}

void GLToy_Dialog::AddOKButton(
    void ( * const pfnCallback )( void * const ),
    void* const pYData )
{
    Expand( 0.0f, 0.2f + fDIALOG_TEXT_OFFSET );

    GLToy_Widget_ImageButton* pxOKButton =
        new GLToy_Widget_ImageButton(
            WIDGET_IMAGEBUTTON,
            GetWidth() * 0.5f - 0.1f,
            fDIALOG_TEXT_OFFSET );

    pxOKButton->SetLabelString( "OK" );
    pxOKButton->SetFont( "Console" );
    pxOKButton->SetTexture( "Widgets/Base_Round.png" );
    pxOKButton->SetCallback( pfnCallback );
}

void GLToy_Dialog::AddYesNoButtons(
    void ( * const pfnYesCallback )( void * const ),
    void ( * const pfnNoCallback )( void * const ),
    void* const pYesData, void* const pNoData )
{
    Expand( 0.0f, 0.2f + fDIALOG_TEXT_OFFSET );

    GLToy_Widget_ImageButton* pxYesButton =
        new GLToy_Widget_ImageButton(
            WIDGET_IMAGEBUTTON,
            GetWidth() * 0.5f - 0.35f,
            fDIALOG_TEXT_OFFSET );

    pxYesButton->SetLabelString( "Yes" );
    pxYesButton->SetFont( "Console" );
    pxYesButton->SetTexture( "Widgets/Tick.png" );
    pxYesButton->SetCallback( pfnYesCallback );

    GLToy_Widget_ImageButton* pxNoButton =
        new GLToy_Widget_ImageButton(
            WIDGET_IMAGEBUTTON,
            GetWidth() * 0.5f + 0.05f,
            fDIALOG_TEXT_OFFSET );

    pxNoButton->SetLabelString( "No" );
    pxNoButton->SetFont( "Console" );
    pxNoButton->SetTexture( "Widgets/Cross.png" );
    pxNoButton->SetCallback( pfnNoCallback );

    AddWidget( pxYesButton );
    AddWidget( pxNoButton );
}

void GLToy_Dialog::SizeToText( const GLToy_String& szString )
{
    GLToy_Widget_Label xLabel( WIDGET_LABEL, fDIALOG_TEXT_OFFSET, fDIALOG_TEXT_OFFSET );
    xLabel.SetFont( "Console" );

    GLToy_Font* pxFont = GLToy_Font_System::FindFont( xLabel.GetFont() );

    if( !pxFont )
    {
        return;
    }

    const float fWidth = pxFont->GetWidth() * static_cast< float >( szString.GetLength() ) + 2.0f * fDIALOG_TEXT_OFFSET;
    const float fHeight = pxFont->GetHeight() + 2.0f * fDIALOG_TEXT_OFFSET;
    SetSize( fWidth, fHeight );
}
