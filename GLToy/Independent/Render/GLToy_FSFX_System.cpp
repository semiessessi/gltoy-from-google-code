/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2011 Semi Essessi
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
#include <Render/GLToy_FSFX_System.h>

// GLToy
#include <Core/Data Structures/GLToy_HashMap.h>
#include <File/GLToy_File_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_FSFX
{

public:

    GLToy_FSFX( const GLToy_Hash uHash )
    : m_xShaderPasses()
    , m_uHash( uHash )
    , m_bEnabled( false )
    {
    }

    void AddPass( const GLToy_Hash uShaderHash )
    {
        m_xShaderPasses.Append( uShaderHash );
    }

    void Render() const
    {
    }

    void Update()
    {
    }

    GLToy_Array< GLToy_Hash > m_xShaderPasses;
    GLToy_Hash m_uHash;
    bool m_bEnabled;

};

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_HashMap< GLToy_FSFX > GLToy_FSFX_System::s_xFSFX;
GLToy_Array< const GLToy_FSFX* > GLToy_FSFX_System::s_xRenderList;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_FSFX_System::Initialise()
{
    s_xFSFX.Clear();

    GLToy_Array< GLToy_String > xFSFXPaths = GLToy_File_System::PathsFromFilter( "FSFX/", "*.fsfx" );

    GLToy_ConstIterate( GLToy_String, szPath, xFSFXPaths )

        GLToy_String szName = szPath;

        szName.RemoveAt( 0, 5 ); // remove "FSFX/"
        szName.RemoveFromEnd( 5 ); // remove ".fsfx"

        GLToy_DebugOutput( "   - Found FSFX \"%S\".\r\n", szName.GetWideString() );

        GLToy_FSFX xFSFX( szName.GetHash() );

        s_xFSFX.AddNode( xFSFX, szName.GetHash() );
        s_xRenderList.Append( &xFSFX );

    GLToy_Iterate_End;

    return true;
}

void GLToy_FSFX_System::Shutdown()
{
    s_xFSFX.Clear();
}

void GLToy_FSFX_System::Render()
{
    GLToy_ConstIterate( const GLToy_FSFX*, pxFSFX, s_xRenderList )
        pxFSFX->Render();
    GLToy_Iterate_End;
}

void GLToy_FSFX_System::Update()
{
    GLToy_Iterate( GLToy_FSFX, xFSFX, s_xFSFX )
        xFSFX.Update();
    GLToy_Iterate_End;
}

void GLToy_FSFX_System::EnableFSFX( const GLToy_Hash uHash, const bool bEnabled )
{
    GLToy_FSFX* const pxFSFX = s_xFSFX.FindData( uHash );
    if( pxFSFX )
    {
        pxFSFX->m_bEnabled = bEnabled;
    }
}
