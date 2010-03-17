/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Entity/Model/GLToy_Entity_ModelStatic.h>

// GLToy
#include <Model/GLToy_Model.h>
#include <Model/GLToy_Model_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Entity_ModelStatic::SetModel( const GLToy_Hash uHash )
{
    m_pxModel = GLToy_Model_System::LoadModel( uHash );
}

void GLToy_Entity_ModelStatic::SetModel( const GLToy_String& szName )
{
    m_pxModel = GLToy_Model_System::LoadModel( szName );
}

void GLToy_Entity_ModelStatic::Render() const
{
    if( m_pxModel && IsActive() )
    {
        m_pxModel->InitialiseFirstFrameData();
        m_pxModel->RenderWithPositionAndOrientation( GetPosition(), GetOrientation() );
    }
}