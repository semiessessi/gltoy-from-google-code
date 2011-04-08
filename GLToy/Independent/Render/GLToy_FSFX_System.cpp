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
    , m_ucFlags( 0 )
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
    union
    {
        struct
        {
            bool m_bEnabled                 : 1;
            bool m_bTwoPassConvolution      : 1;
            bool m_bUseDepthBuffer          : 1;

            u_char m_ucSortHint             : 4;
        };

        u_char m_ucFlags;
    };

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

    for( GLToy_ConstIterator< GLToy_String > xIterator; !xIterator.Done( xFSFXPaths ); xIterator.Next() )
{
const GLToy_String& szPath = xIterator.Current( xFSFXPaths );

        GLToy_String szName = szPath;

        szName.RemoveAt( 0, 5 ); // remove "FSFX/"
        szName.RemoveFromEnd( 5 ); // remove ".fsfx"

        GLToy_DebugOutput( "   - Found FSFX \"%S\".\r\n", szName.GetWideString() );

        GLToy_FSFX xFSFX( szName.GetHash() );

        s_xFSFX.AddNode( xFSFX, szName.GetHash() );
        s_xRenderList.Append( &xFSFX );

    }

    return true;
}

void GLToy_FSFX_System::Shutdown()
{
    s_xFSFX.Clear();
}

void GLToy_FSFX_System::Render()
{
    for( GLToy_ConstIterator< const GLToy_FSFX* > xIterator; !xIterator.Done( s_xRenderList ); xIterator.Next() )
    {
        const GLToy_FSFX* const& pxFSFX = xIterator.Current( s_xRenderList );
        if( pxFSFX->m_bEnabled )
        {
            pxFSFX->Render();
        }
    }
}

void GLToy_FSFX_System::Update()
{
    for( GLToy_Iterator< GLToy_FSFX > xIterator; !xIterator.Done( s_xFSFX ); xIterator.Next() )
    {
        GLToy_FSFX& xFSFX = xIterator.Current( s_xFSFX );
        if( xFSFX.m_bEnabled )
        {
            xFSFX.Update();
        }
    }
}

void GLToy_FSFX_System::EnableFSFX( const GLToy_Hash uHash, const bool bEnabled )
{
    GLToy_FSFX* const pxFSFX = s_xFSFX.FindData( uHash );
    if( pxFSFX )
    {
        pxFSFX->m_bEnabled = bEnabled;
    }
}
