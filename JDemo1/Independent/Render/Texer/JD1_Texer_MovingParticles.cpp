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

#include <Core/JD1.h>

// This file's header
#include <Render/Texer/JD1_Texer_MovingParticles.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// M A C R O S
/////////////////////////////////////////////////////////////////////////////////////////////

#define acos GLToy_Maths::ACos
#define cos GLToy_Maths::Cos
#define sin GLToy_Maths::Sin
#define sqrt GLToy_Maths::Sqrt
#define pow GLToy_Maths::Pow
#define max GLToy_Maths::Max
#define abs GLToy_Maths::Abs

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

static u_int g_uPoint = 0;
static bool g_bBeat = false;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void JD1_Texer_MovingParticles::Initialise()
{
    m_uPointCount = 3;
}

void JD1_Texer_MovingParticles::PerFrame()
{
    g_uPoint = 0;
    g_bBeat = false;
}

void JD1_Texer_MovingParticles::OnBeat()
{
    g_bBeat = true;
}

void JD1_Texer_MovingParticles::PerPoint(
    const float i,
    const float v,
    float& x, float& y, float& z,
    float& red, float& green, float& blue,
    bool& skip,
    float& sizex,
    float& sizey ) const
{
    const float fScale = g_bBeat ? 1.0f : 0.7f;

    switch( g_uPoint )
    {
        case 0:
        {
            sizex = sizey = fScale * 0.1f;
            red = 0.9f; green = 0.1f; blue = 0.1f;
            x = 0.5f;
            y = 0.0f;
            z = 1.0f;
            break;
        }

        case 1:
        {
            sizex = sizey = fScale * 0.3f;
            red = 0.1f; green = 0.7f; blue = 0.0f;
            x = 0.0f;
            y = 0.0f;
            z = 1.0f;
            break;
        }

        case 2:
        {
            sizex = sizey = fScale * 0.4f;
            red = 0.3f; green = 0.6f; blue = 1.0f;
            x = -0.4f;
            y = 0.3f;
            z = 1.0f;
            break;
        }
    }

    ++g_uPoint;
}
