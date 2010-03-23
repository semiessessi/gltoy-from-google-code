/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// this file's header
#include <UI/GLToy_UI_System.h>

// GLToy
#include <Core/Data Structures/GLToy_Array.h>
#include <Core/GLToy_UpdateFunctor.h>
#include <Render/GLToy_RenderFunctor.h>
#include <UI/GLToy_Widget.h>
#include <UI/GLToy_Widget_Label.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_UI_System::s_bShowPointer = false;
GLToy_Array< GLToy_Widget* > GLToy_UI_System::s_xTopWidgets;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_UI_System::Initialise()
{
	return true;
}

void GLToy_UI_System::Shutdown()
{
    ClearWidgets();
}

void GLToy_UI_System::Render2D()
{
	s_xTopWidgets.Traverse( GLToy_IndirectRender2DFunctor< GLToy_Widget >() );
}

void GLToy_UI_System::Update()
{
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