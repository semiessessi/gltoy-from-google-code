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
#include <UI/GLToy_UI_System.h>

// GLToy
#include <Core/Data Structures/GLToy_Array.h>
#include <Core/GLToy_UpdateFunctor.h>
#include <Input/GLToy_Input.h>
#include <Maths/GLToy_Maths.h>
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_RenderFunctor.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture.h>
#include <UI/GLToy_Widget.h>
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
GLToy_Array< GLToy_Widget* > GLToy_UI_System::s_xTopWidgets;
GLToy_Vector_2 GLToy_UI_System::s_xMousePosition = GLToy_Vector_2( 0.0f, 0.0f );

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

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
	s_xTopWidgets.Traverse( GLToy_IndirectRender2DFunctor< GLToy_Widget >() );

    // render the pointer
    if( s_bShowPointer )
    {
        GLToy_Texture_System::BindTexture( "Widgets/Pointer.png" );
        GLToy_Render::EnableBlending();
        GLToy_Render::SetBlendFunction( BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA );

        GLToy_Render::StartSubmittingQuads();
        GLToy_Render::SubmitTexturedQuad2D( s_xMousePosition[ 0 ], s_xMousePosition[ 1 ] - fUI_MOUSE_WIDTH, s_xMousePosition[ 0 ] + fUI_MOUSE_WIDTH, s_xMousePosition[ 1 ] );
        GLToy_Render::EndSubmit();

        GLToy_Render::DisableBlending();
    }
}

void GLToy_UI_System::Update()
{
    s_xMousePosition[ 0 ] = GLToy_Maths::Clamp( s_xMousePosition[ 0 ] + GLToy_Input_System::GetMouseDeltaX() * fUI_MOUSE_SCALE, -1.0f, 1.0f );
    s_xMousePosition[ 1 ] = GLToy_Maths::Clamp( s_xMousePosition[ 1 ] - GLToy_Input_System::GetMouseDeltaY() * fUI_MOUSE_SCALE, -1.0f, 1.0f );

	s_xTopWidgets.Traverse( GLToy_IndirectUpdateFunctor< GLToy_Widget >() );
}

void GLToy_UI_System::ClearWidgets()
{
    s_xTopWidgets.DeleteAll();
}

GLToy_Widget* GLToy_UI_System::CreateWidget( const GLToy_WidgetType eType, const float fX, const float fY, const float fWidth, const float fHeight )
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

		default:
		{
			GLToy_Assert( false, "Unable to create unrecognised widget type: %d", eType );
			break;
		}
	}

    if( pxWidget )
    {
        s_xTopWidgets.Append( pxWidget );
    }

	return pxWidget;
}

void GLToy_UI_System::ShowPointer( const bool bShow )
{
    GLToy_Camera::SetFlyCamEnabled( false );
    s_bShowPointer = bShow;
}