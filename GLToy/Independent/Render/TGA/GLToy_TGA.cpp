/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Render/GLToy_Texture.h>

// GLToy
#include <File/GLToy_File.h>
#include <File/GLToy_File_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

//
// LoadTGA
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// Based on "LoadTGAFile" p. 223 OpenGL Game Programming (Astle & Hawkins)
//

void GLToy_Texture::LoadTGA()
{
    // TODO - if file exists

    GLToy_File xTGAFile( GLToy_String( "textures/" ) + m_szName );

    GLToy_BitStream xStream;
    xTGAFile.ReadToBitStream( xStream );

    xStream.IgnoreNBytes( 2 ); // the docs I'm working from don't explain these bytes

    u_char ucType = 0;

    xStream >> ucType;

    // we can only support uncompressed rgb for now
    switch( ucType )
    {
        case 2: // RGB(A)
        // case 3: // L
        // case 10: // RLE RGB(A)
        // case 11: // RLE L
        {
            break;
        }

        default:
        {
            GLToy_Assert( false, "Unsupported TGA type: %u", static_cast< u_int >( ucType ) );
            return;
        }
    }

    // NOTE: Book says 13, but it can't count looking at the code...
    xStream.IgnoreNBytes( 9 ); // the docs I'm working from don't explain these bytes

    short sWidth = 0;
    short sHeight = 0;
    xStream >> sWidth;
    xStream >> sHeight;

    m_uWidth = sWidth;
    m_uHeight = sHeight;

    u_char ucBPP;
    xStream >> ucBPP;

    xStream.IgnoreNBytes( 1 );

    // can assume we have uncompressed rgb(a)

    Resize( static_cast< u_int >( sWidth ) * static_cast< u_int >( sHeight ) );
    const u_int uRawByteCount = GetCount() * ( ucBPP >> 3 );

    if( ucBPP == 24 )
    {
        char* pcRawData = new char[ uRawByteCount ];
        xStream.ByteAlignedWrite( pcRawData, uRawByteCount );

        // now we have some BGR data, insert it into the texture
        GLToy_Iterate( u_int, xIterator, this )
        {
            const u_int uB = pcRawData[ xIterator.Index() * 3 ];
            const u_int uG = pcRawData[ xIterator.Index() * 3 + 1 ];
            const u_int uR = pcRawData[ xIterator.Index() * 3 + 2 ];
            xIterator.Current() = 0xFF000000 | uR | ( uG << 8 ) | ( uB << 16 );
        }

        delete[] pcRawData;
    }
    else
    {
        xStream.ByteAlignedWrite( reinterpret_cast< char* >( GetDataPointer() ), uRawByteCount );
        // swap BGRA -> RGBA
        GLToy_Iterate( u_int, xIterator, this )
        {
            const u_int u_G_A = xIterator.Current() & 0xFF00FF00;
            const u_int uR = ( xIterator.Current() & 0xFF0000 ) >> 16;
            const u_int uB = xIterator.Current() & 0xFF;
            xIterator.Current() = u_G_A | uR | ( uB << 16 );
        }
    }
}