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

#include <Core/FPSToy.h>

// This file's header
#include <Entity/Actor/FPSToy_Entity_Actor.h>

// GLToy
#include <Model/GLToy_Model_MD2.h>
#include <Physics/GLToy_Physics_Object.h>
#include <Physics/GLToy_Physics_System.h>
#include <Render/GLToy_Render.h>
#include <String/GLToy_String.h>

// FPSToy
#include <AI/FPSToy_AI_Zombie.h>
#include <Weapon/FPSToy_Weapon_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void FPSToy_Entity_Actor::Render() const
{
    if( !HasSpawned() )
    {
        return;
    }

	//GLToy_Parent::Render();
	GLToy_Render::RegisterDeferred( this );
	//m_pxPhysicsObject->GetOBB().Render();
}

void FPSToy_Entity_Actor::RenderDeferred() const
{
    if( !HasSpawned() )
    {
        return;
    }

	GLToy_Parent::RenderDeferred();
	//m_pxPhysicsObject->GetOBB().Render();
}

void FPSToy_Entity_Actor::Update()
{
    if( !HasSpawned() || IsDead() )
    {
		if( !m_bWasDead )
		{
			StopAnimation( GLToy_Model_MD2::ANIMID_RUN );
			PlayAnimation( GLToy_Model_MD2::ANIMID_DEATH_FALLBACKSLOW, 0.0f, -GLToy_Maths::LargeFloat );
			m_bWasDead = true;
			m_pxPhysicsObject->ControlMovement( GLToy_Maths::ZeroVector2 );
			m_pxPhysicsObject->SetVelocity( GLToy_Maths::ZeroVector3 );
		}
		GLToy_Parent::Update();
        return;
    }

	SetPosition( m_pxPhysicsObject->GetPosition() );

	GLToy_Parent::Update();

	// TODO: FIXME: Possible divide by zero here!
	GLToy_Vector_3 xDesiredVelocity = m_pxAI->GetDesiredVelocity();
	float fSpeed = xDesiredVelocity.Magnitude();
	xDesiredVelocity.y = 0.0f;
	xDesiredVelocity.Normalise();
	m_pxPhysicsObject->ControlMovement( fSpeed * GLToy_Vector_2( xDesiredVelocity.x, xDesiredVelocity.z ) );

	SetOrientation( GLToy_Maths::Orientation_FromDirectionAndUp( xDesiredVelocity ) );

	if( m_pxAI )
	{
		m_pxAI->TellPosition( m_pxPhysicsObject->GetPosition() );
	}
}

void FPSToy_Entity_Actor::Spawn( const GLToy_Vector_3& xPosition, const GLToy_Matrix_3& xOrientation )
{
	m_pxPhysicsObject = GLToy_Physics_System::CreateControlledCapsule( GetHash(), xPosition );
	m_pxPhysicsObject->SetPosition( xPosition );

	SetModel( "megaman" );

	m_fHealth = 50.0f;

	m_pxAI = new FPSToy_AI_Zombie();

	ForceAnimStackCreation();
	PlayAnimation( GLToy_Model_MD2::ANIMID_RUN );
}
