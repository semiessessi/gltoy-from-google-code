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
#include <Entity/GLToy_Entity_System.h>
#include <Render/GLToy_Camera.h>

// SRC
#include <Entity/SRC_EntityTypes.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

SRC_Entity_Robot::SRC_Entity_Robot( const GLToy_Hash uHash, const u_int uType )
: GLToy_Parent( uHash, uType )
{
    SetBB( GLToy_AABB( GLToy_Vector_3( 32.0f, 32.0f, 32.0f ), GLToy_Vector_3( GLToy_Maths::ZeroVector3 ) ) );
}

SRC_Entity_Robot::~SRC_Entity_Robot()
{
}

void SRC_Entity_Robot::Render() const
{
    GLToy_Parent::RenderOBB();
}

void SRC_Entity_Robot::SpawnRobot_Console()
{
    static u_int ls_uID = 0;
    GLToy_Entity* const pxRobot = GLToy_Entity_System::CreateEntity( ( GLToy_String( "Robot " ) + ls_uID ).GetHash(), SRC_ENTITY_ROBOT );

    pxRobot->SetPosition( GLToy_Camera::GetPosition() );

    ++ls_uID;
}
