/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010, 2011 Semi Essessi, Thomas Young
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
#include <Render/GLToy_Texture_System.h>

// X
#include <Core/X_Cheats.h>
#include <Entity/X_EntityTypes.h>
#include <Entity/Projectile/X_Entity_Projectile.h>
#include <Entity/Collectible/X_Entity_Collectible.h>
#include <Entity/Enemy/X_Entity_Enemy.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// Static member vars
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Array< X_Entity_Player* > X_Entity_Player::s_xList;

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const float fACCELERATION = 3.0f;
static const float fSPEED = 2.5f;
static const float fSIZE = 0.08f;
static const GLToy_Hash xPLAYER_SHIP_TEXTURE = GLToy_Hash_Constant( "Sprites/Ship/Ship.png" );

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

X_Entity_Player::X_Entity_Player( const GLToy_Hash uHash, const u_int uType )
: GLToy_Parent( uHash, uType )
, m_xMovement( GLToy_Maths::ZeroVector2 )
, m_xPreviousMovement( GLToy_Maths::ZeroVector2 )
, m_xSpeed( GLToy_Maths::ZeroVector2 )
, m_xLerpStart( GLToy_Maths::ZeroVector2 )
, m_fAccelerationTimer( 0.0f )
, m_uLives( 3 )
, m_xWeapon()
, m_fShootTimer( 0.0f )
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
	if( m_xMovement[ 0 ] != m_xPreviousMovement[ 0 ] )
	{
		m_xLerpStart[ 0 ] = 0.0f;
		m_xLerpStart[ 1 ] = m_xSpeed[ 1 ];
		m_fAccelerationTimer = 0.0f;
	}

	if( m_xMovement[ 1 ] != m_xPreviousMovement[ 1 ] )
	{
		m_xLerpStart[ 1 ] = 0.0f;
		m_xLerpStart[ 0 ] = m_xSpeed[ 0 ];
		m_fAccelerationTimer = 0.0f;
	}

	m_fAccelerationTimer += GLToy_Timer::GetFrameTime();
	m_xSpeed = GLToy_Maths::ClampedLerp( m_xLerpStart, m_xMovement, fACCELERATION * m_fAccelerationTimer );

    xPosition += GLToy_Vector_3( m_xSpeed, 0.0f ) * fSPEED * GLToy_Timer::GetFrameTime();

    // TODO: think about aspect ratio...
    xPosition[ 0 ] = GLToy_Maths::Clamp( xPosition[ 0 ], -GLToy_Render::Get2DWidth() * 0.5f, GLToy_Render::Get2DWidth() * 0.5f );
    xPosition[ 1 ] = GLToy_Maths::Clamp( xPosition[ 1 ], -1.0f, 1.0f );

    SetPosition( xPosition );

    static X_Entity_Player* ls_pxThis;
    ls_pxThis = this;
    // check for collisions:
    if( !X_Cheats::IsNoClip() )
    {
        GLToy_QuickFunctor( CollisionFunctor, X_Entity_Enemy*, ppxEnemy,

            if( ppxEnemy && !( *ppxEnemy )->IsDead() && ( *ppxEnemy )->GetBoundingSphere().IntersectsWithSphere( ls_pxThis->m_xBoundingSphere ) )
            {
                if( !X_Cheats::IsGodMode() )
                {
                    --( ls_pxThis->m_uLives );
                }

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
    }

    if( !X_Cheats::IsGodMode() && ( m_uLives == 0xFFFFFFFF ) )
    {
        GLToy_State_System::ChangeState( GLToy_Hash_Constant( "GameOver" ) );
    }

	m_fShootTimer = GLToy_Maths::Max( m_fShootTimer - GLToy_Timer::GetFrameTime(), 0.0f );

    GLToy_Parent::Update();

	m_xPreviousMovement = m_xMovement;
}

void X_Entity_Player::Render() const
{
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

void X_Entity_Player::RenderDeferred() const
{
    const GLToy_Vector_3& xPosition = GetPosition();

    GLToy_Material* const pxMaterial = GLToy_Material_System::FindMaterial( xPLAYER_SHIP_TEXTURE );
    if( pxMaterial )
    {
        pxMaterial->Bind();
    }

    GLToy_Render::StartSubmittingQuads();
		
	GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 0.0f ) );
	GLToy_Render::SubmitUV( xCOMPRESSED_NORMAL_BASIS, 1 );
	GLToy_Render::SubmitVertex( xPosition[ 0 ] - fSIZE, xPosition[ 1 ] + fSIZE, xPosition[ 2 ] ); 
    GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 0.0f ) );
	GLToy_Render::SubmitUV( xCOMPRESSED_NORMAL_BASIS, 1 );
	GLToy_Render::SubmitVertex( xPosition[ 0 ] + fSIZE, xPosition[ 1 ] + fSIZE, xPosition[ 2 ] ); 
	GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 1.0f ) );
	GLToy_Render::SubmitUV( xCOMPRESSED_NORMAL_BASIS, 1 );
	GLToy_Render::SubmitVertex( xPosition[ 0 ] + fSIZE, xPosition[ 1 ] - fSIZE, xPosition[ 2 ] );
	GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 1.0f ) );
	GLToy_Render::SubmitUV( xCOMPRESSED_NORMAL_BASIS, 1 );
	GLToy_Render::SubmitVertex( xPosition[ 0 ] - fSIZE, xPosition[ 1 ] - fSIZE, xPosition[ 2 ] );
	
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
			m_xWeapon.Boost( 0.5f );
		}
        break;

		case X_COLLECTIBLE_TYPE_WEAPON:
		{
			m_xWeapon.RandomGenerate();
		}
        break;
	}
}

void X_Entity_Player::Shoot()
{
	if( m_fShootTimer == 0.0f )
	{
		const u_int uWeaponTexture = static_cast< u_int >( m_xWeapon.GetSpread() * 3.0f );

		if( m_xWeapon.GetNumProjectiles() == 1 )
		{
			X_Entity_Projectile* pxProjectile = static_cast< X_Entity_Projectile* >( GLToy_Entity_System::CreateEntity( GLToy_Random_Hash(), X_ENTITY_PROJECTILE ) );
            pxProjectile->SetPosition( GetPosition() );
			
			GLToy_Vector_2 xDirection( 0.0f, 1.0f );
			xDirection = GLToy_Maths::Rotate_2D( xDirection, GLToy_Maths::Random( -m_xWeapon.GetSpread(), m_xWeapon.GetSpread() ) );
			pxProjectile->SetDirection( GLToy_Vector_3( xDirection.x, xDirection.y, 0.0f ) );

			pxProjectile->SetRadius( m_xWeapon.GetSize() );
			pxProjectile->SetTexture( uWeaponTexture );
		}
		else
		{
			GLToy_Vector_3 xDirection = GLToy_Vector_3( -m_xWeapon.GetSpread(), 1.0f, 0.0f );
			const GLToy_Vector_3 xIncrement = GLToy_Vector_3( 2.0f * m_xWeapon.GetSpread() / static_cast< float >( m_xWeapon.GetNumProjectiles() - 1 ), 0.0f, 0.0f );
			for( int i = 0 ; i < m_xWeapon.GetNumProjectiles(); ++i )
			{
				X_Entity_Projectile* pxProjectile = static_cast< X_Entity_Projectile* >( GLToy_Entity_System::CreateEntity( GLToy_Random_Hash(), X_ENTITY_PROJECTILE ) );
				pxProjectile->SetPosition( GetPosition() );
				GLToy_Vector_3 xNormalisedDirection = xDirection;
				xNormalisedDirection.Normalise();
				pxProjectile->SetDirection( xNormalisedDirection );
				xDirection += xIncrement;

				pxProjectile->SetRadius( m_xWeapon.GetSize() );
				pxProjectile->SetTexture( uWeaponTexture );
			}
		}

		m_fShootTimer = m_xWeapon.GetRate();
	}

}

void X_Entity_Player::SetWeapon( const X_Equipment_Weapon& xWeapon )
{
	m_xWeapon = xWeapon;
}

//eof
