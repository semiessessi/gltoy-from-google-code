/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Render/FSDemo_Render.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Maths/GLToy_Vector.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

FSDemo_FridgeScript* FSDemo_Render::s_pxScript = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool FSDemo_Render::Initialise()
{
    GLTOY_INITIALISER_CALL( FSDemo_FridgeScript );

    // register API
    FSDemo_FridgeScript::RegisterAPI( "GetTime", FSAPI_GetTime, 0 );
    FSDemo_FridgeScript::RegisterAPI( "Rotate", FSAPI_Rotate, 4 );
    FSDemo_FridgeScript::RegisterAPI( "Translate", FSAPI_Translate, 3 );
    FSDemo_FridgeScript::RegisterAPI( "StartTriangles", FSAPI_StartTriangles, 0 );
    FSDemo_FridgeScript::RegisterAPI( "EndTriangles", FSAPI_EndTriangles, 0 );
    FSDemo_FridgeScript::RegisterAPI( "Triangle", FSAPI_Triangle, 12 );

    // initialise the script
    s_pxScript = FSDemo_FridgeScript::CreateFromFile( "script.fs" );

    if( !s_pxScript )
    {
        return false;
    }

    return true;
}

void FSDemo_Render::Shutdown()
{
    // clean up the script
    delete s_pxScript;

    FSDemo_FridgeScript::Shutdown();
}

void FSDemo_Render::Render()
{
    // run the script
    s_pxScript->Execute();
}

// FridgeScript rendering API
float FRIDGE_API FSDemo_Render::FSAPI_GetTime()
{
    return GLToy_Timer::GetTime();
}

float FRIDGE_API FSDemo_Render::FSAPI_Translate( float fX, float fY, float fZ )
{
    GLToy_Render::Translate( GLToy_Vector_3( fX, fY, fZ ) );

    return 0.0f;
}

float FRIDGE_API FSDemo_Render::FSAPI_Rotate( float fX, float fY, float fZ, float fAngle )
{
    GLToy_Render::Rotate( GLToy_Vector_3( fX, fY, fZ ), fAngle );

    return 0.0f;
}

float FRIDGE_API FSDemo_Render::FSAPI_StartTriangles()
{
    GLToy_Texture_System::BindWhite();
    GLToy_Render::StartSubmittingTriangles();

    return 0.0f;
}

float FRIDGE_API FSDemo_Render::FSAPI_EndTriangles()
{
    GLToy_Render::EndSubmit();

    return 0.0f;
}

float FRIDGE_API FSDemo_Render::FSAPI_Triangle(     float fV1X, float fV1Y, float fV1Z,
                                                    float fV2X, float fV2Y, float fV2Z,
                                                    float fV3X, float fV3Y, float fV3Z,
                                                    float fColourRed, float fColourGreen, float fColourBlue
                                                )
{
    GLToy_Render::SubmitColour( GLToy_Vector_3( fColourRed, fColourGreen, fColourBlue ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( fV1X, fV1Y, fV1Z ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( fV2X, fV2Y, fV2Z ) );
    GLToy_Render::SubmitVertex( GLToy_Vector_3( fV3X, fV3Y, fV3Z ) );

    return 0.0f;
}