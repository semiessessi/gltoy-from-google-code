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
#include <Core/State/X_State_Game.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Core/Console/GLToy_Console.h>
#include <Environment/GLToy_Environment_System.h>
#include <Entity/GLToy_Entity_System.h>
#include <Input/GLToy_Input_System.h>
#include <Maths/GLToy_Maths.h>
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_Light_System.h>
#include <Render/GLToy_Render.h>
#include <Render/Font/GLToy_Font.h>
#include <UI/GLToy_UI_System.h>

// X
#include "AI/X_Enemy_Brain.h"
#include "AI/X_Enemy_Brain_Types.h"
#include "Core/X_Spawner.h"
#include "Core/X_Spawner_Types.h"
#include <Entity/X_EntityTypes.h>
#include <Entity/Collectible/X_Entity_Collectible.h>
#include <Entity/Enemy/X_Entity_Enemy.h>
#include <Entity/Player/X_Entity_Player.h>
#include <Entity/Projectile/X_Entity_Projectile.h>
#include <Equipment/X_Equipment_Weapon.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

u_int X_State_Game::s_uScore = 0;

static const float fX_COLLECTIBLE_INTERVAL = 10.0f;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

static void MakeWeapon()
{
	GLToy_Array< X_Entity_Player* >& xList = X_Entity_Player::GetList();
	if( xList.IsEmpty() )
	{
		return;
	}

	X_Equipment_Weapon xWeapon;
	xWeapon.RandomGenerate();
    xWeapon.Boost( xList[0]->GetWeapon().GetBoost() );
	xList[0]->SetWeapon( xWeapon );
}

X_State_Game::X_State_Game()
: GLToy_Parent()
, m_pxPlayer( NULL )
, m_fCollectibleTimer( 0.0f )
, m_fStateTimer( 0.0f )
, m_pxTestSpawner( 0 )
{
	GLToy_Console::RegisterCommand( "makeweapon", MakeWeapon );
}

void X_State_Game::Initialise()
{
    GLToy_UI_System::ShowPointer( false );
    GLToy_Camera::SetLocked( true );
	GLToy_Camera::SetPosition( GLToy_Vector_3( 0.0f, 0.0f, -1.0f ) );
	GLToy_Camera::SetOrientation( GLToy_Maths::IdentityMatrix3 );

    GLToy_Entity_System::DestroyEntities();
    GLToy_Entity_System::SetRender( true );

	// create our player's entity
	m_pxPlayer = static_cast< X_Entity_Player* >( GLToy_Entity_System::CreateEntity( GLToy_Hash_Constant( "Player" ), X_ENTITY_PLAYER ) );

	// Create a spawner
	X_Enemy_Definition xTestDef;
	xTestDef.m_uBrain = eENEMY_BRAIN_DIVE;
	m_pxTestSpawner = X_Enemy_Spawner_Factory::CreateSpawner( eENEMY_SPAWNER_SWEEP, xTestDef );

	m_fCollectibleTimer = fX_COLLECTIBLE_INTERVAL * 2.0f;
	m_fStateTimer = 0.0f;

    GLToy_Light_System::Reset();
    GLToy_Light_System::AddGlobalDirectionalLight( GLToy_Vector_3( 0.308f, 0.308f, -0.9f ), GLToy_Vector_3( 1.0f, 1.0f, 1.0f ) );
	GLToy_Light_PointProperties xProps;
	xProps.m_fSphereRadius = 4.0f;
	xProps.m_uFlags = 0;
	xProps.m_xColour[0] = 1.0f;
	xProps.m_xColour[1] = 1.0f;
	xProps.m_xColour[2] = 1.0f;
	xProps.m_xPosition = GLToy_Maths::ZeroVector3;
	xProps.m_xPosition.z -= 1.0f;
	GLToy_Light_System::AddPointLight( 1, xProps );

    s_uScore = 0;

    MakeWeapon();
}

void X_State_Game::Shutdown()
{
    GLToy_Light_System::Reset();
	GLToy_Entity_System::DestroyEntities();
	m_pxPlayer = NULL;
	delete m_pxTestSpawner;
}

void X_State_Game::Update()
{
    if( m_pxTestSpawner )
	{
		m_pxTestSpawner->Update();
	}

	m_fCollectibleTimer -= GLToy_Timer::GetFrameTime();
    m_fStateTimer += GLToy_Timer::GetFrameTime();

	const bool bLeft = GLToy_Input_System::IsKeyDown( GLToy_Input_System::GetLeftKey() );
    const bool bRight = GLToy_Input_System::IsKeyDown( GLToy_Input_System::GetRightKey() );
    const bool bUp = GLToy_Input_System::IsKeyDown( GLToy_Input_System::GetUpKey() );
    const bool bDown = GLToy_Input_System::IsKeyDown( GLToy_Input_System::GetDownKey() );

    m_pxPlayer->SetMovement(
        GLToy_Vector_2(
                ( bRight ? 1.0f : 0.0f ) + ( bLeft ? -1.0f : 0.0f ),
                ( bUp ? 1.0f : 0.0f ) + ( bDown ? -1.0f : 0.0f ) ) );

	

	if( m_fCollectibleTimer < 0.0f )
	{
		GLToy_Hash xHash = X_COLLECTIBLE_TYPE_BOOST;
		if( GLToy_Maths::Random() > 0.95f )
		{
			xHash = X_COLLECTIBLE_TYPE_LIFE;
		}
		else if( GLToy_Maths::Random() > 0.8f )
		{
			xHash = X_COLLECTIBLE_TYPE_WEAPON;
		}

		X_Entity_Collectible* pxEntity = static_cast<X_Entity_Collectible*>( GLToy_Entity_System::CreateEntity( GLToy_Random_Hash(), X_ENTITY_COLLECTIBLE ) );
		pxEntity->SetCollectType( xHash );
		pxEntity->SetPosition( GLToy_Vector_3( GLToy_Maths::Random( -1.0f, 1.0f ), 1.5f, 0.0f ) );

		m_fCollectibleTimer = fX_COLLECTIBLE_INTERVAL;
	}

	GLToy_Array< X_Entity_Player* >& xList = X_Entity_Player::GetList();
	if( GLToy_Input_System::IsKeyDown( GLToy_Input_System::GetSpaceKey() ) )
	{
		xList[0]->Shoot();
    }
}

void X_State_Game::Render2D() const
{
    static GLToy_String szLives;
    static GLToy_String szScore;
    szLives.SetToFormatString( "Lives: %d", m_pxPlayer->GetLives() );
    szScore.SetToFormatString( "Score: %d", s_uScore );

    GLToy_Font_System::RenderString( szLives, "FrontEnd", -GLToy_Render::GetAspectRatio(), 0.9f );
    GLToy_Font_System::RenderString( szScore, "FrontEnd", -GLToy_Render::GetAspectRatio(), 0.8f );
}
