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
