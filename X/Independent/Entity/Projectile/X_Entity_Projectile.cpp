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

#include <Core/GLToy.h>

// This file's header
#include <Entity/Projectile/X_Entity_Projectile.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Particle/GLToy_PFX_System.h>
#include <Render/GLToy_Light_System.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Render_Metrics.h>
#include <Render/GLToy_Texture_System.h>
#include <Sound/GLToy_Sound_System.h>

// X
#include <Entity/Enemy/X_Entity_Enemy.h>
#include <Entity/Player/X_Entity_Player.h>
#include "Core/X_Score.h"

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const GLToy_Hash xPROJECTILE_TEXTURE[] = { GLToy_Hash_Constant( "Sprites/Projectile/Projectile1.png" ),
												  GLToy_Hash_Constant( "Sprites/Projectile/Projectile2.png" ),
												  GLToy_Hash_Constant( "Sprites/Projectile/Projectile3.png" ),
												  GLToy_Hash_Constant( "Sprites/Projectile/Projectile4.png" ),
												  GLToy_Hash_Constant( "Sprites/Projectile/Projectile5.png" ) };

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Array< X_Entity_Projectile* > X_Entity_Projectile::s_xList;

X_Entity_Projectile::X_Entity_Projectile( const GLToy_Hash uHash, const u_int uType )
: GLToy_Parent( uHash, uType )
, m_uTexture( 0 )
, m_uLight( GLToy_Random_Hash() )
, m_bFromPlayer( false )
, m_fSpeed( 0.0f )
{
    //m_xBoundingSphere.SetRadius( 0.0f );
	m_xDirection[1] = 1.0f;
    GLToy_Light_PointProperties xProperties;
    xProperties.m_fSphereRadius = 0.3f;
    xProperties.m_xColour = GLToy_Vector_3( 1.5f, 1.5f, 1.5f );
    GLToy_Light_System::AddPointLight( m_uLight, xProperties );

	s_xList.Append( this );
}

X_Entity_Projectile::~X_Entity_Projectile()
{
    GLToy_Light_System::DestroyLight( m_uLight );

	s_xList.RemoveByValue( this );
}

void X_Entity_Projectile::SetTexture( const u_int uIndex )
{
    m_uTexture = uIndex;
    GLToy_Texture_System::CreateTexture( xPROJECTILE_TEXTURE[ uIndex ] );
    BoundsFromMaterial( xPROJECTILE_TEXTURE[ uIndex ], GetRadius() );
}

void X_Entity_Projectile::Update()
{
    GLToy_Vector_3 xPosition = GetPosition();
    xPosition += m_xDirection * ( m_fSpeed * GLToy_Timer::GetFrameTime() );

    SetPosition( xPosition );

    if( xPosition[ 1 ] > 1.5f )
    {
        Destroy();
    }

    GLToy_Light* const pxLight = GLToy_Light_System::FindLight( m_uLight );
    if( pxLight )
    {
        pxLight->SetPosition( xPosition + GLToy_Vector_3( 0.0f, 0.0f, -0.1f ) );
    }

    static X_Entity_Projectile* ls_pxThis;
    ls_pxThis = this;

    // check for collisions:
	// TODO: This seems like a crap place to do this
	if( IsFromPlayer() )
	{
		GLToy_QuickFunctor( CollisionFunctor, X_Entity_Enemy*, ppxEnemy,

			if( ppxEnemy && !( *ppxEnemy )->IsDead() && ( *ppxEnemy )->GetBB().IntersectsWithAABB( ls_pxThis->GetBB() ) )
			{
				GLToy_PFX_System::CreatePFX( GLToy_GetHash("Shot_Hit1" ), ls_pxThis->GetBB().GetPosition(), ( *ppxEnemy )->GetVelocity() );
				( *ppxEnemy )->Hurt( 0, 10.0f );
				( *ppxEnemy )->AddImpulse( ls_pxThis->m_xDirection );
				X_Score::Add( 0.1f, ls_pxThis->GetBB().GetPosition() );
				GLToy_Handle xVoice = GLToy_Sound_System::CreateVoice( GLToy_Hash_Constant( "Hit" ) );
				GLToy_Sound_Voice* pxVoice = GLToy_Sound_System::GetVoice( xVoice );
				if( pxVoice )
				{
					pxVoice->SetSpeakerMapping( GLToy_Sound_Voice::SM_DIRECT );
					pxVoice->SetPosition(  ls_pxThis->GetPosition() );
					pxVoice->SetRadius( 600.0f );
					pxVoice->SetAmplitude( 0.5f );
					pxVoice->Play();
					pxVoice->Release();
				}
				ls_pxThis->Destroy();
			}
		);

		X_Entity_Enemy::GetList().Traverse( CollisionFunctor() );
	}
	
    GLToy_Parent::Update();
}

void X_Entity_Projectile::Render() const
{
    GLToy_Render::RegisterTransparent( this, 0.0f );
}

void X_Entity_Projectile::RenderTransparent() const
{
    GLToy_Render_Metrics::IncrementTriangleCount( 2 );

    const GLToy_Vector_3& xPosition = GetPosition();

	const float fSize = GetRadius();

    GLToy_Render::SetBlendFunction( BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA );

	GLToy_Texture_System::BindTexture( xPROJECTILE_TEXTURE[ m_uTexture ] );

	GLToy_Render::StartSubmittingQuads();
		
    GLToy_Render::SubmitColour( GLToy_Vector_3( 1.0f, 1.0f, 1.0f ) );
	GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 0.0f ) );
	GLToy_Render::SubmitVertex( xPosition[ 0 ] - fSize, xPosition[ 1 ] + fSize, xPosition[ 2 ] ); 
    GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 0.0f ) );
	GLToy_Render::SubmitVertex( xPosition[ 0 ] + fSize, xPosition[ 1 ] + fSize, xPosition[ 2 ] ); 
	GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 1.0f ) );
    GLToy_Render::SubmitVertex( xPosition[ 0 ] + fSize, xPosition[ 1 ] - fSize, xPosition[ 2 ] );
	GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 1.0f ) );
    GLToy_Render::SubmitVertex( xPosition[ 0 ] - fSize, xPosition[ 1 ] - fSize, xPosition[ 2 ] );
	
    GLToy_Render::EndSubmit();
}
