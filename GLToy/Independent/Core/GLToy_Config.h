/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010 Semi Essessi
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

#ifndef __GLTOY_CONFIG_H_
#define __GLTOY_CONFIG_H_

#ifndef GLTOY_PLATFORM_X11
    // keep FridgeScript out of final builds for now
	// SE - 18/12/2010 - actually keep it out of everything and I can fix FSDemo later if I actually care...
    //#ifndef _FINAL
    //    #define GLTOY_USE_FRIDGESCRIPT         (1)
    //#endif
    
    #ifdef _HAVOK
        #define GLTOY_USE_HAVOK_PHYSICS        (1)
		#define GLTOY_HAVOK_VERSION            (2010)
    #endif

    #ifdef _BULLET
        #define GLTOY_USE_BULLET_PHYSICS       (1) 
    #endif
#endif

#endif
