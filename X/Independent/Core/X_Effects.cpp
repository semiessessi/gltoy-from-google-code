
// TY 26.03.11

#include "Core/GLToy.h"

#include "X_Effects.h"

#include "Core/GLToy_Timer.h"
#include "Maths/GLToy_Maths.h"
#include "Material/GLToy_Material_System.h"
#include "Particle/GLToy_PFX_System.h"
#include "Render/GLToy_Render.h"
#include "Render/GLToy_Renderable.h"

#include "Core/X.h"
#include "Entity/Enemy/X_Entity_Enemy.h"
#include "Sound/X_Sound_System.h"

static const GLToy_Hash xTEXTURE_FX_SHOCKWAVE = GLToy_Hash_Constant( "Sprites/FX/Shockwave1.png" );

// _______________________________________________________________________

class X_Effect : public GLToy_Renderable
{
	public:

		X_Effect()
		: m_fTimer( 0.0f )
		{}

		virtual bool IsDone() = 0;

		virtual void Update()
		{
			m_fTimer += GLToy_Timer::GetFrameTime();
		}

		virtual void Render() const {};
		virtual void RenderDeferred() const {};

	protected:

		float GetTimeSinceStarting() const
		{
			return m_fTimer;
		}

	private:

		float m_fTimer;
};

// _______________________________________________________________________

class X_Effect_Positional : public X_Effect
{
	public:

		X_Effect_Positional( const GLToy_Vector_3& xPosition )
		: X_Effect()
		, m_xPosition( xPosition )
		{}
		//virtual ~X_Effect_Positional() {}

		const GLToy_Vector_3& GetPosition() const { return m_xPosition; }

	protected:

		GLToy_Vector_3 m_xPosition;
};

// _______________________________________________________________________

class X_Effect_Shockwave : public X_Effect_Positional
{
	typedef X_Effect_Positional PARENT;

	public:

		X_Effect_Shockwave( const GLToy_Vector_3& xPosition, float fSize )
		: PARENT( xPosition )
		, m_fSize( fSize )
		{}

		inline const float fEFFECT_DURATION() const { return 1.0f; }
		inline const float fPUSH() const { return 1.0f; }
		inline const float fHURT() const { return 2.0f; }

		virtual bool IsDone()
		{
			return ( GetTimeSinceStarting() > fEFFECT_DURATION() );
		}

		virtual void Update()
		{
			PARENT::Update();

			const float fSize = GetNormalisedSize() * m_fSize;

			GLToy_Array<X_Entity_Enemy*> xList = X_Entity_Enemy::GetList();
			for( u_int uEnemy = 0; uEnemy < xList.GetCount(); ++uEnemy )
			{
				if( IsAlreadyHit( xList[uEnemy]->GetHash() ) )
				{
					continue;
				}
				GLToy_Vector_3 xFront = xList[uEnemy]->GetPosition() - m_xPosition;
				if( xFront == GLToy_Maths::ZeroVector3 )
				{
					xFront.y += 0.01f;
				}
				xFront.Normalise();
				const GLToy_Vector_3 xHitPos = m_xPosition + xFront * fSize;
				if( xList[uEnemy]->GetBB().IsInside( xHitPos ) )
				{
					// Push
					GLToy_Vector_2 xPush( xFront.x, xFront.y );
					xPush = xPush * fPUSH() * ( 1.0f - fSize );
					xList[uEnemy]->AddImpulse( xPush );
					
					// Hurt
					const float fHurtVal = ( 1.0f - fSize ) * fHURT();
 					xList[uEnemy]->Hurt( 0, fHurtVal );

					m_xAlreadyHit.Append( xList[uEnemy]->GetHash() );
				}
			}
		}
		
		virtual void Render() const
		{
			GLToy_Render_Metrics::IncrementTriangleCount( 2 );

			/* if( GLToy_Render::HasDeferredBuffer() )
			{
				GLToy_Render::RegisterDeferred( this );
				return;
			} */

			GLToy_Render::EnableBlending();
			GLToy_Render::SetBlendFunction( BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA );

			GLToy_Texture_System::BindTexture( xTEXTURE_FX_SHOCKWAVE );

			const float fSize = GetNormalisedSize() * m_fSize;

			GLToy_Render::StartSubmittingQuads();
		
			GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, GetOpacity() ) );
			GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 0.0f ) );
			GLToy_Render::SubmitVertex( m_xPosition[ 0 ] - fSize, m_xPosition[ 1 ] + fSize, m_xPosition[ 2 ] ); 
			GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 0.0f ) );
			GLToy_Render::SubmitVertex( m_xPosition[ 0 ] + fSize, m_xPosition[ 1 ] + fSize, m_xPosition[ 2 ] ); 
			GLToy_Render::SubmitUV( GLToy_Vector_2( 1.0f, 1.0f ) );
			GLToy_Render::SubmitVertex( m_xPosition[ 0 ] + fSize, m_xPosition[ 1 ] - fSize, m_xPosition[ 2 ] );
			GLToy_Render::SubmitUV( GLToy_Vector_2( 0.0f, 1.0f ) );
			GLToy_Render::SubmitVertex( m_xPosition[ 0 ] - fSize, m_xPosition[ 1 ] - fSize, m_xPosition[ 2 ] );
	
			GLToy_Render::EndSubmit();

			GLToy_Render::DisableBlending();
		}

		virtual void RenderDeferred() const
		{
			// Tried this but transparency is not supported
		}

	protected:

		float m_fSize;
		GLToy_Array<GLToy_Hash> m_xAlreadyHit; // TODO: Use a hash tree

		bool IsAlreadyHit( GLToy_Hash uHash ) const
		{
			for( u_int uEnemy = 0; uEnemy < m_xAlreadyHit.GetCount(); ++uEnemy )
			{
				if( m_xAlreadyHit[uEnemy] == uHash )
				{
					return true;
				}
			}
			return false;
		}

		float GetNormalisedSize() const
		{
			const float fNorm = GetTimeSinceStarting() / fEFFECT_DURATION();
			return GLToy_Maths::Pow( fNorm, 0.5f );
		}

		float GetOpacity() const
		{
			return ( 1.0f - GetNormalisedSize() ) * 0.7f;
		}
};

// _______________________________________________________________________

class X_Effect_Explosion : public X_Effect_Positional
{
	typedef X_Effect_Positional PARENT;

	public:

		X_Effect_Explosion( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xVelocity )
		: PARENT( xPosition )
		{
			GLToy_PFX_System::CreatePFX( GLToy_Hash_Constant( "Explosion1" ), m_xPosition, xVelocity );
			X_Sound_System::PlayOneShotSound( GLToy_Hash_Constant( "Explode" ), GetPosition() );
		}

		virtual bool IsDone()
		{
			return true;
		}

	protected:

		GLToy_Vector_3 m_xVelocity;
};

// _______________________________________________________________________


GLToy_Array< X_Effect* > X_Effect_System::s_xEffects;


void X_Effect_System::CreateShockwave( const GLToy_Vector_3& xPosition, const float fSize )
{
	s_xEffects.Append( new X_Effect_Shockwave( xPosition, fSize ) );
}

void X_Effect_System::CreateExplosion( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xVelocity )
{
 	s_xEffects.Append( new X_Effect_Explosion( xPosition, xVelocity ) );
}

void X_Effect_System::Initialise()
{

}

void X_Effect_System::Update()
{
	if( s_xEffects.GetCount() == 0 )
	{
		return;
	}

	for( u_int uEffect = s_xEffects.GetCount() - 1; uEffect != 0; --uEffect )
	{
		if( s_xEffects[uEffect] )
		{
			s_xEffects[uEffect]->Update();
			if( s_xEffects[uEffect]->IsDone() )
			{
				delete s_xEffects[uEffect];
				s_xEffects.RemoveAt( static_cast<int>( uEffect ) );
			}
		}
	}
}

void X_Effect_System::Render()
{
	for( u_int uEffect = 0; uEffect < s_xEffects.GetCount(); ++uEffect )
	{
		if( s_xEffects[uEffect] )
		{
			s_xEffects[uEffect]->Render();
		}
	}
}

void X_Effect_System::Shutdown()
{
	for( u_int uEffect = s_xEffects.GetCount() - 1; uEffect != 0; --uEffect )
	{
		if( s_xEffects[uEffect] )
		{
			delete s_xEffects[uEffect];
			s_xEffects.RemoveAt( static_cast<int>( uEffect ) );
		}
	}
}

// eof
