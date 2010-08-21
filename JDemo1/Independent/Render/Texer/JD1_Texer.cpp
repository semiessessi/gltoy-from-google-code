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
#include <Render/Texer/JD1_Texer.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Maths/GLToy_Maths.h>
#include <Maths/GLToy_Noise.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_RenderFunctor.h>
#include <Render/GLToy_Texture.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void JD1_Texer::Render() const
{
    GLToy_PointerRenderFunctor< GLToy_Sprite* > xFunctor;
    m_xSprites.Traverse( xFunctor );
}

void JD1_Texer::Update()
{
    GLToy_Parent::Update();

    for( u_int u = m_xSprites.GetCount(); u < m_uPointCount; ++u )
    {
        m_xSprites.Append( new GLToy_Sprite() );
        m_xSprites[ u ]->SetTexture( m_uTexture );
    }

    GLToy_Vector_3 xCurrentPoint;
	GLToy_Vector_3 xCurrentColour = GLToy_Vector_3( 1.0f, 1.0f, 1.0f );
	float fParameter;
	float fFakeOsc;
	float fSizeX;
    float fSizeY;
	bool bSkip = false;

    for( u_int u = 0; u < m_uPointCount; ++u )
	{
		fParameter = static_cast< float >( u ) / static_cast< float >( m_uPointCount - 1 );
		fFakeOsc = GLToy_Noise::Fractal2D( fParameter * 20.0f + 5.0f * GLToy_Timer::GetTime(), 0.0f, 30.0f );

		PerPoint( fParameter, fFakeOsc, xCurrentPoint[ 0 ], xCurrentPoint[ 1 ], xCurrentPoint[ 2 ], xCurrentColour[ 0 ], xCurrentColour[ 1 ], xCurrentColour[ 2 ], bSkip, fSizeX, fSizeY );

		if( !bSkip )
		{
            m_xSprites[ u ]->SetPosition( xCurrentPoint );
            m_xSprites[ u ]->SetSize( fSizeX );
		}
	}
}