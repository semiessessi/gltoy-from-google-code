/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
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
#include <Render/Shader/GLToy_Shader_System.h>

// GLToy
#include <Core/Data Structures/GLToy_HashTree.h>
#include <File/GLToy_TextFile.h>
#include <File/GLToy_File_System.h>
#include <Maths/GLToy_Maths.h>
#include <Maths/GLToy_Noise.h>
#include <Render/GLToy_Render.h>
#include <Render/Shader/GLToy_Shader.h>
#include <Render/GLToy_Texture.h>

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

    // create a noise texture
    u_int* puNoise = new u_int[ 256 * 256 ];

    for( u_int u = 0; u < 256; ++u )
    {
        for( u_int v = 0; v < 256; ++v )
        {
            u_char* const pucComponents = reinterpret_cast< u_char* >( &( puNoise[ u * 256 + v ] ) );
            const float fU = static_cast< float >( u ) / 256;
            const float fV = static_cast< float >( v ) / 256;
            pucComponents[ 0 ] = static_cast< u_char >( GLToy_Maths::Clamp( GLToy_Noise::Cubic2D( fU, fV, 32.0f, 1.0f, 32 ) * 128.0f + 127.0f, 0.0f, 255.0f ) );
            pucComponents[ 1 ] = static_cast< u_char >( GLToy_Maths::Clamp( GLToy_Noise::Cubic2D( fU + 0.5f, fV, 32.0f, 1.0f, 32 ) * 128.0f + 127.0f, 0.0f, 255.0f ) );
            pucComponents[ 2 ] = static_cast< u_char >( GLToy_Maths::Clamp( GLToy_Noise::Cubic2D( fU, fV + 0.5f, 32.0f, 1.0f, 32 ) * 128.0f + 127.0f, 0.0f, 255.0f ) );
            pucComponents[ 3 ] = static_cast< u_char >( GLToy_Maths::Clamp( GLToy_Noise::Cubic2D( fU + 0.5f, fV + 0.5f, 32.0f, 1.0f, 32 ) * 128.0f + 127.0f, 0.0f, 255.0f ) );
        }
    }

    GLToy_Texture_System::CreateTextureFromRGBAData( GLToy_Hash_Constant( "GLToy_Shader_Noise" ), puNoise, 256, 256 );
#ifdef GLTOY_DEBUG
	GLToy_Texture_System::SaveTextureTGA( "GLToy_Shader_Noise", puNoise, 256, 256 );
#endif

    delete[] puNoise;

#ifndef GLTOY_DEMO

    GLToy_Array< GLToy_String > xShaderPaths = GLToy_File_System::PathsFromFilter( "Shaders/", "*.shader" );

    GLToy_ConstIterate( GLToy_String, xIterator, &xShaderPaths )
    {
        GLToy_String szName = xIterator.Current();
        szName.RemoveAt( 0, 8 ); // remove "Shaders/"
        szName.RemoveFromEnd( 7 ); // remove .shader
    
        GLToy_DebugOutput( "   - Found shader \"%S\".\r\n", szName.GetWideString() );
        
        GLToy_TextFile xShaderFile = GLToy_TextFile( xIterator.Current() );

        GLToy_String szShaderData = xShaderFile.GetString();

        const GLToy_String szFSPath = GLToy_String( "Shaders/" ) + szShaderData.RemoveFirstLine() + ".fs";
        const GLToy_String szVSPath = GLToy_String( "Shaders/" ) + szShaderData.RemoveFirstLine() + ".vs";

        GLToy_TextFile xFSFile = GLToy_TextFile( szFSPath );
        GLToy_TextFile xVSFile = GLToy_TextFile( szVSPath );

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
        GLToy_DebugOutput( "   - Compiling fragment shader \"%S\"...\r\n", szFSPath.GetWideString() );
        if( iLogLength > 0 )
        {
            GLToy_DebugOutput( acLog );
        }
        else
        {
            GLToy_DebugOutput( "   - Success!\r\n" );
        }

        char* pxVSString = new char[ xVSFile.GetSize() ];
        xVSFile.GetRawString( pxVSString );

        GLToy_Render::ShaderSource( uVSID, 1, &pxVSString, 0 );
        GLToy_Render::CompileShader( uVSID );

        delete[] pxVSString;

        GLToy_Render::GetShaderInfoLog( uVSID, 1023, &iLogLength, acLog );
        GLToy_DebugOutput( "   - Compiling vertex shader \"%S\"...\r\n", szVSPath.GetWideString() );
        if( iLogLength > 0 )
        {
            GLToy_DebugOutput( acLog );
        }
        else
        {
            GLToy_DebugOutput( "   - Success!\r\n" );
        }

        s_xPrograms.AddNode( new GLToy_ShaderProgram( uID, uFSID, uVSID ), szName.GetHash() );

        GLToy_Render::ValidateProgram( uID );
        GLToy_Render::GetProgramInfoLog( uID, 1023, &iLogLength, acLog );
        GLToy_DebugOutput( "   - Validating shader program \"%S\"...\r\n", szName.GetWideString() );
        if( iLogLength > 0 )
        {
            GLToy_DebugOutput( acLog );
            GLToy_Render::DeleteShader( uFSID );
        }
        else
        {
            GLToy_DebugOutput( "   - Success!\r\n" );
        }
    }

#endif

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

void GLToy_Shader_System::CreateShaderFromStrings( const GLToy_String& szName, const GLToy_String& szFragmentShader, const GLToy_String& szVertexShader )
{
	const u_int uID = GLToy_Render::CreateProgram();
    if( uID == 0 )
    {
        GLToy_DebugOutput( "   - Failed to create shader program \"%S\".\r\n", szName.GetWideString() );
        return;
    }

    const u_int uFSID = GLToy_Render::CreateFragmentShader();
    if( uFSID == 0 )
    {
        GLToy_DebugOutput( "   - Failed to create fragment shader for \"%S\".\r\n", szName.GetWideString() );
        return;
    }

    const u_int uVSID = GLToy_Render::CreateVertexShader();
    if( uVSID == 0 )
    {
        GLToy_DebugOutput( "   - Failed to create vertex shader for \"%S\".\r\n", szName.GetWideString() );
        GLToy_Render::DeleteShader( uFSID );
        return;
    }

    char* pxFSString = szFragmentShader.CreateANSIString();

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
    GLToy_DebugOutput( "   - Compiling fragment shader for \"%S\"...\r\n", szName.GetWideString() );
    if( iLogLength > 0 )
    {
        GLToy_DebugOutput( acLog );
    }
    else
    {
        GLToy_DebugOutput( "   - Success!\r\n" );
    }

    char* pxVSString = szVertexShader.CreateANSIString();

    GLToy_Render::ShaderSource( uVSID, 1, &pxVSString, 0 );
    GLToy_Render::CompileShader( uVSID );

    delete[] pxVSString;

    GLToy_Render::GetShaderInfoLog( uVSID, 1023, &iLogLength, acLog );
    GLToy_DebugOutput( "   - Compiling vertex shader for \"%S\"...\r\n", szName.GetWideString() );
    if( iLogLength > 0 )
    {
        GLToy_DebugOutput( acLog );
    }
    else
    {
        GLToy_DebugOutput( "   - Success!\r\n" );
    }

    s_xPrograms.AddNode( new GLToy_ShaderProgram( uID, uFSID, uVSID ), szName.GetHash() );

    GLToy_Render::ValidateProgram( uID );
    GLToy_Render::GetProgramInfoLog( uID, 1023, &iLogLength, acLog );
    GLToy_DebugOutput( "   - Validating shader program \"%S\"...\r\n", szName.GetWideString() );
    if( iLogLength > 0 )
    {
        GLToy_DebugOutput( acLog );
        GLToy_Render::DeleteShader( uFSID );
    }
    else
    {
        GLToy_DebugOutput( "   - Success!\r\n" );
    }
}
