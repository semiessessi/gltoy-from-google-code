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
#include <Render/Shader/GLToy_Shader_System.h>

// GLToy
#include <Core/Data Structures/GLToy_HashTree.h>
#include <File/GLToy_ANSITextFile.h>
#include <File/GLToy_File_System.h>
#include <Render/GLToy_Render.h>
#include <Render/Shader/GLToy_Shader.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_HashTree< GLToy_ShaderProgram* > GLToy_Shader_System::s_xPrograms;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Shader_System::Initialise()
{
    s_xPrograms.Clear();

    GLToy_Array< GLToy_String > xShaderPaths = GLToy_File_System::PathsFromFilter( "Shaders/", "*.shader" );

    GLToy_ConstIterate( GLToy_String, xIterator, &xShaderPaths )
    {
        GLToy_String szName = xIterator.Current();
        szName.RemoveAt( 0, 8 ); // remove "Shaders/"
        szName.RemoveFromEnd( 7 ); // remove .shader
    
        GLToy_DebugOutput( "   - Found shader \"%S\".\r\n", szName.GetWideString() );
        
        GLToy_ANSITextFile xShaderFile = GLToy_ANSITextFile( xIterator.Current() );

        GLToy_String szShaderData = xShaderFile.GetString();

        const GLToy_String szFSPath = GLToy_String( "shaders/" ) + szShaderData.RemoveFirstLine() + ".fs";
        const GLToy_String szVSPath = GLToy_String( "shaders/" ) + szShaderData.RemoveFirstLine() + ".vs";

        GLToy_ANSITextFile xFSFile = GLToy_ANSITextFile( szFSPath );
        GLToy_ANSITextFile xVSFile = GLToy_ANSITextFile( szVSPath );

        if( xFSFile.GetSize() == 1 )
        {
            GLToy_DebugOutput( "   - Fragment shader \"%S\" is empty or doesn't exist.\r\n", szFSPath.GetWideString() );
            continue;
        }

        if( xVSFile.GetSize() == 1 )
        {
            GLToy_DebugOutput( "   - Vertex shader \"%S\" is empty or doesn't exist.\r\n", szVSPath.GetWideString() );
            continue;
        }

        const u_int uID = GLToy_Render::CreateProgram();
        if( uID == 0 )
        {
            GLToy_DebugOutput( "   - Failed to create shader program \"%S\".\r\n", szName.GetWideString() );
            continue;
        }

        const u_int uFSID = GLToy_Render::CreateFragmentShader();
        if( uFSID == 0 )
        {
            GLToy_DebugOutput( "   - Failed to create fragment shader \"%S\".\r\n", szFSPath.GetWideString() );
            continue;
        }

        const u_int uVSID = GLToy_Render::CreateVertexShader();
        if( uVSID == 0 )
        {
            GLToy_DebugOutput( "   - Failed to create vertex shader \"%S\".\r\n", szVSPath.GetWideString() );
            GLToy_Render::DeleteShader( uFSID );
            continue;
        }

        char* pxFSString = new char[ xFSFile.GetSize() ];
        xFSFile.GetRawString( pxFSString );

        u_int uFoo = GLToy_Render::GetError();
        bool bGoo = GLToy_Render::IsShader( uFSID );

        GLToy_Render::ShaderSource( uFSID, 1, &pxFSString, 0 );
        uFoo = GLToy_Render::GetError();
        bGoo = GLToy_Render::IsShader( uFSID );
        GLToy_Render::CompileShader( uFSID );
        uFoo = GLToy_Render::GetError();
        bGoo = GLToy_Render::IsShader( uFSID );

        delete[] pxFSString;

        char acLog[ 1024 ];
        int iLogLength = 0;

        GLToy_Render::GetShaderInfoLog( uFSID, 1023, &iLogLength, acLog );
        if( iLogLength > 0 )
        {
            GLToy_DebugOutput( "   - Compiling fragment shader \"%S\"...\r\n", szFSPath.GetWideString() );
            GLToy_DebugOutput( acLog );
        }

        char* pxVSString = new char[ xVSFile.GetSize() ];
        xVSFile.GetRawString( pxVSString );

        GLToy_Render::ShaderSource( uVSID, 1, &pxVSString, 0 );
        GLToy_Render::CompileShader( uVSID );

        delete[] pxVSString;

        GLToy_Render::GetShaderInfoLog( uVSID, 1023, &iLogLength, acLog );
        if( iLogLength > 0 )
        {
            GLToy_DebugOutput( "   - Compiling vertex shader \"%S\"...\r\n", szVSPath.GetWideString() );
            GLToy_DebugOutput( acLog );
        }

        s_xPrograms.AddNode( new GLToy_ShaderProgram( uID, uFSID, uVSID ), szName.GetHash() );

        GLToy_Render::ValidateProgram( uID );
        GLToy_Render::GetProgramInfoLog( uID, 1023, &iLogLength, acLog );
        if( iLogLength > 0 )
        {
            GLToy_DebugOutput( "   - Validating shader program \"%S\"...\r\n", szName.GetWideString() );
            GLToy_DebugOutput( acLog );
            GLToy_Render::DeleteShader( uFSID );
        }
    }

    return true;
}

void GLToy_Shader_System::Shutdown()
{
    s_xPrograms.DeleteAll();
}

void GLToy_Shader_System::BindShaderProgram( const GLToy_Hash uHash )
{
    GLToy_ShaderProgram** ppxShaderProgram = s_xPrograms.FindData( uHash );
    if( ppxShaderProgram )
    {
        ( *ppxShaderProgram )->Bind();
    }
}

void GLToy_Shader_System::BindShaderProgram( const GLToy_String& szName )
{
    return BindShaderProgram( szName.GetHash() );
}

GLToy_ShaderProgram* GLToy_Shader_System::FindShader( const GLToy_Hash uHash )
{
    GLToy_ShaderProgram** ppxShaderProgram = s_xPrograms.FindData( uHash );
    return ppxShaderProgram ? *ppxShaderProgram : NULL;
}

GLToy_ShaderProgram* GLToy_Shader_System::LookUpShader( const GLToy_String& szName )
{
    return FindShader( szName.GetHash() );
}
