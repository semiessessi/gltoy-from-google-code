/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Render/Shader/GLToy_Shader.h>

// GLToy
#include <Render/GLToy_Render.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_ShaderProgram::GLToy_ShaderProgram( const u_int uID, const u_int uFSID, const u_int uVSID )
: m_uID( uID )
, m_uFSID( uFSID )
, m_uVSID( uVSID )
{
    if( IsReady() )
    {
        GLToy_Render::AttachShader( m_uID, m_uFSID );
        GLToy_Render::AttachShader( m_uID, m_uVSID );
        GLToy_Render::LinkProgram( m_uID );
        GLToy_Render::UseProgram( 0 );
    }
}

GLToy_ShaderProgram::~GLToy_ShaderProgram()
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

void GLToy_ShaderProgram::Bind() const
{
    GLToy_Render::UseProgram( m_uID );
}

void GLToy_ShaderProgram::SetUniform( const GLToy_String& szName, const int iValue )
{
	char* szString = szName.CreateANSIString();
	u_int uUniformID = GLToy_Render::GetUniformID( m_uID, szString );
	delete[] szString;
	GLToy_Render::SetUniform( uUniformID, iValue );
}