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