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

#ifndef __FSDEMO_RENDER_H_
#define __FSDEMO_RENDER_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// FSDemo
#include <FridgeScript/FSDemo_FridgeScript.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class FSDemo_Render
{

public:

    static bool Initialise();
    static void Shutdown();

    static void Render();

    static float FRIDGE_API FSAPI_Translate( float fX, float fY, float fZ );
    static float FRIDGE_API FSAPI_Rotate( float fX, float fY, float fZ, float fAngle );
    static float FRIDGE_API FSAPI_GetTime();
    static float FRIDGE_API FSAPI_StartTriangles();
    static float FRIDGE_API FSAPI_EndTriangles();
    static float FRIDGE_API FSAPI_Triangle(     float fV1X, float fV1Y, float fV1Z,
                                                float fV2X, float fV2Y, float fV2Z,
                                                float fV3X, float fV3Y, float fV3Z,
                                                float fColourRed, float fColourGreen, float fColourBlue
                                            );

private:

    static void Platform_Render();

    static FSDemo_FridgeScript* s_pxScript;

};

#endif
