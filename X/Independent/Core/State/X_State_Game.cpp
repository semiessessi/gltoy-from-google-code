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
#include <UI/GLToy_Widget_StatBar.h>
#include <UI/GLToy_WidgetTypes.h>

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
#include <Equipment/X_Weapon_Types.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

u_int X_State_Game::s_uScore = 0;

static const float fX_COLLECTIBLE_INTERVAL = 10.0f;

static const GLToy_Hash xENEMY_SHIP_MATERIAL_1 = GLToy_Hash_Constant( "Enemy/Enemy1" );
static const GLToy_Hash xENEMY_SHIP_MATERIAL_2 = GLToy_Hash_Constant( "Enemy/Enemy2" );

// SE - lazy globals avoid touching header/declaration...
static const float s_fInitialInterval = 30.0f;
static const float s_fInitialRespiteTime = 7.0f;
static const float s_fInitialFullOnTime = 10.0f;

static float s_fStageTimer = 0.0f;
static float s_fIntervalTimer = 0.0f;
static float s_fRespiteTimer = 0.0f;
static float s_fFullOnTimer = 0.0f;

static float s_fInterval = s_fInitialInterval;
static float s_fRespiteTime = s_fInitialRespiteTime;
static float s_fFullOnTime = s_fInitialFullOnTime;

static const float s_fMinInterval = 5.0f;
static const float s_fMinRespiteTime = 3.0f;

static const float s_fIntervalDecrease = 5.0f;
static const float s_fRespiteDecrease = 1.0f;
static const float s_fFullOnIncrease = 2.5f;

static u_int s_uCurrentSpawner = 0;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

// SE - more lazy
void CreateSpawner( X_Enemy_Spawner** const axSpawners )
{
    X_Enemy_Definition xDefinition;
	xDefinition.m_uBrain = eENEMY_BRAIN_SWARM;
    GLToy_Hash uSpawnType = eENEMY_SPAWNER_SCATTER;
    switch( s_uCurrentSpawner )
    {
        case 0:
        case 7:
        {
            xDefinition.m_uBrain == ( s_uCurrentSpawner ) ? eENEMY_BRAIN_SUICIDE : eENEMY_BRAIN_DIVE;
            uSpawnType = eENEMY_SPAWNER_SWEEP;
            break;
        }

        default:
        case 1:
        case 2:
        case 3:
        {
            break;
        }

        case 4:
        case 5:
        case 6:
        {
            xDefinition.m_uBrain = eENEMY_BRAIN_SUICIDE;
            break;
        }
    }

    xDefinition.m_uTexture = ( xDefinition.m_uBrain == eENEMY_BRAIN_DIVE ) ? xENEMY_SHIP_MATERIAL_1 : xENEMY_SHIP_MATERIAL_2;
    axSpawners[ s_uCurrentSpawner ] = X_Enemy_Spawner_Factory::CreateSpawner( uSpawnType, xDefinition );
    s_uCurrentSpawner++;
}

X_State_Game::X_State_Game()
: GLToy_Parent()
, m_pxPlayer( NULL )
, m_fCollectibleTimer( 0.0f )
, m_fStateTimer( 0.0f )
{
    for( u_int u = 0; u < uNUM_SPAWNERS; ++u )
    {
        m_apxSpawners[ u ] = NULL;
    }
}

void X_State_Game::Initialise()
{
    // clean up state...
    s_fStageTimer =
    s_fIntervalTimer =
    s_fRespiteTimer = 
    s_fFullOnTimer = 0.0f;
    
    s_fInterval = s_fInitialInterval;
    s_fRespiteTime = s_fInitialRespiteTime;
    s_fFullOnTime = s_fInitialFullOnTime;
    
    GLToy_UI_System::ShowPointer( false );
    GLToy_Camera::SetLocked( true );
	GLToy_Camera::SetPosition( GLToy_Vector_3( 0.0f, 0.0f, -1.0f ) );
	GLToy_Camera::SetOrientation( GLToy_Maths::IdentityMatrix3 );

    GLToy_Entity_System::DestroyEntities();
    GLToy_Entity_System::SetRender( true );

    for( u_int u = 0; u < uNUM_SPAWNERS; ++u )
    {
        m_apxSpawners[ u ] = NULL;
    }

	// create our player's entity
	m_pxPlayer = static_cast< X_Entity_Player* >( GLToy_Entity_System::CreateEntity( GLToy_Hash_Constant( "Player" ), X_ENTITY_PLAYER ) );
	if( m_pxPlayer )
	{
		m_pxPlayer->CreateWeapon( ePLAYER_WEAPON_VULCAN );
	}

	// Create a spawner
	X_Enemy_Definition xTestDef;
	xTestDef.m_uBrain = eENEMY_BRAIN_DIVE;
    xTestDef.m_uTexture = xENEMY_SHIP_MATERIAL_1;
	m_apxSpawners[ 0 ] = X_Enemy_Spawner_Factory::CreateSpawner( eENEMY_SPAWNER_SWEEP, xTestDef );
    s_uCurrentSpawner = 1;

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

    GLToy_UI_System::CreateStatBar( "BarTest.ptx", m_pxPlayer->GetShield(), 1.0f, 0.5f, -0.875f, 0.5f, 0.1f );

    s_uScore = 0;
}

void X_State_Game::Shutdown()
{
    GLToy_Light_System::Reset();
	GLToy_Entity_System::DestroyEntities();
	m_pxPlayer = NULL;
	for( u_int u = 0; u < uNUM_SPAWNERS; ++u )
    {
        delete m_apxSpawners[ u ];
        m_apxSpawners[ u ] = NULL;
    }

    GLToy_UI_System::ClearWidgets();
}

void X_State_Game::Update()
{
    for( u_int u = 0; u < uNUM_SPAWNERS; ++u )
    {
        if( m_apxSpawners[ u ] )
	    {
		    m_apxSpawners[ u ]->Update();
	    }
    }

	m_fCollectibleTimer -= GLToy_Timer::GetFrameTime();
    m_fStateTimer += GLToy_Timer::GetFrameTime();

    s_fStageTimer += GLToy_Timer::GetFrameTime();
    s_fIntervalTimer += GLToy_Timer::GetFrameTime();
    s_fRespiteTimer += ( s_uCurrentSpawner ) ? 0.0f : GLToy_Timer::GetFrameTime();
    s_fFullOnTimer += ( s_uCurrentSpawner != 8 ) ? 0.0f : GLToy_Timer::GetFrameTime();

    // if we have been full on for the whole time stop it all...
    if( s_fFullOnTimer > s_fFullOnTime )
    {
        s_fFullOnTimer = 0.0f;
        s_fRespiteTimer = 0.0f;
        s_uCurrentSpawner = 0;
      	for( u_int u = 0; u < uNUM_SPAWNERS; ++u )
        {
            delete m_apxSpawners[ u ];
            m_apxSpawners[ u ] = NULL;
        }
    }

    if( s_fRespiteTimer > s_fRespiteTime )
    {
        // move to the next stage...
        s_fRespiteTimer = 0.0f;
        s_fStageTimer = 0.0f;

        // start spawning again... always start with the simplest, easiest wave
        // making it more difficult by adding more things more quickly...
        X_Enemy_Definition xTestDef;
	    xTestDef.m_uBrain = eENEMY_BRAIN_DIVE;
        xTestDef.m_uTexture = xENEMY_SHIP_MATERIAL_1;
	    m_apxSpawners[ 0 ] = X_Enemy_Spawner_Factory::CreateSpawner( eENEMY_SPAWNER_SWEEP, xTestDef );
        s_uCurrentSpawner = 1;

        // make harder
        s_fInterval -= ( s_fInterval > s_fMinInterval ) ? s_fIntervalDecrease : 0.0f;
        s_fRespiteTime -= ( s_fRespiteTime > s_fMinRespiteTime ) ? s_fRespiteDecrease : 0.0f;
        s_fFullOnTime += s_fFullOnIncrease;
    }

    if( s_uCurrentSpawner && ( s_uCurrentSpawner < 8 ) && ( s_fIntervalTimer > s_fInterval ) )
    {
        s_fIntervalTimer = 0.0f;
        // fire up another spawner...
        CreateSpawner( m_apxSpawners );
    }

    //if( !( m_apxSpawners[ 1 ] ) && ( m_fStateTimer > 30.0f ) )
    //{
    //    X_Enemy_Definition xTestDef;
	   // xTestDef.m_uBrain = eENEMY_BRAIN_SWARM;
    //    xTestDef.m_uTexture = xENEMY_SHIP_MATERIAL_2;
    //    m_apxSpawners[ 1 ] = X_Enemy_Spawner_Factory::CreateSpawner( eENEMY_SPAWNER_SCATTER, xTestDef );
    //}

    //if( !( m_apxSpawners[ 2 ] ) && ( m_fStateTimer > 60.0f ) )
    //{
    //    X_Enemy_Definition xTestDef;
	   // xTestDef.m_uBrain = eENEMY_BRAIN_SUICIDE;
    //    xTestDef.m_uTexture = xENEMY_SHIP_MATERIAL_2;
    //    m_apxSpawners[ 2 ] = X_Enemy_Spawner_Factory::CreateSpawner( eENEMY_SPAWNER_SCATTER, xTestDef );
    //}

    //if( !( m_apxSpawners[ 3 ] ) && ( m_fStateTimer > 90.0f ) )
    //{
    //    delete m_apxSpawners[ 0 ];
    //    m_apxSpawners[ 0 ] = NULL;
    //    X_Enemy_Definition xTestDef;
	   // xTestDef.m_uBrain = eENEMY_BRAIN_SUICIDE;
    //    xTestDef.m_uTexture = xENEMY_SHIP_MATERIAL_2;
    //    m_apxSpawners[ 3 ] = X_Enemy_Spawner_Factory::CreateSpawner( eENEMY_SPAWNER_SWEEP, xTestDef );
    //}

    //if( !( m_apxSpawners[ 4 ] ) && ( m_fStateTimer > 120.0f ) )
    //{
    //    for( u_int u = 4; u < uNUM_SPAWNERS; ++u )
    //    {
    //        X_Enemy_Definition xTestDef;
	   //     xTestDef.m_uBrain = eENEMY_BRAIN_SWARM;
    //        xTestDef.m_uTexture = xENEMY_SHIP_MATERIAL_2;
    //        m_apxSpawners[ u ] = X_Enemy_Spawner_Factory::CreateSpawner( eENEMY_SPAWNER_SCATTER, xTestDef );
    //    }
    //}

    //if( !( m_apxSpawners[ 0 ] ) && ( m_fStateTimer > 150.0f ) )
    //{
    //    X_Enemy_Definition xTestDef;
	   // xTestDef.m_uBrain = eENEMY_BRAIN_DIVE;
    //    xTestDef.m_uTexture = xENEMY_SHIP_MATERIAL_1;
    //    m_apxSpawners[ 0 ] = X_Enemy_Spawner_Factory::CreateSpawner( eENEMY_SPAWNER_SWEEP, xTestDef );
    //}

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

	static bool bKeyDown = false;
	GLToy_Array< X_Entity_Player* >& xList = X_Entity_Player::GetList();
	if( GLToy_Input_System::IsKeyDown( GLToy_Input_System::GetSpaceKey() ) )
	{
		if( !bKeyDown )
		{
			xList[0]->StartShooting();
			bKeyDown = true;
		}
    }
	else
	{
		if( bKeyDown )
		{
			xList[0]->StopShooting();
			bKeyDown = false;
		}
	}
}

void X_State_Game::Render2D() const
{
    static GLToy_String szLives;
	static GLToy_String szShield;
    static GLToy_String szScore;
    szLives.SetToFormatString( "Lives: %d", m_pxPlayer->GetLives() );
	szShield.SetToFormatString( "Shield: %.1f%", m_pxPlayer->GetShield() );
    szScore.SetToFormatString( "Score: %d", s_uScore );

    GLToy_Font_System::RenderString( szLives, "FrontEnd", -GLToy_Render::GetAspectRatio(), 0.9f );
	GLToy_Font_System::RenderString( szShield, "FrontEnd", -GLToy_Render::GetAspectRatio(), 0.8f );
    GLToy_Font_System::RenderString( szScore, "FrontEnd", -GLToy_Render::GetAspectRatio(), 0.7f );
}
