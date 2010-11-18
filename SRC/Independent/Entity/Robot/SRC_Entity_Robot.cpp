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

#include <Core/SRC.h>

// This file's header
#include <Entity/Robot/SRC_Entity_Robot.h>

// GLToy
#include <Core/State/GLToy_State_System.h>
#include <Entity/GLToy_Entity_System.h>
#include <Environment/GLToy_Environment.h>
#include <Environment/GLToy_Environment_System.h>
#include <Input/GLToy_Input.h>
#include <Physics/GLToy_Physics_Object.h>
#include <Physics/GLToy_Physics_System.h>
#include <Render/GLToy_Camera.h>
#include <UI/GLToy_UI_System.h>

// SRC
#include <Entity/SRC_EntityTypes.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const float fSRC_ROBOT_SPEED = 32.0f;

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Array< SRC_Entity_Robot* > SRC_Entity_Robot::s_xInstanceList;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

SRC_Entity_Robot::SRC_Entity_Robot( const GLToy_Hash uHash, const u_int uType )
: GLToy_Parent( uHash, uType )
, m_xTargetPosition( GLToy_Maths::ZeroVector2 )
, m_bMoving( false )
, m_bOldMouseData( false )
{
    s_xInstanceList.Append( this );
}

SRC_Entity_Robot::~SRC_Entity_Robot()
{
    s_xInstanceList.RemoveByValue( this );
}

void SRC_Entity_Robot::Spawn( const GLToy_Vector_3& xPosition )
{
    SetPhysicsObject( GLToy_Physics_System::CreateControlledCapsule( GetHash(), xPosition, 0.5f ) );
}

void SRC_Entity_Robot::Render() const
{
    GLToy_Physics_Object* pxPhysicsObject = GetPhysicsObject();
    if( pxPhysicsObject )
    {
        pxPhysicsObject->GetOBB().Render();

        // render the direction of travel for debugging
        //GLToy_Ray( pxPhysicsObject->GetPosition(), pxPhysicsObject->GetMovementDirection() ).Render();
    }
}

void SRC_Entity_Robot::Update()
{
    GLToy_Physics_Object* pxPhysicsObject = GetPhysicsObject();

    // TODO: work out if this will be a problem for other things when doing trace etc... :/
    // maybe all objects should do something like this - afaik atm the BBs just diverge
    // maybe overload the getters/setters?
    if( pxPhysicsObject )
    {
        SetBB( pxPhysicsObject->GetOBB().GetUnrotatedBB() );
        SetOrientation( pxPhysicsObject->GetOBB().GetOrientation() );
    }

    if( pxPhysicsObject && GLToy_State_System::GetState() == GLToy_Hash_Constant( "Game" ) )
    {
        const bool bLeftDown = GLToy_Input_System::IsMouseLeftButtonDown();

        if( !bLeftDown && m_bOldMouseData )
        {
            const GLToy_Environment* pxEnvironment = GLToy_Environment_System::GetCurrentEnvironment();
            if( pxEnvironment )
            {
                const GLToy_Ray xRay = GLToy_Camera::ScreenSpaceToRay( GLToy_UI_System::GetMousePosition() );
                const float fDistance = pxEnvironment->Trace( xRay, 4096.0f );
                if( fDistance > 0.0f )
                {
                    const GLToy_Vector_3 xHitPosition = xRay.Evaluate( fDistance );
                    m_xTargetPosition[ 0 ] = xHitPosition[ 0 ];
                    m_xTargetPosition[ 1 ] = xHitPosition[ 2 ];
                    m_bMoving = true;
                }
            }
        }

        m_bOldMouseData = bLeftDown;
    }

    if( pxPhysicsObject )
    {
        GLToy_ConstIterate( GLToy_Physics_ObjectCollision, xIterator, &( m_pxPhysicsObject->GetCollisions() ) )
        {
            const GLToy_Physics_ObjectCollision& xCollision = xIterator.Current();
            if( false /* check the normal is sufficiently harsh */ )
            {
                // detonate
                if( xCollision.m_bHitEnvironment )
                {
                    m_bMoving = false;
                }
                else if( xCollision.m_bHitEntity )
                {
                    // ...
                }

                break;
            }
        }

        const GLToy_Vector_2 xActual( pxPhysicsObject->GetPosition()[ 0 ], pxPhysicsObject->GetPosition()[ 2 ] );
        GLToy_Vector_2 xDifference = m_xTargetPosition - xActual;
        if( xDifference.MagnitudeSquared() < 64.0f )
        {
            m_bMoving = false;
        }

        if( m_bMoving )
        {
            xDifference.Normalise();
            pxPhysicsObject->ControlMovement( xDifference * fSRC_ROBOT_SPEED );
        }
        else
        {
            pxPhysicsObject->ControlMovement( GLToy_Maths::ZeroVector2 );
        }
    }
}

void SRC_Entity_Robot::SpawnRobot_Console()
{
    static u_int ls_uID = 0;
    SRC_Entity_Robot* const pxRobot = static_cast< SRC_Entity_Robot* >( GLToy_Entity_System::CreateEntity( ( GLToy_String( "Robot " ) + ls_uID ).GetHash(), SRC_ENTITY_ROBOT ) );

    pxRobot->Spawn( GLToy_Camera::GetPosition() );

    ++ls_uID;
}
