/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010 Semi Essessi
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
#include <Environment/GLToy_Environment_System.h>
#include <Entity/GLToy_Entity_System.h>
#include <Maths/GLToy_Maths.h>
#include <Render/GLToy_Camera.h>
#include <UI/GLToy_UI_System.h>

// X
#include <Entity/X_EntityTypes.h>
#include <Entity/Player/X_Entity_Player.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

X_State_Game::X_State_Game()
: GLToy_Parent()
, m_pxPlayer( NULL )
{
}

void X_State_Game::Initialise()
{
    GLToy_UI_System::ShowPointer( false );
    GLToy_Camera::SetLocked( true );
	GLToy_Camera::SetPosition( GLToy_Maths::ZeroVector3 );
	GLToy_Camera::SetOrientation( GLToy_Maths::IdentityMatrix3 );

    GLToy_Entity_System::SetRender( true );

	// create our player's entity
	m_pxPlayer = static_cast< X_Entity_Player* >( GLToy_Entity_System::CreateEntity( GLToy_Hash_Constant( "Player" ), X_ENTITY_PLAYER ) );
}

void X_State_Game::Shutdown()
{
	GLToy_Entity_System::DestroyEntities();
	m_pxPlayer = NULL;
}

void X_State_Game::Update()
{
	// since our controls are so simple, handle them here partly and forward the relevant bits to the entity
}
