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
#include <Render/GLToy_Render_Metrics.h>
#include <Render/GLToy_Texture_System.h>

// X
#include <Core/X_Cheats.h>
#include <Entity/X_EntityTypes.h>
#include <Entity/Collectible/X_Entity_Collectible.h>
#include <Entity/Enemy/X_Entity_Enemy.h>
#include "Equipment/X_Equipment_Weapon.h"

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
, m_pxWeapon( 0 )
{
    m_xBoundingSphere.SetRadius( fSIZE );

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

	if( m_pxWeapon )
	{
		m_pxWeapon->Update();
	}

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

    GLToy_Parent::Update();

	m_xPreviousMovement = m_xMovement;
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
