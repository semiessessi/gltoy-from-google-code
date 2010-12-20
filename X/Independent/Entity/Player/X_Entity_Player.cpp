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
#include <Entity/Player/X_Entity_Player.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Core/State/GLToy_State_System.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture.h>

// X
#include <Entity/Collectible/X_Entity_Collectible.h>
#include <Entity/Enemy/X_Entity_Enemy.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// Static member vars
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Array< X_Entity_Player* > X_Entity_Player::s_xList;

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const float fSPEED = 2.5f;
static const float fSIZE = 0.05f;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

X_Entity_Player::X_Entity_Player( const GLToy_Hash uHash, const u_int uType )
: GLToy_Parent( uHash, uType )
, m_xMovement( GLToy_Maths::ZeroVector2 )
, m_uLives( 3 )
, m_uGunType( X_PLAYER_GUN_TYPE_SINGLE )
{
    m_xBoundingSphere.SetRadius( fSIZE );

	s_xList.Append( this );
}

X_Entity_Player::~X_Entity_Player()
{
	s_xList.RemoveByValue( this );
}

void X_Entity_Player::Update()
{
    GLToy_Vector_3 xPosition = GetPosition();
    xPosition += GLToy_Vector_3( m_xMovement, 0.0f ) * fSPEED * GLToy_Timer::GetFrameTime();

    // TODO: think about aspect ratio...
    xPosition[ 0 ] = GLToy_Maths::Clamp( xPosition[ 0 ], -1.0f, 1.0f );
    xPosition[ 1 ] = GLToy_Maths::Clamp( xPosition[ 1 ], -1.0f, 1.0f );

    SetPosition( xPosition );

    static X_Entity_Player* ls_pxThis;
    ls_pxThis = this;
    // check for collisions:
    GLToy_QuickFunctor( CollisionFunctor, X_Entity_Enemy*, ppxEnemy,

        if( ppxEnemy && !( *ppxEnemy )->IsDead() && ( *ppxEnemy )->GetBoundingSphere().IntersectsWithSphere( ls_pxThis->m_xBoundingSphere ) )
        {
            --( ls_pxThis->m_uLives );
            ( *ppxEnemy )->Kill();
        }

    );

    X_Entity_Enemy::GetList().Traverse( CollisionFunctor() );

	GLToy_QuickFunctor( CollectFunctor, X_Entity_Collectible*, ppxCollectible,

        if( ppxCollectible && ( *ppxCollectible )->GetBoundingSphere().IntersectsWithSphere( ls_pxThis->m_xBoundingSphere ) )
        {
            ls_pxThis->Collect( *ppxCollectible );
            ( *ppxCollectible )->Destroy();
        }

    );

    X_Entity_Collectible::GetList().Traverse( CollectFunctor() );

    if( m_uLives == 0xFFFFFFFF )
    {
        GLToy_State_System::ChangeState( GLToy_Hash_Constant( "GameOver" ) );
    }

    GLToy_Parent::Update();
}

void X_Entity_Player::Render() const
{
    const GLToy_Vector_3& xPosition = GetPosition();

    GLToy_Texture_System::BindWhite();

    GLToy_Render::StartSubmittingTriangles();

    GLToy_Render::SubmitColour( GLToy_Vector_3( 1.0f, 1.0f, 1.0f ) );
    GLToy_Render::SubmitVertex( xPosition[ 0 ], xPosition[ 1 ] + fSIZE, xPosition[ 2 ] );  
    GLToy_Render::SubmitVertex( xPosition[ 0 ] + fSIZE, xPosition[ 1 ] - fSIZE, xPosition[ 2 ] ); 
    GLToy_Render::SubmitVertex( xPosition[ 0 ] - fSIZE, xPosition[ 1 ] - fSIZE, xPosition[ 2 ] );

    GLToy_Render::EndSubmit();
}

void X_Entity_Player::Collect( const X_Entity_Collectible* pxCollectible )
{
	switch( pxCollectible->GetCollectType() )
	{
		case X_COLLECTIBLE_TYPE_LIFE:
		{
			m_uLives++;
		}
		break;

		case X_COLLECTIBLE_TYPE_WEAPON:
		{
			switch( m_uGunType )
			{
				case X_PLAYER_GUN_TYPE_SINGLE:	m_uGunType = X_PLAYER_GUN_TYPE_TRIPLE; break;
				case X_PLAYER_GUN_TYPE_TRIPLE:	m_uGunType = X_PLAYER_GUN_TYPE_FIVER;  break;
			}
		}
	}
}

//eof
