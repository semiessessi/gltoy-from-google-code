#ifndef __GLTOY_HASH_H_
#define __GLTOY_HASH_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

#define GLToy_Hash_Constant( string ) _GLToy_GetHash( string, sizeof( string ) - 1 )

//
// This implements Bernstein's hash, modified to be case-insensitive
//

GLToy_Inline GLToy_Hash GLToy_GetHash( const char* szString )
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

GLToy_Inline GLToy_Hash GLToy_GetHash( const wchar_t* wszString )
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

GLToy_Inline GLToy_Hash _GLToy_GetHash( const char* const szString, const u_int uLength )
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