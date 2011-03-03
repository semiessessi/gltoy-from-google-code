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
#include <Entity/Enemy/X_Entity_Enemy.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Material/GLToy_Material_System.h>
#include <Particle/GLToy_PFX_System.h>
#include <Render/GLToy_Light_System.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Render_Metrics.h>
#include <Render/GLToy_Texture_System.h>

// X
#include "AI/X_Enemy_Brain.h"
#include <Core/State/X_State_Game.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const float fSPEED = 0.8f;
static const float fWIGGLE_SPEED = 0.5f;
static const float fWIGGLE_RANGE = 0.0025f;
static const float fSIZE = 0.1f;
static const GLToy_Hash xENEMY_SHIP_MATERIAL = GLToy_Hash_Constant( "Enemy/Enemy1" );

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Array< X_Entity_Enemy* > X_Entity_Enemy::s_xList;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

X_Entity_Enemy::X_Entity_Enemy( const GLToy_Hash uHash, const u_int uType )
: GLToy_Parent( uHash, uType )
, m_fSpeed( 0.0f )
, m_uLight( GLToy_Random_Hash() )
, m_pxBrain( 0 )
{
    //m_xBoundingSphere.SetRadius( fSIZE );
	TightenCollision( xENEMY_SHIP_MATERIAL );

	SetHealth( 40.0f );

	m_xDirection = GLToy_Maths::ZeroVector2;
	m_xDirection[1] = 1.0f;

    s_xList.Append( this );

    GLToy_Light_PointProperties xProperties;
    xProperties.m_fSphereRadius = 0.35f;
    xProperties.m_xColour = GLToy_Vector_3( 1.5f, 0.5f, 0.3f );
    GLToy_Light_System::AddPointLight( m_uLight, xProperties );
}

X_Entity_Enemy::~X_Entity_Enemy()
{
    GLToy_Light_System::DestroyLight( m_uLight );

    s_xList.RemoveByValue( this );
}

void X_Entity_Enemy::SetDefinition( const X_Enemy_Definition& xDefinition )
{
	m_xDefinition = m_xDefinition;

	delete m_pxBrain;
	m_pxBrain = X_Enemy_Brain_Factory::CreateBrain( xDefinition.m_uBrain, GetHash() );
}


void X_Entity_Enemy::Update()
{
	if( m_pxBrain )
	{
		m_pxBrain->Update();
	}

	GLToy_Vector_3 xPosition = GetPosition();
	xPosition.x += m_xDirection.x * m_fSpeed * GLToy_Timer::GetFrameTime();
	xPosition.y += m_xDirection.y * m_fSpeed * GLToy_Timer::GetFrameTime();
	SetPosition( xPosition );

    if( xPosition[ 1 ] < -1.5f )
    {
        Destroy();
    }

    GLToy_Light* const pxLight = GLToy_Light_System::FindLight( m_uLight );
    if( pxLight )
    {
        pxLight->SetPosition( xPosition + GLToy_Vector_3( 0.0f, 0.5f * fSIZE, -0.1f ) );
    }

    if( IsDead() )
    {
        GLToy_PFX_System::CreatePFX( GLToy_Hash_Constant( "Explosion1" ), xPosition, GetVelocity() );
        X_State_Game::AddScore( 10 );
        Destroy();
    }

    GLToy_Parent::Update();
}

void X_Entity_Enemy::Render() const
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

    GLToy_Material_System::BindMaterial( xENEMY_SHIP_MATERIAL );

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

void X_Entity_Enemy::RenderDeferred() const
{
    GLToy_Vector_3 xPosition = m_xBoundingSphere.GetPosition();

    GLToy_Material* const pxMaterial = GLToy_Material_System::FindMaterial( xENEMY_SHIP_MATERIAL );
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

void X_Entity_Enemy::TightenCollision( const GLToy_Hash uMaterialHash )
{
	const GLToy_Material* const pxMaterial = GLToy_Material_System::FindMaterial( uMaterialHash );
	if( !pxMaterial )
	{
		m_xBoundingSphere.SetRadius( fSIZE );
		return;
	}
	
	const GLToy_Texture* const pxTexture = GLToy_Texture_System::FindTexture( pxMaterial->GetTextureHash( 0 ) );
	if( !pxTexture )
	{
		m_xBoundingSphere.SetRadius( fSIZE );
		return;
	}
	
	u_int uMinX = pxTexture->GetWidth() - 1, uMaxX = 0, uMinY = pxTexture->GetHeight() - 1, uMaxY = 0;
	for( u_int u = 0; u < pxTexture->GetWidth(); ++u )
	for( u_int v = 0; v < pxTexture->GetHeight(); ++v )
	{
		// does it have non-zero alpha?
		if( pxTexture->GetPixel( u, v ) & 0xFF000000 )
		{
			uMinX = ( u < uMinX ) ? u : uMinX;
			uMaxX = ( u > uMinX ) ? u : uMaxX;
			uMinY = ( v < uMinY ) ? v : uMinY;
			uMaxY = ( v > uMinY ) ? v : uMaxY;
		}
	}
	
	const float fCentreX = static_cast< float >( pxTexture->GetWidth() ) * 0.5f;
	const float fCentreY = static_cast< float >( pxTexture->GetHeight() ) * 0.5f;
	
	const float fDistanceX = GLToy_Maths::Max( fCentreX - static_cast< float >( uMinX ), static_cast< float >( uMaxX ) - fCentreX ) / fCentreX;
	const float fDistanceY = GLToy_Maths::Max( fCentreY - static_cast< float >( uMinY ), static_cast< float >( uMaxY ) - fCentreY ) / fCentreY;
	
	m_xBoundingSphere.SetRadius( fSIZE * GLToy_Maths::Max( fDistanceX, fDistanceY ) );
}
