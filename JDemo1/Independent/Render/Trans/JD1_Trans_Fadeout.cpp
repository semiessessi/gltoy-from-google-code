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

#include <Core/JD1.h>

// This file's header
#include <Render/Trans/JD1_Trans_Fadeout.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Maths/GLToy_Maths.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void JD1_Trans_Fadeout::Render() const
{
    static float fRunningAlpha = 0.0f;
    const float fDAlpha = 1.0f - GLToy_Maths::Pow( 1.0f - m_fAmount, 10.0f * GLToy_Timer::GetFrameTime() );
    fRunningAlpha += fDAlpha;
    float fAlpha = 0.0f;
    while( fRunningAlpha > ( 1.0f / 255.0f ) )
    {
        fRunningAlpha -= ( 1.0f / 255.0f );
        fAlpha += ( 1.0f / 255.0f );
    }

    GLToy_Render::SetOrthogonalProjectionMatrix();
    GLToy_Render::PushViewMatrix();
    GLToy_Render::SetIdentityViewMatrix();

    GLToy_Texture_System::BindBlack();

    GLToy_Render::DisableDepthWrites();
    GLToy_Render::DisableDepthTesting();
    GLToy_Render::EnableBlending();
    GLToy_Render::SetBlendFunction( BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA );

    GLToy_Render::StartSubmittingQuads();

    GLToy_Render::SubmitColour( GLToy_Vector_4( 0.0f, 0.0f, 0.0f, fAlpha ) );
    GLToy_Render::SubmitTexturedQuad2D( GLToy_Vector_2( -0.5f * GLToy_Render::Get2DWidth(), -1.0f ), GLToy_Vector_2( GLToy_Render::Get2DWidth(), 2.0f ) );

    GLToy_Render::EndSubmit();

    GLToy_Render::EnableDepthTesting();
    GLToy_Render::EnableDepthWrites();
    GLToy_Render::DisableBlending();

    GLToy_Render::SetPerspectiveProjectionMatrix();
    GLToy_Render::PopViewMatrix();
}
