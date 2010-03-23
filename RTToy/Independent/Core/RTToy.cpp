/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// This file's header
#include <Core/RTToy.h>

// GLToy
#include <Core/State/GLToy_State_System.h>

//#include <UI/GLToy_UI_System.h>
//#include <UI/GLToy_Widget.h>
//#include <UI/GLToy_Widget_Label.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool RTToy::Initialise()
{
    GLToy_State_System::ChangeState( GLToy_Hash_Constant( "EditorFrontEnd" ) );

    //GLToy_Widget_Label* pxLabel = static_cast< GLToy_Widget_Label* >( GLToy_UI_System::CreateWidget( WIDGET_LABEL, 0.0f, 0.0f ) );
    //pxLabel->SetFont( "Console" );
    //pxLabel->SetString( "test" );

    return true;
}

void RTToy::Shutdown()
{
}