/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009, 2010 Semi Essessi
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
#include <File/GLToy_TextFile.h>

// C/C++ headers
#include <stdio.h>
#include <string.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_TextFile::GLToy_TextFile( const GLToy_String& szFilename )
: GLToy_Parent( szFilename )
{
}

GLToy_TextFile::~GLToy_TextFile()
{
}

void GLToy_TextFile::GetRawString( char* szString ) const
{
    GetAllData( szString );
    szString[ m_uSize ] = 0;
}

GLToy_String GLToy_TextFile::GetString() const
{
    GLToy_String szReturnValue;

    if( GetSize() < 1 )
    {
        return szReturnValue;
    }

    u_char* szData = new u_char[ GetSize() ];
    GetRawString( reinterpret_cast< char* >( szData ) );
    
    enum Format
    {
        ANSI,
        UTF8,
        UTF16LE,
        UTF16BE,
        UTF32LE,
        UTF32BE
    } eFormat;

    u_int uPos = 0;
    eFormat = ANSI;
		
    if( szData[ 0 ] == 0xEF )	// could be the UTF-8 BOM
    {
	    if( ( szData[ 1 ] == 0xBB ) && ( szData[ 2 ] == 0xBF ) )
	    {
		    // make sure we don't overwrite format... even though should never happen
		    eFormat = UTF8;
		    uPos = 3;
	    }
    }
    else if( szData[ 0 ] == 0xFF ) // could be the little endian UTF-16 or UTF-32 BOM
    {
	    if( szData[ 1 ] == 0xFE ) 
	    {
		    if( ( szData[ 2 ] == 0x00 ) && ( szData[ 3 ] == 0x00 ) )
		    {
			    eFormat = UTF32LE;
			    uPos = 4;
		    }
		    else
		    {
			    eFormat = UTF16LE;
			    uPos = 2;
		    }
	    }
    }
    else if( ( szData[ 0 ] == 0xFE ) && ( szData[ 1 ] == 0xFF ) ) // this is the big endian UTF-16 BOM
    {
	    eFormat = UTF16BE;
	    uPos = 2;
    }
    else if( ( szData[ 0 ] == 0x00 ) && ( szData[ 1 ] == 0x00 ) && ( szData[ 2 ] == 0xFE ) && ( szData[ 3 ] == 0xFF ) ) // this is the big endian UTF-32 BOM
    {
	    eFormat = UTF32BE;
	    uPos = 4;
    }
    
    switch( eFormat )
    {
        case UTF8:
        {
            // go through byte by byte and add to data if less than 0x80, otherwise decode
			// note that this decodes the character properly, then throws away any chars that can't be represented by GLToy
			// should meet requirements mentioned in RFC3629 - http://tools.ietf.org/html/rfc3629

			for( u_int u = uPos; u < GetSize(); ++u )
			{
				if( szData[ u ] < 0x80 )
				{
					szReturnValue += szData[ u ];
				}
				else
				{
					// check how many aucBytes in sequence encoded and break out into their own array
					u_int uNumBytes = 0;
					u_int uTempChar = 0;

					if( szData[ u ] & 0x80 ) ++uNumBytes;
					if( szData[ u ] & 0x40 ) ++uNumBytes;
					if( szData[ u ] & 0x20 ) ++uNumBytes;
					if( szData[ u ] & 0x10 ) ++uNumBytes; // allows up to 4 aucBytes of UTF-8

					unsigned char aucBytes[ 4 ];
					for( u_int j = 0; j < uNumBytes; ++j )
                    {
                        aucBytes[ j ] = szData[ u + j ];
                    }

					// we need to take the lowest 6-bits from each byte, from lowest to highest and concatenate them into the UCS16 2-aucBytes
					// anything outside of the UCS2 range should be replaced with a "?" for now.
					u_int k = uNumBytes - 1;
                    u += k;
					// shift each of the trailing aucBytes and add as appropriate
					for( u_int l = 0; k > 0; --k, l += 6 )
					{
						uTempChar |= ( static_cast< u_int >( aucBytes[ k ] & 0x3F ) << l );
					}
					
                    const unsigned char aucMask[] = { 0xFF, 0x7F, 0x3F, 0x1F, 0x0F };
					// now take the leading byte and add the bits from it
					uTempChar |= ( ( u_int )( aucBytes[ k ] & aucMask[ uNumBytes ] ) << ( ( uNumBytes - 1 ) * 6 ) );
					
					// check if the codepoint is valid and supported by GLToy and add the appropriate character to the data
#ifdef GLTOY_PLATFORM_WIN32
					if( uTempChar > 0xFFFF )
                    {
                        szReturnValue += "?";
                    }
					else
#endif
                    {
                        szReturnValue += static_cast< wchar_t >( uTempChar );
                    }
				}
			}

			break;
        }

        case UTF16LE:
        {
			// go through 2 bytes at a time and decode
			// as with UTF-8 decoder above this will fully decode the character before converting to UCS2
			// this is horrible... UCS2 != UTF-16 despite the common opinion. so the "obvious shortcut" is in fact a deathtrap
			// should meet requirements mentioned in RFC2781 - http://tools.ietf.org/html/rfc2781

			for( u_int u = uPos; u < ( GetSize() - 1 ); u += 2 )
			{
				unsigned short usWord = ( static_cast< u_short >( szData[ u + 1 ] ) << 8 ) | static_cast< u_short >( szData[ u ] );
				if( ( usWord < 0xDC00 ) || ( usWord > 0xDFFF ) )
				{
					// single word character - just dump straight into the data
					szReturnValue += static_cast< wchar_t >( usWord );
				}
                // TODO: improve this if possible
				else if ( usWord > 0xDBFF ) // pick out some invalid characters and complain with a "?" for now...
				{
					szReturnValue += "?";
				}
				else
				{
					// two word character ( higher number of words are essentially forbidden )
					// we have the low-order word in usWord...
					u += 2;
					u_int uTempChar = ( static_cast< u_int >( ( static_cast< u_short >( szData[ u + 3 ] ) << 8 ) | static_cast< u_short >( szData[ u + 2 ] ) ) & 0xFFFF ) << 10;
					uTempChar = uTempChar | ( ( static_cast< u_int >( usWord ) ) & 0xFFFF );

#ifdef GLTOY_PLATFORM_WIN32
					// Unicode allows for more code points... but GLToy does not... at least under Windows :(
					if( uTempChar > 0xFFFF )
					{
						szReturnValue += "?";
					}
					else
#endif
                    {
						szReturnValue += static_cast< wchar_t >( uTempChar );
                    }
					
				}
			}
			break;
        }

        case UTF16BE:
        {
            // this is just the above with the byte order swapped

			for( u_int u = uPos; u < ( GetSize() - 1 ); u += 2 )
			{
				u_short usWord = ( static_cast< u_short >( szData[ u ] ) << 8 ) | static_cast< u_short >( szData[ u + 1 ] );
				if( ( usWord < 0xDC00 ) || ( usWord > 0xDFFF ) )
				{
					// single word character - just dump straight into the data
					szReturnValue += static_cast< wchar_t >( usWord );
				}
				else if ( usWord > 0xDBFF ) // pick out some invalid characters and complain with a "?" for now...
				{
					szReturnValue += "?";
				}
				else
				{
					// two word character ( higher number of words are essentially forbidden )
					// we have the low-order word in usWord...
					u += 2;
					u_int uTempChar = ( static_cast< u_int >( ( static_cast< u_short >( szData[ u + 2 ] ) << 8 ) | static_cast< u_short >( szData[ u + 3 ] ) ) & 0xFFFF ) << 10;
					uTempChar = uTempChar | ( ( static_cast< u_int >( usWord ) ) & 0xFFFF );
					
#ifdef GLTOY_PLATFORM_WIN32
					// Unicode allows for more code points... but GLToy does not... at least under Windows :(
					if( uTempChar > 0xFFFF )
					{
						szReturnValue += "?";
					}
					else
#endif
					{
						szReturnValue += static_cast< wchar_t >( uTempChar );
					}
					
				}
			}		
			break;
        }

		case UTF32LE:
        {
			// this roughly complies with the Unicode Standard 4.1 section 3.10, D43-45
			// however to allow all jEngine builds to handle the results it throws away all code points > 0xFFFF and replaces them with ? ( for now )
			
            for( u_int u = uPos; u < ( GetSize() - 3  ); u += 4 )
			{
				// construct the character
				u_int uTempChar = static_cast< u_int >( szData[ u ] )
								| static_cast< u_int >( szData[ u + 1 ] ) << 8
								| static_cast< u_int >( szData[ u + 2 ] ) << 16
								| static_cast< u_int >( szData[ u + 3 ] ) << 24;
											
#ifdef GLTOY_PLATFORM_WIN32
					// Unicode allows for more code points... but GLToy does not... at least under Windows :(
					if( uTempChar > 0xFFFF )
					{
						szReturnValue += "?";
					}
					else
#endif
				{
					szReturnValue += static_cast< wchar_t >( uTempChar );
				}
			}
			break;
        }
			
		case UTF32BE:
        {
			// just swapped byte order from above

			for( u_int u = uPos; u < ( GetSize() - 3  ); u += 4 )
			{
				// construct the character
				u_int uTempChar = static_cast< u_int >( szData[ u + 3 ] )
								| static_cast< u_int >( szData[ u + 2 ] ) << 8
								| static_cast< u_int >( szData[ u + 1 ] ) << 16
								| static_cast< u_int >( szData[ u ] ) << 24;
											
#ifdef GLTOY_PLATFORM_WIN32
					// Unicode allows for more code points... but GLToy does not... at least under Windows :(
					if( uTempChar > 0xFFFF )
					{
						szReturnValue += "?";
					}
					else
#endif
				{
					szReturnValue += static_cast< wchar_t >( uTempChar );
				}
			}
			break;
        }

        case ANSI:
        default:
        {
            szReturnValue = szData;
            break;
        }
    }

    delete szData;

    return szReturnValue;
}
