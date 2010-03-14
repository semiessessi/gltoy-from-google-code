/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Render/Shader/RTToy_Shader_System.h>

// GLToy
#include <Core/Data Structures/GLToy_HashTree.h>
#include <File/GLToy_ANSITextFile.h>
#include <File/GLToy_File_System.h>
#include <Render/GLToy_Render.h>

// RTToy
#include <Render/Shader/RTToy_Shader.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_HashTree< RTToy_ShaderProgram* > RTToy_Shader_System::s_xPrograms;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool RTToy_Shader_System::Initialise()
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

        const GLToy_String szFSPath = szShaderData.RemoveFirstLine() + ".fs";
        const GLToy_String szVSPath = szShaderData.RemoveFirstLine() + ".vs";

        GLToy_ANSITextFile xFSFile = GLToy_ANSITextFile( szFSPath );
        GLToy_ANSITextFile xVSFile = GLToy_ANSITextFile( szVSPath);

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

        const u_int uFSID = GLToy_Render::CreateFragmentShader();
        if( uFSID == 0 )
        {
            GLToy_DebugOutput( "   - Failed to create fragment shader \"%S\".\r\n", szFSPath.GetWideString() );
            continue;
        }

        char* pxFSString = new char[ xFSFile.GetSize() ];
        xFSFile.GetRawString( pxFSString );

        GLToy_Render::ShaderSource( uFSID, 1, &pxFSString, 0 );
        GLToy_Render::CompileShader( uFSID );

        delete[] pxFSString;

        char acLog[ 1024 ];
        int iLogLength = 0;

        GLToy_Render::GetShaderInfoLog( uFSID, 1023, &iLogLength, acLog );
        if( iLogLength > 0 )
        {
            GLToy_DebugOutput( "   - Compile error in fragment shader \"%S\".\r\n", szFSPath.GetWideString() );
            GLToy_DebugOutput( acLog );
            GLToy_Render::DeleteShader( uFSID );
            continue;
        }

        const u_int uVSID = GLToy_Render::CreateVertexShader();
        if( uVSID == 0 )
        {
            GLToy_DebugOutput( "   - Failed to create vertex shader \"%S\".\r\n", szVSPath.GetWideString() );
            GLToy_Render::DeleteShader( uFSID );
            continue;
        }

        char* pxVSString = new char[ xVSFile.GetSize() ];
        xVSFile.GetRawString( pxVSString );

        GLToy_Render::ShaderSource( uVSID, 1, &pxVSString, 0 );
        GLToy_Render::CompileShader( uVSID );

        delete[] pxVSString;

        GLToy_Render::GetShaderInfoLog( uVSID, 1023, &iLogLength, acLog );
        if( iLogLength > 0 )
        {
            GLToy_DebugOutput( "   - Compile error in vertex shader \"%S\".\r\n", szVSPath.GetWideString() );
            GLToy_DebugOutput( acLog );
            GLToy_Render::DeleteShader( uFSID );
            GLToy_Render::DeleteShader( uVSID );
            continue;
        }

        s_xPrograms.AddNode( new RTToy_ShaderProgram( uFSID, uVSID ), szName.GetHash() );
    }

    return true;
}

void RTToy_Shader_System::Shutdown()
{
    s_xPrograms.DeleteAll();
    s_xPrograms.Clear();
}

void RTToy_Shader_System::BindShaderProgram( const GLToy_Hash uHash )
{
    RTToy_ShaderProgram** ppxShaderProgram = s_xPrograms.FindData( uHash );
    if( ppxShaderProgram )
    {
        ( *ppxShaderProgram )->Bind();
    }
}

void RTToy_Shader_System::BindShaderProgram( const GLToy_String& szName )
{
    return BindShaderProgram( szName.GetHash() );
}