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

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// This file's header
#include <Core/JD1.h>

// GLToy
#include <Core/State/GLToy_State_System.h>
#include <Render/GLToy_Render.h>

// JD1
#include <Demo/JD1_Demo_System.h>
#include <Demo/JD1_DemoScene_NoiseTest.h>

#include <Maths/GLToy_Maths.h>
#include <Render/GLToy_Texture.h>
#include <Render/GLToy_Texture_Procedural.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool JD1::Initialise()
{
    GLToy_Texture_Procedural xTest;

    xTest.AppendFillLayer( 0xFFFF0000 );
    
    u_int uID = xTest.AppendCheckerboardLayer();
    xTest.SetBlendMode( uID, GLToy_Texture_Procedural::BLEND_SUB );

    xTest.AppendTileLayer( 4 );

    xTest.CreateTexture( "Test_1", 256, 256 );

    xTest.SaveToCPPHeader( "Test_1" );
    
    GLToy_Texture* pxTexture = GLToy_Texture_System::FindTexture( GLToy_Hash_Constant( "Test_1" ) );
    GLToy_Texture_System::SaveTextureTGA( "Test_1", pxTexture->GetRGBADataPointer(), 256, 256 );

    //GLToy_Texture_Procedural xTest2;

    //xTest2.AppendFillLayer( 0xFF3F7FFF );
    //
    //u_int uNoiseID = xTest2.AppendFBMNoiseLayer( 2.0f );
    //xTest2.SetBlendMode( uNoiseID, GLToy_Texture_Procedural::BLEND_ADD );
    //uNoiseID = xTest2.AppendFBMNoiseLayer( 4.0f );
    //xTest2.SetBlendMode( uNoiseID, GLToy_Texture_Procedural::BLEND_SUB );
    //u_int uGradientID = xTest2.AppendGradientLayer( GLToy_Texture_Procedural::GRADIENT_RADIAL_IN );
    //xTest2.SetBlendMode( uGradientID, GLToy_Texture_Procedural::BLEND_LUMINANCE_INTO_ALPHA );

    //xTest2.CreateTexture( "Test_2", 256, 256 );

    //xTest2.SaveToCPPHeader( "Test_2" );
    //
    //pxTexture = GLToy_Texture_System::FindTexture( GLToy_Hash_Constant( "Test_2" ) );
    //GLToy_Texture_System::SaveTextureTGA( "Test_2", pxTexture->GetRGBADataPointer(), 256, 256 );

    //GLToy_Texture_Procedural xTest3;

    //xTest3.AppendFillLayer( 0xFF00FF00 );
    //
    //uGradientID = xTest3.AppendGradientLayer( GLToy_Texture_Procedural::GRADIENT_TOP );
    //xTest3.SetBlendMode( uGradientID, GLToy_Texture_Procedural::BLEND_MUL );

    //xTest3.CreateTexture( "Test_3", 256, 256 );

    //xTest3.SaveToCPPHeader( "Test_3" );

    //pxTexture = GLToy_Texture_System::FindTexture( GLToy_Hash_Constant( "Test_3" ) );
    //GLToy_Texture_System::SaveTextureTGA( "Test_3", pxTexture->GetRGBADataPointer(), 256, 256 );

    //GLToy_Texture_Procedural xTest4;

    //xTest4.AppendFillLayer( 0xFF3F3F3F );
    //
    //xTest4.AppendCircleLayer( GLToy_Vector_2( 0.5f, 0.5f ), 0.25f, 0xFFFFFFFF );
    //xTest4.AppendTileLayer( 4 );

    //xTest4.CreateTexture( "Test_4", 256, 256 );

    //xTest4.SaveToCPPHeader( "Test_4" );

    //pxTexture = GLToy_Texture_System::FindTexture( GLToy_Hash_Constant( "Test_4" ) );
    //GLToy_Texture_System::SaveTextureTGA( "Test_4", pxTexture->GetRGBADataPointer(), 256, 256 );

    GLToy::ChangeWindowTitle( "JDemo 1" );

    GLToy_InitialiserCall( JD1_Demo_System );

    GLToy_State_System::ChangeState( GLToy_Hash_Constant( "FixedCamera" ) );

    JD1_Demo_System::Queue( new JD1_DemoScene_NoiseTest() );// , 30.0f, JD1_DEMO_FLASH_WHITE, 0.4f );

    return true;
}

void JD1::Shutdown()
{
    JD1_Demo_System::Shutdown();
}

void JD1::Update()
{
    JD1_Demo_System::Update();
}
