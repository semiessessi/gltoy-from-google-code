/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010, 2011 Semi Essessi
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
#include <Input/GLToy_Input_System.h>
#include <Maths/GLToy_Maths.h>
#include <Render/Font/GLToy_Font.h>
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_RenderFunctor.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture_System.h>
#include <UI/GLToy_Dialog.h>
#include <UI/GLToy_Widget.h>
#include <UI/GLToy_Widget_EditorMenu.h>
#include <UI/GLToy_Widget_Image.h>
#include <UI/GLToy_Widget_ImageButton.h>
#include <UI/GLToy_Widget_Label.h>
#include <UI/GLToy_Widget_StatBar.h>

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

bool GLToy_UI_System::s_bOldPointerShow = false;
bool GLToy_UI_System::s_bOldFlyCam = false;
bool GLToy_UI_System::s_bOldFpsCam = false;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_UI_System::Initialise()
{
    GLToy_Texture_System::CreateTexture( "Widgets/Pointer.png" );

    GLToy_Font* pxFont = GLToy_Font_System::FindFont( GLToy_Hash_Constant( "FrontEnd" ) );
    if( pxFont )
    {
        pxFont->Initialise();
    }

    return true;
}

void GLToy_UI_System::Shutdown()
{
    ClearWidgets();
}

void GLToy_UI_System::Render2D()
{
    // reset the depth functions etc.
    GLToy_Render::ClearDepth( 1.0f );
    GLToy_Render::DisableDepthTesting();

    GLToy_IndirectRender2DFunctor< GLToy_Widget > xWidgetFunctor;
    GLToy_IndirectRender2DFunctor< GLToy_Dialog > xDialogFunctor;
    s_xWidgets.Traverse( xWidgetFunctor );    
    s_xDialogs.Traverse( xDialogFunctor );

    GLToy_Render::DisableDepthTesting();

    if( s_pxCurrentModalDialog )
    {
        s_pxCurrentModalDialog->Render2D();
    }

    // render the pointer
    if( s_bShowPointer )
    {
        GLToy_Render::DisableDepthTesting();
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
        GLToy_IndirectUpdateFunctor< GLToy_Widget > xWidgetFunctor;
        GLToy_IndirectUpdateFunctor< GLToy_Dialog > xDialogFunctor;
        s_xDialogs.Traverse( xDialogFunctor );
        s_xWidgets.Traverse( xWidgetFunctor );
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

GLToy_Widget_Image* GLToy_UI_System::CreateImage( const GLToy_String& szTexture, const float fX, const float fY, const float fWidth, const float fHeight )
{
    GLToy_Widget_Image* pxImage =
        static_cast< GLToy_Widget_Image* >(
            CreateWidget( WIDGET_IMAGE, fX, fY, fWidth, fHeight ) );

    pxImage->SetTexture( szTexture );

    return pxImage;
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

GLToy_Widget_StatBar* GLToy_UI_System::CreateStatBar( const GLToy_String& szTexture, const float& fStat, const float fMax, const float fX, const float fY, const float fWidth, const float fHeight )
{
    GLToy_Widget_StatBar* pxBar =
        static_cast< GLToy_Widget_StatBar* >(
            CreateWidget( WIDGET_STATBAR, fX, fY, fWidth, fHeight ) );

    pxBar->SetTexture( szTexture );
    pxBar->SetStat( fStat, fMax );

    return pxBar;
}

GLToy_Dialog* GLToy_UI_System::CreateDialog(
    const GLToy_DialogStyle ucStyle,
    const float fX, const float fY,
    const float fWidth, const float fHeight )
{

    GLToy_Dialog* pxDialog = NULL;
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
            pxDialog = s_pxCurrentModalDialog;
        }
    }
    else
    {
        pxDialog = new GLToy_Dialog( ucStyle, fX, fY, fWidth, fHeight );
    }

    s_bOldPointerShow = IsPointerShown();

    ShowPointer( true );

    return pxDialog;
}

void GLToy_UI_System::ShowErrorDialog( const GLToy_String& szError, ... )
{
    va_list xArgumentList;

    va_start( xArgumentList, szError );
    
    GLToy_String xErrorMessage;
    xErrorMessage.SetToFormatString( szError, xArgumentList );

    va_end( xArgumentList );

    GLToy_Dialog* const pxErrorDialog = CreateDialog( DIALOG_STYLE_CENTRE | DIALOG_STYLE_MODAL );
    
    if( !pxErrorDialog )
    {
        return;
    }

    pxErrorDialog->AddText( szError );
    pxErrorDialog->SizeToText( szError );
    pxErrorDialog->AddOKButton( DefaultModalCallback );
}

void GLToy_UI_System::ShowQuitDialog()
{
    GLToy_Dialog* const pxQuitDialog = CreateDialog( DIALOG_STYLE_CENTRE | DIALOG_STYLE_MODAL );

    if( !pxQuitDialog )
    {
        return;
    }

    pxQuitDialog->AddText( "Are you sure you want to quit?" );
    pxQuitDialog->SizeToText( "Are you sure you want to quit?" );
    pxQuitDialog->AddYesNoButtons( QuitCallback, DefaultModalCallback );
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

        case WIDGET_STATBAR:
        {
            pxWidget = new GLToy_Widget_StatBar( eType, fX, fY, fWidth, fHeight );
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
    if( !s_pxCurrentModalDialog )
    {
        return;
    }

    s_pxCurrentModalDialog->Destroy();
    s_pxCurrentModalDialog = NULL;
    ShowPointer( s_bOldPointerShow );
}

void GLToy_UI_System::ShowPointer( const bool bShow )
{
    s_bShowPointer = bShow;
    if( bShow )
    {
        s_bOldFlyCam = GLToy_Camera::IsFlyCamEnabled();
        s_bOldFpsCam = GLToy_Camera::IsControllerCamEnabled();
        GLToy_Camera::SetControllerCamEnabled( false );
        GLToy_Camera::SetFlyCamEnabled( false );
    }

    GLToy_Camera::SetLocked( bShow );

    if( !bShow )
    {
        GLToy_Camera::SetFlyCamEnabled( s_bOldFlyCam );
        GLToy_Camera::SetControllerCamEnabled( s_bOldFpsCam );
    }
}

float GLToy_UI_System::GetPulse()
{
    return 0.92f + 0.08f * GLToy_Maths::Cos( 7.5f * GLToy_Timer::GetTime() );
}


void GLToy_UI_System::QuitCallback( void* const pData )
{
    GLToy::Quit();
}

void GLToy_UI_System::DefaultModalCallback( void* const pData )
{
    DestroyCurrentModalDialog();
}