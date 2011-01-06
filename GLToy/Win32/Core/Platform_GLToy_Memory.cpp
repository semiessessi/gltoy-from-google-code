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

#include <Core/GLToy.h>

// This file's headers
#include <Core/GLToy_Memory.h>

// Win32
#include <windows.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void* GLToy_Memory::Platform_Allocate( const u_int uSize )
{
    return HeapAlloc( GetProcessHeap(), 0, uSize );
}

void GLToy_Memory::Platform_Free( void* const pxMemory )
{
    HeapFree( GetProcessHeap(), 0, pxMemory );
}

void GLToy_Memory::Platform_Swap_16Bytes( void* const px1, void* const px2 )
{
    Swap_8Bytes( px1, px2 );
    Swap_8Bytes( reinterpret_cast< int* >( px1 ) + 1, reinterpret_cast< int* >( px2 ) + 1 );
    //__asm
    //{
    //    movq xmm1, px1
    //    movq xmm2, px2
    //    movq px2, xmm1
    //    movq px1, xmm2
    //}
}

void GLToy_Memory::Platform_Swap_32Bytes( void* const px1, void* const px2 )
{
    __asm
    {
        movdqu xmm1, px1
        movdqu xmm2, px2
        movdqu px2, xmm1
        movdqu px1, xmm2
    }
}

void GLToy_Memory::Platform_Copy( void* const pxDestination, const void* const pxSource, const u_int uBytes )
{
    for( u_int u = 0; u < uBytes; ++u )
    {
        reinterpret_cast< char* const >( pxDestination )[ u ]
            = reinterpret_cast< const char* const >( pxSource )[ u ];
    }
    // TODO: some clever SSE crap with unrolled loops etc.
    //__asm
    //{
    //    movdqu xmm1, pxMemory...
    //}
}
