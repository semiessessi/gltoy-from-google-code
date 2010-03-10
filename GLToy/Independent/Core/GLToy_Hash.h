#ifndef __GLTOY_HASH_H_
#define __GLTOY_HASH_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

#define GLToy_Hash_Constant( string ) _GLToy_GetHash( string, sizeof( string ) - 1 )

//
// This implements Bernstein's hash
//

GLToy_Inline GLToy_Hash GLToy_GetHash( const char* szString )
{
    GLToy_Hash uHash = 0;

    while( *szString )
    {
        uHash += uHash << 5;
        uHash += *szString;
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
        uHash += *wszString;
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
        uHash += szString[ u ];
    }

    return uHash;
}

#endif