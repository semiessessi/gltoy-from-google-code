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

    if( m_pxModel )
    {
        m_uModelHash = uHash;
        SetBB( m_pxModel->GetBB() + GetPosition() );
    }
}

void GLToy_Entity_ModelStatic::SetModel( const GLToy_String& szName )
{
    SetModel( szName.GetHash() );
}

void GLToy_Entity_ModelStatic::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    GLToy_Parent::ReadFromBitStream( xStream );

    xStream >> m_uModelHash;

    SetModel( m_uModelHash );
}

void GLToy_Entity_ModelStatic::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    GLToy_Parent::WriteToBitStream( xStream );

    xStream << m_uModelHash;
}

void GLToy_Entity_ModelStatic::Render() const
{
    if( m_pxModel && IsActive() )
    {
        m_pxModel->InitialiseFirstFrameData();
        m_pxModel->RenderWithPositionAndOrientation( GetPosition(), GetOrientation() );
    }
}