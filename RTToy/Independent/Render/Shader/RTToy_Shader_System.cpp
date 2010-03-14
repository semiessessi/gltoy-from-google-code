/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Render/Shader/RTToy_Shader_System.h>

// GLToy
#include <Core/Data Structures/GLToy_HashTree.h>
#include <File/GLToy_File_System.h>

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
        GLToy_String xName = xIterator.Current();
        xName.RemoveAt( 0, 8 ); // remove "Shaders/"
        xName.RemoveFromEnd( 7 ); // remove .shader
    
        GLToy_DebugOutput( "   - Found shader \"%S\".\r\n", xName.GetWideString() );
        
        GLToy_ANSITextFile xShaderFile = GLToy_ANSITextFile( xIterator.Current() );

        GLToy_String szShaderData = xShaderFile.GetString();

        const GLToy_String szFSPath = szShaderData.ReadFirstLine();
        const GLToy_String szVSPath = szShaderData.ReadFirstLine();

        GLToy_ANSITextFile xFSFile = GLToy_ANSITextFile( szFSPath );
        GLToy_ANSITextFile xVSFile = GLToy_ANSITextFile( szVSPath );

        // s_xPrograms.AddNode( NULL, xName.GetHash() );
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