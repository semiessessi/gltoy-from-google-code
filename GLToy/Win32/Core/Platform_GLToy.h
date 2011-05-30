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

#ifndef __PLATFORM_GLTOY_H_
#define __PLATFORM_GLTOY_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// M A C R O S
/////////////////////////////////////////////////////////////////////////////////////////////

#pragma warning( disable : 4985 ) // to work around obscure MS SDK issues

#define GLTOY_PLATFORM_WIN32

#ifdef GLTOY_DEMO
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#else
#define WIN32_LEAN
#endif

#ifndef GLTOY_DEMO
#define GLToy_ForceInline __forceinline
#ifndef GLTOY_DEBUG
#define GLToy_Inline __forceinline
#else
#define GLToy_Inline
#endif
#else
#define GLToy_ForceInline __inline
#define GLToy_Inline
#endif

#define GLToy_ForceCDecl __cdecl
#define GLToy_ForceStdCall __stdcall

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

unsigned int Win32_GLToy_GetDC();

#endif
