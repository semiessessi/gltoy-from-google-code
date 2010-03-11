/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Render/Font/GLToy_Font.h>

// GLToy
#include <Core/Data Structures/GLToy_BinaryTree.h>
#include <File/GLToy_ANSITextFile.h>
#include <File/GLToy_File_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_BinaryTree< GLToy_Font*, GLToy_Hash > GLToy_Font_System::s_xFonts;

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
        
        GLToy_DebugOutput( "   - Found font %S.\r\n", xName.GetWideString() );

        GLToy_ANSITextFile xFontFile = GLToy_ANSITextFile( xIterator.Current() );

        GLToy_String xFontData = xFontFile.GetString();

        if( xFontData.BeginsWith( "bitmap " ) )
        {

        }

        // s_xFonts.AddNode( GLToy_Font( xName ), xName.GetHash() );
    }

    return true;
}

void GLToy_Font_System::Shutdown()
{
    s_xFonts.Clear();
}