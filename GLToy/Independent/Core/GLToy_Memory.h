/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
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

    static void MarkUninitialised( void* const pxMemory, const u_int uBytes ) { Set( pxMemory, uBytes, 0xCC ); }
    static void MarkDestroyed( void* const pxMemory, const u_int uBytes ) { Set( pxMemory, uBytes, 0xDD ); }
    static void Zero( void* const pxMemory, const u_int uBytes ) { Set( pxMemory, uBytes, 0 ); }

    static void* Platform_Allocate( const u_int uSize );
    static void Platform_Free( void* const pxMemory );
};

/////////////////////////////////////////////////////////////////////////////////////////////
// D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

#ifdef GLTOY_DEBUG
void* GLToy_ForceCDecl operator new( u_int uSize, const char* szFile, const int iLine );
void GLToy_ForceCDecl operator delete( void* pxMemory, const char* szFile, const int iLine );
#else
#ifndef GLTOY_DEMO
void* GLToy_ForceCDecl operator new( u_int uSize );
#endif
#endif
#ifndef GLTOY_DEMO
void GLToy_ForceCDecl operator delete( void* pxMemory );
#endif

#endif
