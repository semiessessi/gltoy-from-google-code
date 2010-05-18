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
#include <Render/Font/GLToy_Font.h>

// GLToy
#include <Core/Data Structures/GLToy_HashTree.h>
#include <File/GLToy_ANSITextFile.h>
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

        GLToy_ANSITextFile xFontFile = GLToy_ANSITextFile( xIterator.Current() );

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
