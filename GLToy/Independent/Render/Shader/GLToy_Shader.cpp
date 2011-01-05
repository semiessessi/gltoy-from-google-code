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

// This file's header
#include <Render/Shader/GLToy_Shader.h>

// GLToy
#include <Maths/GLToy_Vector.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture_System.h>

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
    if( m_bUseNoise )
    {
        // we might only have 4 texture units...
        // TODO: use the last texture unit instead
        GLToy_Texture_System::BindTexture( GLToy_Hash_Constant( "GLToy_Shader_Noise" ), 3 );
        SetUniform( "xNoiseTexture", 3 );
    }
}

void GLToy_ShaderProgram::SetUniform( const GLToy_String& szName, const int iValue ) const
{
    char* szString = szName.CreateANSIString();
    u_int uUniformID = GLToy_Render::GetUniformID( m_uID, szString );
    delete[] szString;
    GLToy_Render::SetUniform( uUniformID, iValue );
}

void GLToy_ShaderProgram::SetUniform( const GLToy_String& szName, const float fValue ) const
{
    char* szString = szName.CreateANSIString();
    u_int uUniformID = GLToy_Render::GetUniformID( m_uID, szString );
    delete[] szString;
    GLToy_Render::SetUniform( uUniformID, fValue );
}

void GLToy_ShaderProgram::SetUniform( const GLToy_String& szName, const float fValue1, const float fValue2 ) const
{
    char* szString = szName.CreateANSIString();
    u_int uUniformID = GLToy_Render::GetUniformID( m_uID, szString );
    delete[] szString;
    GLToy_Render::SetUniform( uUniformID, fValue1, fValue2 );
}

void GLToy_ShaderProgram::SetUniform( const GLToy_String& szName, const float fValue1, const float fValue2, const float fValue3 ) const
{
    char* szString = szName.CreateANSIString();
    u_int uUniformID = GLToy_Render::GetUniformID( m_uID, szString );
    delete[] szString;
    GLToy_Render::SetUniform( uUniformID, fValue1, fValue2, fValue3 );
}

void GLToy_ShaderProgram::SetUniform( const GLToy_String& szName, const float fValue1, const float fValue2, const float fValue3, const float fValue4 ) const
{
    char* szString = szName.CreateANSIString();
    u_int uUniformID = GLToy_Render::GetUniformID( m_uID, szString );
    delete[] szString;
    GLToy_Render::SetUniform( uUniformID, fValue1, fValue2, fValue3, fValue4 );
}

void GLToy_ShaderProgram::SetUniform( const GLToy_String& szName, const GLToy_Vector_2& xValue ) const
{
    char* szString = szName.CreateANSIString();
    u_int uUniformID = GLToy_Render::GetUniformID( m_uID, szString );
    delete[] szString;
    GLToy_Render::SetUniform( uUniformID, xValue[ 0 ], xValue[ 1 ] );
}

void GLToy_ShaderProgram::SetUniform( const GLToy_String& szName, const GLToy_Vector_3& xValue ) const
{
    char* szString = szName.CreateANSIString();
    u_int uUniformID = GLToy_Render::GetUniformID( m_uID, szString );
    delete[] szString;
    GLToy_Render::SetUniform( uUniformID, xValue[ 0 ], xValue[ 1 ], xValue[ 2 ] );
}

void GLToy_ShaderProgram::SetUniform( const GLToy_String& szName, const GLToy_Vector_4& xValue ) const
{
    char* szString = szName.CreateANSIString();
    u_int uUniformID = GLToy_Render::GetUniformID( m_uID, szString );
    delete[] szString;
    GLToy_Render::SetUniform( uUniformID, xValue[ 0 ], xValue[ 1 ], xValue[ 2 ], xValue[ 3 ] );
}

void GLToy_ShaderProgram::SetUseNoise( const bool bUseNoise )
{
	m_bUseNoise = bUseNoise;
}
