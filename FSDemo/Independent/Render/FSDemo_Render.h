/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009, 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of FSDemo.
//
// FSDemo is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// FSDemo is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with FSDemo.
// If not, see <http://www.gnu.org/licenses/>.
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