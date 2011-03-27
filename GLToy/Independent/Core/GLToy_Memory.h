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

#ifndef __GLTOY_MEMORY_H_
#define __GLTOY_MEMORY_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Memory
{

public:

    static bool Initialise();
    static void Shutdown();

    static void Set( void* const pxMemory, const u_int uBytes, const u_char ucValue );
    static GLToy_ForceInline void Set_1Byte( void* const pxMemory, const u_int uBytes, const u_char ucValue ) { Set( pxMemory, uBytes, ucValue ); }
    static void Set_4Bytes( void* const pxMemory, const u_int uDWords, const u_int uValue );
    
    // TODO: test and see if these are actually faster
    static GLToy_ForceInline void Swap_1Byte( void* const px1, void* const px2 )
    {
        char* const pc1 = reinterpret_cast< char* const >( px1 );
        char* const pc2 = reinterpret_cast< char* const >( px2 );
        *pc1 ^= *pc2; *pc2 ^= *pc1; *pc1 ^= *pc2;
    }

    static GLToy_ForceInline void Swap_2Bytes( void* const px1, void* const px2 )
    {
        short* const pc1 = reinterpret_cast< short* const >( px1 );
        short* const pc2 = reinterpret_cast< short* const >( px2 );
        *pc1 ^= *pc2; *pc2 ^= *pc1; *pc1 ^= *pc2;
    }

    static GLToy_ForceInline void Swap_4Bytes( void* const px1, void* const px2 )
    {
        int* const pc1 = reinterpret_cast< int* const >( px1 );
        int* const pc2 = reinterpret_cast< int* const >( px2 );
        *pc1 ^= *pc2; *pc2 ^= *pc1; *pc1 ^= *pc2;
    }

    static GLToy_ForceInline void Swap_8Bytes( void* const px1, void* const px2 )
    {
        long long* const pc1 = reinterpret_cast< long long* const >( px1 );
        long long* const pc2 = reinterpret_cast< long long* const >( px2 );
        *pc1 ^= *pc2; *pc2 ^= *pc1; *pc1 ^= *pc2;
    }

    static GLToy_ForceInline void Swap_16Bytes( void* const px1, void* const px2 ) { Platform_Swap_16Bytes( px1, px2 ); }
    static GLToy_ForceInline void Swap_32Bytes( void* const px1, void* const px2 ) { Platform_Swap_32Bytes( px1, px2 ); }

    static GLToy_ForceInline void MarkUninitialised( void* const pxMemory, const u_int uBytes ) { Set( pxMemory, uBytes, 0xCC ); }
    static GLToy_ForceInline void MarkDestroyed( void* const pxMemory, const u_int uBytes ) { Set( pxMemory, uBytes, 0xDD ); }
    
    static GLToy_ForceInline void Copy( void* const pxDestination, const void* const pxSource, const u_int uBytes ) { Platform_Copy( pxDestination, pxSource, uBytes ); }
    static GLToy_ForceInline void Zero( void* const pxMemory, const u_int uBytes ) { Set( pxMemory, uBytes, 0 ); }

    template < int iBytes >
    static void FixedCopy( void* const pxDestination, const void* const pxSource )
    {    
        for( int u = 0; u < ( iBytes >> 2 ); ++u )
        {
            reinterpret_cast< int* const >( pxDestination )[ u ]
                = reinterpret_cast< const int* const >( pxSource )[ u ];
        }

        for( int u = iBytes - ( iBytes & 3 ); u < iBytes; ++u )
        {
            reinterpret_cast< char* const >( pxDestination )[ u ]
                = reinterpret_cast< const char* const >( pxSource )[ u ];
        }
    }

    static void* Platform_Allocate( const u_int uSize );
    static void Platform_Free( void* const pxMemory );

    static void Platform_Swap_16Bytes( void* const px1, void* const px2 );
    static void Platform_Swap_32Bytes( void* const px1, void* const px2 );

    static void Platform_Copy( void* const pxDestination, const void* const pxSource, const u_int uBytes );

};

/////////////////////////////////////////////////////////////////////////////////////////////
// D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

#ifdef GLTOY_DEBUG
void* GLToy_ForceCDecl operator new( u_int uSize, const char* szFile, const int iLine );
void* GLToy_ForceCDecl operator new[]( u_int uSize, const char* szFile, const int iLine );
void GLToy_ForceCDecl operator delete( void* pxMemory, const char* szFile, const int iLine );
void GLToy_ForceCDecl operator delete[]( void* pxMemory, const char* szFile, const int iLine );
#else
#ifndef GLTOY_DEMO
void* GLToy_ForceCDecl operator new( u_int uSize );
void* GLToy_ForceCDecl operator new[]( u_int uSize );
#endif
#endif
#ifndef GLTOY_DEMO
void GLToy_ForceCDecl operator delete( void* pxMemory );
void GLToy_ForceCDecl operator delete[]( void* pxMemory );
#endif

#endif
