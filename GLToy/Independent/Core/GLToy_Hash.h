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

#ifndef __GLTOY_HASH_H_
#define __GLTOY_HASH_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

#define GLToy_Hash_Constant( string ) _GLToy_GetHash( string, sizeof( string ) - 1 )

//
// This implements Bernstein's hash, modified to be case-insensitive
//

GLToy_ForceInline GLToy_Hash GLToy_GetHash( const char* szString )
{
    GLToy_Hash uHash = 0;

    while( *szString )
    {
        uHash += uHash << 5;
        uHash += ( *szString < 'a' ) ? *szString : ( *szString - 'a' + 'A' );
        ++szString;
    }

    return uHash;
}

GLToy_ForceInline GLToy_Hash GLToy_GetHash( const wchar_t* wszString )
{
    GLToy_Hash uHash = 0;

    while( *wszString )
    {
        uHash += uHash << 5;
        uHash += ( *wszString < 'a' ) ? *wszString : ( *wszString - 'a' + 'A' );
        ++wszString;
    }

    return uHash;
}

//
// This version is designed to compile out
//

GLToy_ForceInline GLToy_Hash _GLToy_GetHash( const char* const szString, const u_int uLength )
{
    GLToy_Hash uHash = 0;

    for( u_int u = 0; u < uLength; ++u )
    {
        uHash += uHash << 5;
        uHash += ( szString[ u ] < 'a' ) ? szString[ u ] : ( szString[ u ] - 'a' + 'A' );
    }

    return uHash;
}

#endif
