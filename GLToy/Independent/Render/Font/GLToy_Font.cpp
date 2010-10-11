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
#include <Render/Font/GLToy_Font.h>

// GLToy
#include <Core/Data Structures/GLToy_HashTree.h>
#include <File/GLToy_TextFile.h>
#include <File/GLToy_File_System.h>
#include <Render/Font/GLToy_Font_Bitmap.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_HashTree< GLToy_Font* > GLToy_Font_System::s_xFonts;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Font_System::Initialise()
{
    s_xFonts.Clear();

    GLToy_Array< GLToy_String > xFontPaths = GLToy_File_System::PathsFromFilter( "Fonts/", "*.font" );

    GLToy_ConstIterate( GLToy_String, xIterator, &xFontPaths )
    {
        GLToy_String xName = xIterator.Current();
        xName.RemoveAt( 0, 6 ); // remove "Fonts/"
        xName.RemoveFromEnd( 5 ); // remove .font
        
        GLToy_DebugOutput( "   - Found font \"%S\".\r\n", xName.GetWideString() );

        GLToy_TextFile xFontFile = GLToy_TextFile( xIterator.Current() );

        GLToy_String xFontData = xFontFile.GetString();

        if( xFontData.BeginsWith( "bitmap " ) )
        {
            xFontData.RemoveFirstWord();
            const u_int uSize = xFontData.ExtractUnsignedInt();
            xFontData.RemoveFirstWord();
            const GLToy_String xTextureName = xFontData.RemoveFirstWord();

            s_xFonts.AddNode( new GLToy_Font_Bitmap( xName, uSize, xTextureName ), xName.GetHash() );
        }
    }

    return true;
}

void GLToy_Font_System::Shutdown()
{
    s_xFonts.DeleteAll();
}

GLToy_Font* GLToy_Font_System::FindFont( const GLToy_Hash uHash )
{
    GLToy_Font** ppxFont = s_xFonts.FindData( uHash );
    return ppxFont ? *ppxFont : NULL;
}

GLToy_Font* GLToy_Font_System::LookUpFont( const GLToy_String& xName )
{
    GLToy_Font** ppxFont = s_xFonts.FindData( xName.GetHash() );
    return ppxFont ? *ppxFont : NULL;
}

void GLToy_Font_System::InitialiseFont( const GLToy_String& xName )
{
    GLToy_Font* pxFont = LookUpFont( xName );
    if( pxFont )
    {
        pxFont->Initialise();
    }
}

void GLToy_Font_System::ShutdownFont( const GLToy_String& xName )
{
    GLToy_Font* pxFont = LookUpFont( xName );
    if( pxFont )
    {
        pxFont->Shutdown();
    }
}

void GLToy_Font_System::RenderString( const GLToy_String& szString, const GLToy_String& xFontName, const float fX, const float fY )
{
    GLToy_Font* pxFont = LookUpFont( xFontName );
    if( pxFont )
    {
        pxFont->RenderString( szString, fX, fY );
    }
}
