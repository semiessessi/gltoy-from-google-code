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

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_IndirectArray< GLToy_Widget* > GLToy_UI_System::s_xTopWidgets;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_UI_System::Initialise()
{
	return true;
}

void GLToy_UI_System::Shutdown()
{
}

void GLToy_UI_System::Render2D()
{
	s_xTopWidgets.Traverse( GLToy_IndirectRender2DFunctor< GLToy_Widget >() );
}

void GLToy_UI_System::Update()
{
	s_xTopWidgets.Traverse( GLToy_IndirectUpdateFunctor< GLToy_Widget >() );
}

GLToy_Widget* GLToy_UI_System::CreateWidgetFromType( const GLToy_WidgetType eType )
{
	GLToy_Widget* pxWidget = NULL;

	switch( eType )
	{
		case WIDGET_NULL:
		{
			GLToy_Assert( false, "Creating a null widget, are you sure this is correct?" );
			pxWidget = new GLToy_Widget( eType );
		}

		default:
		{
			GLToy_Assert( false, "Unable to create unrecognised widget type: %d", eType );
			break;
		}
	}

	return pxWidget;
}