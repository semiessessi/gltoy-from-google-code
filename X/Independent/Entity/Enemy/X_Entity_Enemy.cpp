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

#include <Core/GLToy.h>

// This file's header
#include <Entity/Enemy/X_Entity_Enemy.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture.h>

// X
#include <Core/State/X_State_Game.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const float fSPEED = 0.8f;
static const float fWIGGLE_SPEED = 0.5f;
static const float fWIGGLE_RANGE = 0.0025f;
static const float fSIZE = 0.1f;
static const GLToy_Hash xENEMY_SHIP_TEXTURE = GLToy_Hash_Constant( "Sprites/Enemy/Enemy1.png" );

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Array< X_Entity_Enemy* > X_Entity_Enemy::s_xList;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

X_Entity_Enemy::X_Entity_Enemy( const GLToy_Hash uHash, const u_int uType )
: GLToy_Parent( uHash, uType )
{
    m_xBoundingSphere.SetRadius( fSIZE );

	SetHealth( 40.0f );

    s_xList.Append( this );
}

X_Entity_Enemy::~X_Entity_Enemy()
{
    s_xList.RemoveByValue( this );
}

void X_Entity_Enemy::Update()
{
    GLToy_Vector_3 xPosition = GetPosition();
    xPosition -= GLToy_Vector_3( 0.0f, fSPEED * GLToy_Timer::GetFrameTime(), 0.0f );
	xPosition[0] += GLToy_Maths::Sin( GLToy_Timer::GetTime() * fWIGGLE_SPEED + static_cast<float>( GetHash() ) * 0.0001f ) * fWIGGLE_RANGE; // dont ask me about this mess

    SetPosition( xPosition );

    if( xPosition[ 1 ] < -1.5f )
    {
        Destroy();
    }

    if( IsDead() )
    {
        X_State_Game::AddScore( 10 );
        Destroy();
    }

    GLToy_Parent::Update();
}

void X_Entity_Enemy::Render() const
{
	const GLToy_Vector_3& xPosition = GetPosition();

    GLToy_Render::EnableBlending();
    GLToy_Render::SetBlendFunction( BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA );

    GLToy_Texture_System::BindTexture( xENEMY_SHIP_TEXTURE );

    GLToy_Render::StartSubmittingQuads();
		
    GLToy_Render::SubmitColour( GLToy_Vector_3( 1.0f, 1.0f, 1.0f ) );
	GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 0.0f ) );
	GLToy_Render::SubmitVertex( xPosition[ 0 ] - fSIZE, xPosition[ 1 ] + fSIZE, xPosition[ 2 ] ); 
    GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 0.0f ) );
	GLToy_Render::SubmitVertex( xPosition[ 0 ] + fSIZE, xPosition[ 1 ] + fSIZE, xPosition[ 2 ] ); 
	GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 1.0f ) );
    GLToy_Render::SubmitVertex( xPosition[ 0 ] + fSIZE, xPosition[ 1 ] - fSIZE, xPosition[ 2 ] );
	GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 1.0f ) );
    GLToy_Render::SubmitVertex( xPosition[ 0 ] - fSIZE, xPosition[ 1 ] - fSIZE, xPosition[ 2 ] );
	
    GLToy_Render::EndSubmit();

	GLToy_Render::DisableBlending();
}
