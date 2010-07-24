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
#include <UI/GLToy_UI_System.h>

// GLToy
#include <Core/Data Structures/GLToy_Array.h>
#include <Core/GLToy_Timer.h>
#include <Core/GLToy_UpdateFunctor.h>
#include <Input/GLToy_Input.h>
#include <Maths/GLToy_Maths.h>
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_RenderFunctor.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture.h>
#include <UI/GLToy_Dialog.h>
#include <UI/GLToy_Widget.h>
#include <UI/GLToy_Widget_EditorMenu.h>
#include <UI/GLToy_Widget_Image.h>
#include <UI/GLToy_Widget_ImageButton.h>
#include <UI/GLToy_Widget_Label.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const float fUI_MOUSE_SCALE = 1.0f / 100.0f;
static const float fUI_MOUSE_WIDTH = 1.0f / 12.5f;

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_UI_System::s_bShowPointer = false;
GLToy_Vector_2 GLToy_UI_System::s_xMousePosition = GLToy_Vector_2( 0.0f, 0.0f );

GLToy_Dialog* GLToy_UI_System::s_pxCurrentModalDialog = NULL;
GLToy_Array< GLToy_Dialog* > GLToy_UI_System::s_xDialogs;
GLToy_Array< GLToy_Widget* > GLToy_UI_System::s_xWidgets;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_UI_QuitCallback( void* const pData )
{
    GLToy::Quit();
}

void GLToy_UI_DefaultModalCallback( void* const pData )
{
    GLToy_UI_System::DestroyCurrentModalDialog();
}

bool GLToy_UI_System::Initialise()
{
    GLToy_Texture_System::CreateTexture( "Widgets/Pointer.png" );
    return true;
}

void GLToy_UI_System::Shutdown()
{
    ClearWidgets();
}

void GLToy_UI_System::Render2D()
{
    s_xWidgets.Traverse( GLToy_IndirectRender2DFunctor< GLToy_Widget >() );    
    s_xDialogs.Traverse( GLToy_IndirectRender2DFunctor< GLToy_Dialog >() );

    if( s_pxCurrentModalDialog )
    {
        s_pxCurrentModalDialog->Render2D();
    }

    // reset the depth functions etc.
    GLToy_Render::ClearDepth( 1.0f );
    GLToy_Render::DisableDepthTesting();

    // render the pointer
    if( s_bShowPointer )
    {
        GLToy_Render::EnableBlending();
        GLToy_Render::SetBlendFunction( BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA );

        GLToy_Texture_System::BindTexture( "Widgets/Pointer.png" );

        GLToy_Render::StartSubmittingQuads();
        
        GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );
        GLToy_Render::SubmitTexturedQuad2D(
            s_xMousePosition[ 0 ], s_xMousePosition[ 1 ] - fUI_MOUSE_WIDTH,
            s_xMousePosition[ 0 ] + fUI_MOUSE_WIDTH, s_xMousePosition[ 1 ] );

        GLToy_Render::EndSubmit();
    }

    GLToy_Render::DisableBlending();
}

void GLToy_UI_System::Update()
{
    s_xMousePosition[ 0 ] =
        GLToy_Maths::Clamp( s_xMousePosition[ 0 ] + GLToy_Input_System::GetMouseDeltaX() * fUI_MOUSE_SCALE, GLToy_Render::GetMinX(), GLToy_Render::GetMaxX() );
    s_xMousePosition[ 1 ] =
        GLToy_Maths::Clamp( s_xMousePosition[ 1 ] - GLToy_Input_System::GetMouseDeltaY() * fUI_MOUSE_SCALE, -1.0f, 1.0f );

    if( s_pxCurrentModalDialog )
    {
        s_pxCurrentModalDialog->Update();
    }
    else
    {
        s_xDialogs.Traverse( GLToy_IndirectUpdateFunctor< GLToy_Dialog >() );
        s_xWidgets.Traverse( GLToy_IndirectUpdateFunctor< GLToy_Widget >() );
    }
}

void GLToy_UI_System::ClearWidgets()
{
    s_xWidgets.DeleteAll();
}

GLToy_Widget_Label* GLToy_UI_System::CreateLabel( const GLToy_String& szLabel, const float fX, const float fY )
{
    GLToy_Widget_Label* pxLabel =
        static_cast< GLToy_Widget_Label* >(
            CreateWidget( WIDGET_LABEL, fX, fY ) );

    pxLabel->SetString( szLabel );

    return pxLabel;
}

GLToy_Widget_ImageButton* GLToy_UI_System::CreateImageButton(
    const GLToy_String& szTexture,
    const GLToy_String& szLabel,
    void ( * const pfnCallback )( void* const pData ),
    const float fX, const float fY,
    const float fWidth, const float fHeight )
{
    GLToy_Widget_ImageButton* pxImageButton =
        static_cast< GLToy_Widget_ImageButton* >(
            CreateWidget( WIDGET_IMAGEBUTTON, fX, fY, fWidth, fHeight ) );

    pxImageButton->SetTexture( szTexture.GetHash() );
    pxImageButton->SetLabelString( szLabel );
    pxImageButton->SetCallback( pfnCallback );

    return pxImageButton;
}

GLToy_Dialog* GLToy_UI_System::CreateDialog(
    const GLToy_DialogStyle ucStyle,
    const float fX, const float fY,
    const float fWidth, const float fHeight )
{
    if( ucStyle & DIALOG_STYLE_MODAL )
    {
        if( s_pxCurrentModalDialog )
        {
            GLToy_Assert( s_pxCurrentModalDialog == NULL, "Cannot create a modal dialog whilst one is already active." );
            return NULL;
        }
        else
        {
            s_pxCurrentModalDialog  = new GLToy_Dialog( ucStyle, fX, fY, fWidth, fHeight );
            return s_pxCurrentModalDialog;
        }
    }

    GLToy_Dialog* pxDialog = new GLToy_Dialog( ucStyle, fX, fY, fWidth, fHeight );

    return pxDialog;
}

void GLToy_UI_System::ShowErrorDialog( const GLToy_String& szError, ... )
{
    va_list xArgumentList;

    va_start( xArgumentList, szError );
    
    GLToy_String xErrorMessage;
    xErrorMessage.SetToFormatString( szError, xArgumentList );

    va_end( xArgumentList );

    GLToy_Dialog* pxQuitDialog = CreateDialog( DIALOG_STYLE_CENTRE | DIALOG_STYLE_MODAL );

    pxQuitDialog->AddText( szError );
    pxQuitDialog->SizeToText( szError );
    pxQuitDialog->AddOKButton( GLToy_UI_DefaultModalCallback );
}

void GLToy_UI_System::ShowQuitDialog()
{
    GLToy_Dialog* pxQuitDialog = CreateDialog( DIALOG_STYLE_CENTRE | DIALOG_STYLE_MODAL );

    pxQuitDialog->AddText( "Are you sure you want to quit?" );
    pxQuitDialog->SizeToText( "Are you sure you want to quit?" );
    pxQuitDialog->AddYesNoButtons( GLToy_UI_QuitCallback, GLToy_UI_DefaultModalCallback );
}

GLToy_Widget* GLToy_UI_System::CreateWidget(
    const GLToy_WidgetType eType,
    const float fX, const float fY,
    const float fWidth, const float fHeight )
{
    GLToy_Widget* pxWidget = NULL;

    switch( eType )
    {
        case WIDGET_NULL:
        {
            GLToy_Assert( false, "Creating a null widget, are you sure this is correct?" );
            pxWidget = new GLToy_Widget( eType, fX, fY, fWidth, fHeight );
            break;
        }

        case WIDGET_LABEL:
        {
            pxWidget = new GLToy_Widget_Label( eType, fX, fY, fWidth, fHeight );
            break;
        }

        case WIDGET_IMAGE:
        {
            pxWidget = new GLToy_Widget_Image( eType, fX, fY, fWidth, fHeight );
            break;
        }

        case WIDGET_IMAGEBUTTON:
        {
            pxWidget = new GLToy_Widget_ImageButton( eType, fX, fY, fWidth, fHeight );
            break;
        }

        case WIDGET_EDITORMENU:
        {
            pxWidget = new GLToy_Widget_EditorMenu( eType, fX, fY, fWidth, fHeight );
            break;
        }
        
        default:
        {
            GLToy_Assert( false, "Unable to create unrecognised widget type: %d", eType );
            break;
        }
    }

    if( pxWidget )
    {
        s_xWidgets.Append( pxWidget );
    }

    return pxWidget;
}

void GLToy_UI_System::DestroyCurrentModalDialog()
{
    if( s_pxCurrentModalDialog )
    {
        s_pxCurrentModalDialog->Destroy();
        s_pxCurrentModalDialog = NULL;
    }
}

void GLToy_UI_System::ShowPointer( const bool bShow )
{
    s_bShowPointer = bShow;
    if( bShow )
    {
        GLToy_Camera::SetFlyCamEnabled( false );
    }
}

float GLToy_UI_System::GetPulse()
{
    return 0.92f + 0.08f * GLToy_Maths::Cos( 7.5f * GLToy_Timer::GetTime() );
}
