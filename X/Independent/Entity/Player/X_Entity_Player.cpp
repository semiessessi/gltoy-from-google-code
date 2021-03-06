/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010, 2011 Semi Essessi, Thomas Young
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

#include <Core/X.h>

// This file's header
#include <Entity/Player/X_Entity_Player.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Core/State/GLToy_State_System.h>
#include <Entity/GLToy_Entity_System.h>
#include <Material/GLToy_Material_System.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Render_Metrics.h>
#include <Render/GLToy_Texture_System.h>

// X
#include <Core/X_Cheats.h>
#include <Entity/X_EntityTypes.h>
#include <Entity/Collectible/X_Entity_Collectible.h>
#include <Entity/Enemy/X_Entity_Enemy.h>
#include <Entity/Projectile/X_Entity_Projectile.h>
#include "Equipment/X_Equipment_Weapon.h"
#include "Sound/X_Sound_System.h"

/////////////////////////////////////////////////////////////////////////////////////////////
// Static member vars
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Array< X_Entity_Player* > X_Entity_Player::s_xList;

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const float fX_PLAYER_ACC = 0.1f;
static const float fX_PLAYER_DECN = 0.2f;
static const float fX_PLAYER_SPEED = 2.5f;
static const float fX_PLAYER_SIZE = 0.08f;
static const GLToy_Hash xPLAYER_SHIP_TEXTURE = GLToy_Hash_Constant( "Sprites/Ship/Ship.png" );
static const GLToy_Hash xPLAYER_SHIP_MATERIAL = GLToy_Hash_Constant( "Ship/Ship" );

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

X_Entity_Player::X_Entity_Player( const GLToy_Hash uHash, const u_int uType )
: GLToy_Parent( uHash, uType )
, m_xMovement( GLToy_Maths::ZeroVector2 )
, m_xPreviousMovement( GLToy_Maths::ZeroVector2 )
, m_xSpeed( GLToy_Maths::ZeroVector2 )
, m_fAccelerationTimer( 0.0f )
, m_uLives( 3 )
, m_fShield( 1.0f )
, m_pxWeapon( 0 )
{
    BoundsFromMaterial( xPLAYER_SHIP_TEXTURE, fX_PLAYER_SIZE );

	s_xList.Append( this );
}

X_Entity_Player::~X_Entity_Player()
{
	s_xList.RemoveByValue( this );

	delete m_pxWeapon;
}

void X_Entity_Player::Update()
{
    GLToy_Vector_3 xPosition = GetPosition();

	// SE - TODO - parameterise recharge rate - maybe allow it to be upgradable?
	m_fShield += 0.1f * GLToy_Timer::GetFrameTime();
	m_fShield = GLToy_Maths::Min( m_fShield, 1.0f );
	m_fAccelerationTimer += GLToy_Timer::GetFrameTime();
	
	if( m_xMovement.x == 0.0f )
	{
		m_xSpeed.x = GLToy_Maths::ClampedLerp( m_xPreviousMovement.x, m_xMovement.x, fX_PLAYER_DECN );
	}
	else
	{
		m_xSpeed.x = GLToy_Maths::ClampedLerp( m_xPreviousMovement.x, m_xMovement.x, fX_PLAYER_ACC );
	}
	
	if( m_xMovement.y == 0.0f )
	{
		m_xSpeed.y = GLToy_Maths::ClampedLerp( m_xPreviousMovement.y, m_xMovement.y, fX_PLAYER_DECN );
	}
	else
	{
		m_xSpeed.y = GLToy_Maths::ClampedLerp( m_xPreviousMovement.y, m_xMovement.y, fX_PLAYER_ACC );
	}
	m_xPreviousMovement = m_xSpeed;

    xPosition += GLToy_Vector_3( m_xSpeed, 0.0f ) * fX_PLAYER_SPEED * GLToy_Timer::GetFrameTime();

    // TODO: think about aspect ratio...
    xPosition[ 0 ] = GLToy_Maths::Clamp( xPosition[ 0 ], -GLToy_Render::Get2DWidth() * 0.5f, GLToy_Render::Get2DWidth() * 0.5f );
    xPosition[ 1 ] = GLToy_Maths::Clamp( xPosition[ 1 ], -1.0f, 1.0f );

    SetPosition( xPosition );

	if( m_pxWeapon )
	{
		m_pxWeapon->Update();
	}

    static X_Entity_Player* ls_pxThis;
    ls_pxThis = this;
    // check for collisions:
    if( !X_Cheats::IsNoClip() )
    {
		// TY - These quick functors are fucking ugly, srsly :|
        GLToy_QuickFunctor( CollisionFunctor, X_Entity_Enemy*, ppxEnemy,

            if( ppxEnemy && !( *ppxEnemy )->IsDead() && ( *ppxEnemy )->GetBB().IntersectsWithAABB( ls_pxThis->GetBB() ) )
            {
                ls_pxThis->Hit();
                ( *ppxEnemy )->Kill();
            }
        );
        X_Entity_Enemy::GetList().Traverse( CollisionFunctor() );

		 GLToy_QuickFunctor( ProjectileFunctor, X_Entity_Projectile*, ppxProjectile,

            if( ppxProjectile && !( *ppxProjectile )->IsFromPlayer() && ( *ppxProjectile )->GetBB().IntersectsWithAABB( ls_pxThis->GetBB() ) )
            {
                ls_pxThis->Hit();
				( *ppxProjectile )->Destroy();
            }
        );
        X_Entity_Projectile::GetList().Traverse( ProjectileFunctor() );

        GLToy_QuickFunctor( CollectFunctor, X_Entity_Collectible*, ppxCollectible,

        if( ppxCollectible && ( *ppxCollectible )->GetBoundingSphere().IntersectsWithAABB( ls_pxThis->GetBB() ) )
        {
            ls_pxThis->Collect( *ppxCollectible );
            ( *ppxCollectible )->Destroy();
        }

        );

        X_Entity_Collectible::GetList().Traverse( CollectFunctor() );
    }

    if( !X_Cheats::IsGodMode() && ( m_uLives == 0xFFFFFFFF ) )
    {
        GLToy_State_System::ChangeState( GLToy_Hash_Constant( "GameOver" ) );
		X_Sound_System::PlayOneShotSound( GLToy_Hash_Constant( "GameOver" ) );
    }

    GLToy_Parent::Update();
}

void X_Entity_Player::Hit()
{
	if( !X_Cheats::IsGodMode() )
    {
		m_fShield -= 0.33f;
		if( m_fShield <= 0.0f )
		{
			m_fShield = 0.0f;
			// TODO: Remove lives, just kill the player when their shield is gone
			if( m_uLives != 0xFFFFFFFF )
            {
                --m_uLives;
            }
		}
		X_Sound_System::PlayOneShotSound( GLToy_Hash_Constant( "PlayerHit" ) );
    }
}

void X_Entity_Player::Render() const
{
    GLToy_Render_Metrics::IncrementTriangleCount( 2 );

    if( GLToy_Render::HasDeferredBuffer() )
    {
        GLToy_Render::RegisterDeferred( this );
        return;
    }

    const GLToy_Vector_3& xPosition = GetPosition();

    GLToy_Render::EnableBlending();
    GLToy_Render::SetBlendFunction( BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA );

    GLToy_Texture_System::BindTexture( xPLAYER_SHIP_TEXTURE );

    GLToy_Render::StartSubmittingQuads();
		
    GLToy_Render::SubmitColour( GLToy_Vector_3( 1.0f, 1.0f, 1.0f ) );
	GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 0.0f ) );
	GLToy_Render::SubmitVertex( xPosition[ 0 ] - fX_PLAYER_SIZE, xPosition[ 1 ] + fX_PLAYER_SIZE, xPosition[ 2 ] ); 
    GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 0.0f ) );
	GLToy_Render::SubmitVertex( xPosition[ 0 ] + fX_PLAYER_SIZE, xPosition[ 1 ] + fX_PLAYER_SIZE, xPosition[ 2 ] ); 
	GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 1.0f ) );
    GLToy_Render::SubmitVertex( xPosition[ 0 ] + fX_PLAYER_SIZE, xPosition[ 1 ] - fX_PLAYER_SIZE, xPosition[ 2 ] );
	GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 1.0f ) );
    GLToy_Render::SubmitVertex( xPosition[ 0 ] - fX_PLAYER_SIZE, xPosition[ 1 ] - fX_PLAYER_SIZE, xPosition[ 2 ] );
	
    GLToy_Render::EndSubmit();

	GLToy_Render::DisableBlending();
}

void X_Entity_Player::RenderDeferred() const
{
    const GLToy_Vector_3& xPosition = GetPosition();

    GLToy_Material* const pxMaterial = GLToy_Material_System::FindMaterial( xPLAYER_SHIP_MATERIAL );
    if( pxMaterial )
    {
        pxMaterial->Bind();
    }

    GLToy_Render::StartSubmittingQuads();
		
	GLToy_Render::SubmitDeferredVertex( xPosition[ 0 ] - fX_PLAYER_SIZE, xPosition[ 1 ] + fX_PLAYER_SIZE, xPosition[ 2 ],
        GLToy_Vector_2( 0.0f, 0.0f ), xSPRITE_NORMAL, xSPRITE_TANGENT ); 
	GLToy_Render::SubmitDeferredVertex( xPosition[ 0 ] + fX_PLAYER_SIZE, xPosition[ 1 ] + fX_PLAYER_SIZE, xPosition[ 2 ],
        GLToy_Vector_2( 1.0f, 0.0f ), xSPRITE_NORMAL, xSPRITE_TANGENT ); 
	GLToy_Render::SubmitDeferredVertex( xPosition[ 0 ] + fX_PLAYER_SIZE, xPosition[ 1 ] - fX_PLAYER_SIZE, xPosition[ 2 ],
        GLToy_Vector_2( 1.0f, 1.0f ), xSPRITE_NORMAL, xSPRITE_TANGENT ); 
	GLToy_Render::SubmitDeferredVertex( xPosition[ 0 ] - fX_PLAYER_SIZE, xPosition[ 1 ] - fX_PLAYER_SIZE, xPosition[ 2 ],
        GLToy_Vector_2( 0.0f, 1.0f ), xSPRITE_NORMAL, xSPRITE_TANGENT ); 
	
    GLToy_Render::EndSubmit();
}

void X_Entity_Player::Collect( const X_Entity_Collectible* pxCollectible )
{
	switch( pxCollectible->GetCollectType() )
	{
		case X_COLLECTIBLE_TYPE_LIFE:
		{
			++m_uLives;
		}
		break;

		case X_COLLECTIBLE_TYPE_BOOST:
		{
			if( m_pxWeapon )
			{
				m_pxWeapon->Boost();
			}
		}
        break;

		case X_COLLECTIBLE_TYPE_WEAPON:
		{
			// No :|
		}
        break;
	}
}

void X_Entity_Player::StartShooting()
{
	if( m_pxWeapon )
	{
		m_pxWeapon->StartShooting();
	}
}

void X_Entity_Player::StopShooting()
{
	if( m_pxWeapon )
	{
		m_pxWeapon->StopShooting();
	}
}

void X_Entity_Player::CreateWeapon( u_int uWeaponType )
{
	delete m_pxWeapon;
	m_pxWeapon = X_Weapon_Factory::CreatePlayerWeapon( uWeaponType );
}

//eof
