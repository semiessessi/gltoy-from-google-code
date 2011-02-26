/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2011 Semi Essessi
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

#include <Core/GLToy.h>

// This file's header
#include <Render/X_Starfield.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Maths/GLToy_Maths.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Render_Metrics.h>
#include <Render/GLToy_Texture_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const GLToy_Vector_3 xOFFSET_X( 1.0f, 0.0f, 0.0f );
static const GLToy_Vector_3 xOFFSET_Y( 0.0f, 1.0f, 0.0f );

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

static GLToy_Vector_3 s_xPositions[ uNUM_STARS ];
static float s_fSizes[ uNUM_STARS ];

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void X_Starfield::Render() const
{
    GLToy_Render::RegisterTransparent( this, 100.0f );
}

void X_Starfield::RenderTransparent() const
{
    GLToy_Render_Metrics::IncrementTriangleCount( uNUM_STARS << 1 );

    GLToy_Render::SetBlendFunction( BLEND_ONE, BLEND_ONE );

    for( u_int u = 0; u < uNUM_STARS; ++u )
    {
        switch( u )
        {
            case 0:
            {
                GLToy_Texture_System::BindTexture( GLToy_Hash_Constant( "Background_Star_1.png" ) );
                GLToy_Render::StartSubmittingQuads();
                break;
            }
            
            case ( uNUM_STARS >> 2 ):
            {
                GLToy_Render::EndSubmit();
                GLToy_Texture_System::BindTexture( GLToy_Hash_Constant( "Background_Star_2.png" ) );
                GLToy_Render::StartSubmittingQuads();
                break;
            }
            
            case ( uNUM_STARS >> 1 ):
            {
                GLToy_Render::EndSubmit();
                GLToy_Texture_System::BindTexture( GLToy_Hash_Constant( "Background_Star_3.png" ) );
                GLToy_Render::StartSubmittingQuads();
                break;
            }
            
            case ( ( uNUM_STARS >> 2 ) + ( uNUM_STARS >> 1 ) ):
            {
                GLToy_Render::EndSubmit();
                GLToy_Texture_System::BindTexture( GLToy_Hash_Constant( "Background_Star_4.png" ) );
                GLToy_Render::StartSubmittingQuads();
                break;
            }
            
            default: break;
        }

        GLToy_Render::SubmitUV( GLToy_Vector_3( 1.0f, 1.0f, 0.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( s_xPositions[ u ] ) + s_fSizes[ u ] * ( xOFFSET_X + xOFFSET_Y ) );
        GLToy_Render::SubmitUV( GLToy_Vector_3( 0.0f, 1.0f, 0.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( s_xPositions[ u ] ) - s_fSizes[ u ] * ( xOFFSET_X - xOFFSET_Y ) );
        GLToy_Render::SubmitUV( GLToy_Vector_3( 0.0f, 0.0f, 0.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( s_xPositions[ u ] ) - s_fSizes[ u ] * ( xOFFSET_X + xOFFSET_Y ) );
        GLToy_Render::SubmitUV( GLToy_Vector_3( 1.0f, 0.0f, 0.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( s_xPositions[ u ] ) + s_fSizes[ u ] * ( xOFFSET_X - xOFFSET_Y ) );
    }

    GLToy_Render::EndSubmit();

    GLToy_Render::SetBlendFunction( BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA );
}

void X_Starfield::InitialiseData()
{
    for( u_int u = 0; u < uNUM_STARS; ++u )
    {
        s_xPositions[ u ] = GLToy_Vector_3( GLToy_Maths::Random( -10.0f, 10.0f ), GLToy_Maths::Random( -10.0f, 10.0f ), ( u & 1 ) ? 7.5f : 5.0f );
        s_fSizes[ u ] = GLToy_Maths::Random( 0.1f, 0.2f );
    }
}

void X_Starfield::Update()
{
    for( u_int u = 0; u < uNUM_STARS; ++u )
    {
        s_xPositions[ u ].y -= GLToy_Timer::GetFrameTime();

        if( s_xPositions[ u ].y < -10.0f )
        {
            s_xPositions[ u ].y = 10.0f + GLToy_Maths::Random();
        }
    }
}
