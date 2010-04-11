/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009, 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of GLToy.
//
// GLToy is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// GLToy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with GLToy.
// If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Render/GLToy_Camera.h>

// GLToy
#include <Core/Console/GLToy_Console.h>
#include <Core/GLToy_Timer.h>
#include <Input/GLToy_Input.h>
#include <Maths/GLToy_Vector.h>
#include <Render/GLToy_Render.h>

// C/C++
#include <math.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const float fCAMERA_SPEED = 128.0f;
static const float fCAMERA_ROTATION_SPEED = 2.0f;
static const float fCAMERA_MOUSE_SCALE = 1.0f / 100.0f;

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Vector_3 GLToy_Camera::s_xPosition = GLToy_Vector_3( 0.0f, 50.0f, -300.0f );
GLToy_Vector_3 GLToy_Camera::s_xDirection = GLToy_Vector_3( 0.0f, 0.0f, 1.0f );
GLToy_Vector_3 GLToy_Camera::s_xUp = GLToy_Vector_3( 0.0f, 1.0f, 0.0f );
float GLToy_Camera::s_fRX = 0.0f;
float GLToy_Camera::s_fRY = 0.0f;
GLToy_Matrix_3 GLToy_Camera::s_xOrientation;
GLToy_Matrix_3 GLToy_Camera::s_xInverseOrientation;
bool GLToy_Camera::s_bFlyCam = true;
bool GLToy_Camera::s_bLockedCam = false;
bool GLToy_Camera::s_bControllerCam = false;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Camera::Initialise()
{
    GLToy_Console::RegisterCommand( "fpscam", SetControllerCamEnabled );
    GLToy_Console::RegisterCommand( "flycam", SetFlyCamEnabled );
    return true;
}

// TODO - find a better home for the fly-cam code

void GLToy_Camera::Update()
{
    s_xOrientation = GLToy_Matrix_3( GetRight(), s_xUp, s_xDirection );
    s_xInverseOrientation = s_xOrientation;
    s_xInverseOrientation.Transpose();

    if( s_bFlyCam || s_bControllerCam )
    {
        // update orientation ...
        if( GLToy_Input_System::IsKeyDown( GLToy_Input_System::GetLeftKey() ) )
        {
            s_fRY -= GLToy_Timer::GetFrameTime() * fCAMERA_ROTATION_SPEED;
        }

        if( GLToy_Input_System::IsKeyDown( GLToy_Input_System::GetRightKey() ) )
        {
            s_fRY += GLToy_Timer::GetFrameTime() * fCAMERA_ROTATION_SPEED;
        }

        s_fRY += GLToy_Input_System::GetMouseDeltaX() * fCAMERA_MOUSE_SCALE;
        s_fRX += GLToy_Input_System::GetMouseDeltaY() * fCAMERA_MOUSE_SCALE;
        s_fRX = GLToy_Maths::Clamp( s_fRX, -( GLToy_Maths::Pi * 0.5f ), GLToy_Maths::Pi * 0.5f );
    }

    // ... then calculate basis from the orientation
    const float fSRX = sin( s_fRX );
    const float fCRX = cos( s_fRX );
    const float fSRY = sin( s_fRY );
    const float fCRY = cos( s_fRY );
    s_xDirection = GLToy_Vector_3( fCRX * fSRY, -fSRX, fCRX * fCRY );
    s_xUp = GLToy_Vector_3( fSRX * fSRY, fCRX, fSRX * fCRY );
    if( s_bFlyCam )
    {
        const GLToy_Vector_3 xRight = GetRight();

        if( GLToy_Input_System::IsKeyDown( 'W' )
            || GLToy_Input_System::IsKeyDown( GLToy_Input_System::GetUpKey() ) )
        {
            s_xPosition = s_xPosition + s_xDirection * GLToy_Timer::GetFrameTime() * fCAMERA_SPEED;
        }

        if( GLToy_Input_System::IsKeyDown( 'S' )
            || GLToy_Input_System::IsKeyDown( GLToy_Input_System::GetDownKey() ) )
        {
            s_xPosition = s_xPosition - s_xDirection * GLToy_Timer::GetFrameTime() * fCAMERA_SPEED;
        }

        if( GLToy_Input_System::IsKeyDown( 'A' ) )
        {
            s_xPosition = s_xPosition - xRight * GLToy_Timer::GetFrameTime() * fCAMERA_SPEED;
        }

        if( GLToy_Input_System::IsKeyDown( 'D' ) )
        {
            s_xPosition = s_xPosition + xRight * GLToy_Timer::GetFrameTime() * fCAMERA_SPEED;
        }
    }
}

void GLToy_Camera::ApplyTransforms()
{
    GLToy_Render::SetPerspectiveProjectionMatrix();

    GLToy_Render::SetIdentityViewMatrix();

    GLToy_Matrix_3 xOrientation = GetOrientation();
    // TODO - re-work platform look at code to avoid gluLookAt and hide this too.
    xOrientation[ 2 ] = -xOrientation[ 2 ]; // silly OpenGL specific twiddling
    GLToy_Render::Transform( xOrientation );

    GLToy_Render::Translate( -s_xPosition );
}

void GLToy_Camera::SetLocked( const bool bLocked )
{
    s_bLockedCam = bLocked;
    if( bLocked )
    {
        s_bFlyCam = false;
        s_bControllerCam = false;
    }
}

void GLToy_Camera::SetControllerCamEnabled( const bool bEnabled )
{
    if( !s_bLockedCam )
    {
        s_bControllerCam = bEnabled;
        // pretend this is a console variable
        GLToy_Console::Print( GLToy_String( "fpscam is set to " ) + ( bEnabled ? "true" : "false" ) );
        if( bEnabled )
        {
            s_bFlyCam = false;
        }
    }
}

void GLToy_Camera::SetFlyCamEnabled( const bool bEnabled )
{
    if( !s_bLockedCam )
    {
        s_bFlyCam = bEnabled;
        // pretend this is a console variable
        GLToy_Console::Print( GLToy_String( "flycam is set to " ) + ( bEnabled ? "true" : "false" ) );
        if( bEnabled )
        {
            s_bControllerCam = false;
        }
    }
}