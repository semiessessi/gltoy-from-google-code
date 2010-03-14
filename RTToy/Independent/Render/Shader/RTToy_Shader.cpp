/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Render/Shader/RTToy_Shader.h>

// GLToy
#include <Render/GLToy_Render.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

RTToy_ShaderProgram::RTToy_ShaderProgram( const u_int uFSID, const u_int uVSID )
: m_uFSID( uFSID )
, m_uVSID( uVSID )
{
    m_uID = GLToy_Render::CreateProgram();

    if( IsReady() )
    {
        GLToy_Render::AttachShader( m_uID, m_uFSID );
        GLToy_Render::AttachShader( m_uID, m_uVSID );
        GLToy_Render::LinkProgram( m_uID );
        GLToy_Render::UseProgram( 0 );
    }
}

RTToy_ShaderProgram::~RTToy_ShaderProgram()
{
    if( IsReady() )
    {
        GLToy_Render::DetachShader( m_uID, m_uFSID );
        GLToy_Render::DetachShader( m_uID, m_uVSID );
        GLToy_Render::DeleteShader( m_uFSID );
        GLToy_Render::DeleteShader( m_uVSID );
        GLToy_Render::DeleteProgram( m_uID );
        GLToy_Render::UseProgram( 0 );
    }
}

void RTToy_ShaderProgram::Bind() const
{
    GLToy_Render::UseProgram( m_uID );
}