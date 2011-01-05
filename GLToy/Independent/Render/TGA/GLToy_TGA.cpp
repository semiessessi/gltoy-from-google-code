/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010, 2011 Semi Essessi
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
//
// Based on "LoadTGAFile" p. 223 OpenGL Game Programming (Astle & Hawkins)
//
/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Render/GLToy_Texture_System.h>

// GLToy
#include <File/GLToy_File.h>
#include <File/GLToy_File_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

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
        GLToy_Iterate( u_int, uCurrent, *this )
            const u_int uB = pcRawData[ xIterator.Index() * 3 ];
            const u_int uG = pcRawData[ xIterator.Index() * 3 + 1 ];
            const u_int uR = pcRawData[ xIterator.Index() * 3 + 2 ];
            uCurrent = 0xFF000000 | uR | ( uG << 8 ) | ( uB << 16 );
        GLToy_Iterate_End;

        delete[] pcRawData;
    }
    else
    {
        xStream.ByteAlignedWrite( reinterpret_cast< char* >( GetDataPointer() ), uRawByteCount );
        // swap BGRA -> RGBA
        GLToy_Iterate( u_int, uCurrent, *this )
            const u_int u_G_A = uCurrent & 0xFF00FF00;
            const u_int uR = ( uCurrent & 0xFF0000 ) >> 16;
            const u_int uB = uCurrent & 0xFF;
            uCurrent = u_G_A | uR | ( uB << 16 );
        GLToy_Iterate_End;
    }
}
