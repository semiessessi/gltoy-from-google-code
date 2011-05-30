/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009-2011 Semi Essessi
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
#include <Render/GLToy_Camera.h>

// GLToy
#include <Core/Console/GLToy_Console.h>
#include <Core/GLToy_Timer.h>
#include <Input/GLToy_Input_System.h>
#include <Maths/GLToy_Vector.h>
#include <Physics/GLToy_Physics_System.h>
#include <Render/GLToy_Render.h>
#include <UI/GLToy_UI_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const float fCAMERA_SPEED = 16.0f;
static const float fCAMERA_ROTATION_SPEED = 1.4f;
static const float fCAMERA_MOUSE_SCALE = 1.0f / 100.0f;
static const float fCAMERA_OVERCAMHEIGHT_MIN = 32.0f;
static const float fCAMERA_OVERCAMHEIGHT_MAX = 512.0f;
static const float fCAMERA_OVERCAMHEIGHT_MOUSE_SCALE = 256.0f;

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Vector_3 GLToy_Camera::s_xPosition = GLToy_Vector_3( 0.0f, 50.0f, -300.0f );
GLToy_Vector_3 GLToy_Camera::s_xDirection = GLToy_Vector_3( 0.0f, 0.0f, 1.0f );
GLToy_Vector_3 GLToy_Camera::s_xUp = GLToy_Vector_3( 0.0f, 1.0f, 0.0f );
float GLToy_Camera::s_fRX = 0.0f;
float GLToy_Camera::s_fRY = 0.0f;
GLToy_Matrix_3 GLToy_Camera::s_xOrientation = GLToy_Maths::IdentityMatrix3;
GLToy_Matrix_3 GLToy_Camera::s_xInverseOrientation = GLToy_Maths::IdentityMatrix3;
GLToy_Matrix_4 GLToy_Camera::s_xViewMatrix = GLToy_Maths::IdentityMatrix4;
GLToy_Matrix_4 GLToy_Camera::s_xInverseViewMatrix = GLToy_Maths::IdentityMatrix4;
bool GLToy_Camera::s_bFlyCam = true;
bool GLToy_Camera::s_bLockedCam = false;
bool GLToy_Camera::s_bControllerCam = false;
bool GLToy_Camera::s_bOverCam = false;
float GLToy_Camera::s_fOverCamHeight = 256.0f;
bool GLToy_Camera::s_bOverCamMouseIsDown = false;
GLToy_Vector_2 GLToy_Camera::s_xOverCamMouseDown;
GLToy_Vector_2 GLToy_Camera::s_xOverCamOldRot;
float GLToy_Camera::s_fOverCamOldHeight = 0.0f;
float GLToy_Camera::s_fSpeed = fCAMERA_SPEED;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Camera::Initialise()
{
    GLToy_Console::RegisterCommand( "fpscam", SetControllerCamEnabled );
    GLToy_Console::RegisterCommand( "flycam", SetFlyCamEnabled );
    GLToy_Console::RegisterCommand( "overcam", SetOverCamEnabled );
    return true;
}

void GLToy_Camera::Update()
{
    s_xOrientation = GLToy_Matrix_3( GetRight(), s_xUp, s_xDirection );
    s_xInverseOrientation = s_xOrientation;
    s_xInverseOrientation.InvertTransformationMatrix();

    s_xViewMatrix = s_xOrientation;
    s_xViewMatrix[ 3 ] = s_xPosition;
    s_xInverseViewMatrix = s_xViewMatrix;
    s_xInverseViewMatrix.InvertTransformationMatrix();

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

	if( s_bOverCam && GLToy_Input_System::IsMouseRightButtonDown() )
	{
		if( !s_bOverCamMouseIsDown )
		{
			s_bOverCamMouseIsDown = true;
			s_xOverCamMouseDown = GLToy_UI_System::GetMousePosition(); // TY - This function needs moving to the input system
			s_xOverCamOldRot = GLToy_Vector_2( s_fRX, s_fRY );
			s_fOverCamOldHeight = s_fOverCamHeight;
		}
		else
		{
			GLToy_Vector_2 xDelta = s_xOverCamMouseDown - GLToy_UI_System::GetMousePosition();

			s_fOverCamHeight =  GLToy_Maths::Min( GLToy_Maths::Max( s_fOverCamOldHeight + xDelta[1] * fCAMERA_OVERCAMHEIGHT_MOUSE_SCALE, fCAMERA_OVERCAMHEIGHT_MIN ), fCAMERA_OVERCAMHEIGHT_MAX );
			s_fRY = s_xOverCamOldRot[1] + xDelta[0] * fCAMERA_ROTATION_SPEED;
		}
	}
	else
	{
		s_bOverCamMouseIsDown = false;
	}
	

    // ... then calculate basis from the orientation
    const float fSRX = GLToy_Maths::Sin( s_fRX );
    const float fCRX = GLToy_Maths::Cos( s_fRX );
    const float fSRY = GLToy_Maths::Sin( s_fRY );
    const float fCRY = GLToy_Maths::Cos( s_fRY );
    s_xDirection = GLToy_Vector_3( fCRX * fSRY, -fSRX, fCRX * fCRY );
    s_xUp = GLToy_Vector_3( fSRX * fSRY, fCRX, fSRX * fCRY );
    if( s_bFlyCam || ( s_bOverCam && !GLToy_Input_System::IsMouseRightButtonDown() ) )
    {
        const GLToy_Vector_3 xRight = GetRight();
        GLToy_Vector_3 xDirection = s_xDirection;

        if( s_bOverCam )
        {
            xDirection[ 1 ] = 0.0f;
            xDirection.Normalise();
        }

        if( GLToy_Input_System::IsKeyDown( 'W' )
            || GLToy_Input_System::IsKeyDown( GLToy_Input_System::GetUpKey() ) )
        {
            s_xPosition = s_xPosition + xDirection * GLToy_Timer::GetFrameTime() * s_fSpeed;
        }

        if( GLToy_Input_System::IsKeyDown( 'S' )
            || GLToy_Input_System::IsKeyDown( GLToy_Input_System::GetDownKey() ) )
        {
            GLToy_Vector_3 xDirection = s_xDirection;
            s_xPosition = s_xPosition - xDirection * GLToy_Timer::GetFrameTime() * s_fSpeed;
        }

        if( GLToy_Input_System::IsKeyDown( 'A' )
            || ( s_bOverCam && GLToy_Input_System::IsKeyDown( GLToy_Input_System::GetLeftKey() ) ) )
        {
            s_xPosition = s_xPosition - xRight * GLToy_Timer::GetFrameTime() * s_fSpeed;
        }

        if( GLToy_Input_System::IsKeyDown( 'D' )
            || ( s_bOverCam && GLToy_Input_System::IsKeyDown( GLToy_Input_System::GetRightKey() ) ) )
        {
            s_xPosition = s_xPosition + xRight * GLToy_Timer::GetFrameTime() * s_fSpeed;
        }
    }

    if( s_bOverCam )
    {
        s_xDirection = GLToy_Maths::Rotate_AxisAngle( s_xDirection, GetRight(), GLToy_Maths::Pi * 0.25f );
        s_xUp = GLToy_Maths::Rotate_AxisAngle( s_xUp, GetRight(), GLToy_Maths::Pi * 0.25f );
    }
}

void GLToy_Camera::ApplyTransforms()
{
    GLToy_Render::SetPerspectiveProjectionMatrix();

    GLToy_Render::SetIdentityViewMatrix();

    GLToy_Matrix_3 xOrientation = GetOrientation();
    xOrientation.Orthonormalise();
    xOrientation[ 2 ] = -xOrientation[ 2 ]; // silly OpenGL specific twiddling
    GLToy_Render::Transform( xOrientation );
    
    GLToy_Render::Translate( -GetPosition() );
}

void GLToy_Camera::LookAt( const GLToy_Vector_3& xPosition )
{
    s_xDirection = xPosition - s_xPosition;
    s_xDirection.Normalise();
    s_xUp = GLToy_Vector_3( s_xDirection[ 1 ], s_xDirection[ 2 ], s_xDirection[ 0 ] ); 
    s_xUp = s_xDirection.Cross( GetRight() );
    s_xUp.Normalise();
}

void GLToy_Camera::Reset()
{
    s_xPosition = GLToy_Maths::ZeroVector3;
    s_xDirection = GLToy_Vector_3( 0.0f, 0.0f, 1.0f );
    s_xUp = GLToy_Vector_3( 0.0f, 1.0f, 0.0f );
}

void GLToy_Camera::SetLocked( const bool bLocked )
{
    s_bLockedCam = bLocked;
    if( bLocked )
    {
        s_xPosition = GetPosition();
        GLToy_Physics_System::SetDefaultControllerActive( false );
        s_bFlyCam = false;
        s_bControllerCam = false;
        s_bOverCam = false;
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
            s_xPosition = GetPosition();
            GLToy_Physics_System::SetDefaultControllerActive( true, GetPosition() );
            s_bFlyCam = false;
            s_bOverCam = false;
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
            s_xPosition = GetPosition();
            GLToy_Physics_System::SetDefaultControllerActive( false );
            s_bControllerCam = false;
            s_bOverCam = false;
        }
    }
}

void GLToy_Camera::SetOverCamEnabled( const bool bEnabled )
{
    if( !s_bLockedCam )
    {
        s_bOverCam = bEnabled;
        // pretend this is a console variable
        GLToy_Console::Print( GLToy_String( "overcam is set to " ) + ( bEnabled ? "true" : "false" ) );
        if( bEnabled )
        {
            s_xPosition[ 1 ] = 0.0f;
            s_bFlyCam = false;
            s_bControllerCam = false;
        }
    }
}

GLToy_Vector_2 GLToy_Camera::WorldSpaceToScreenSpace( const GLToy_Vector_3& xWorldPosition )
{
    const GLToy_Vector_3 xTranslated = xWorldPosition - GetPosition();
    GLToy_Vector_3 xDirection = xTranslated * GetOrientation();
    xDirection.Normalise();

    GLToy_Plane xScreen = GLToy_Plane( GLToy_Vector_3( 0.0f, 0.0f, -1.0f ), 1.0f );

    GLToy_Vector_3 xPosition;
    float fParameter;
    if( GLToy_Ray( GLToy_Maths::ZeroVector3, xDirection ).IntersectsWithPlane( xScreen, &fParameter, &xPosition ) )
    {
        if( fParameter < 0.0f )
        {
            return GLToy_Maths::LargeVector2;
        }

        return GLToy_Vector_2( xPosition[ 0 ], xPosition[ 1 ] );
    }

    // just in case we get a direction parallel to the plane...
    return GLToy_Maths::LargeVector2;
}

GLToy_Ray GLToy_Camera::ScreenSpaceToRay( const GLToy_Vector_2& xPoint )
{
    GLToy_Vector_3 xDirection = GLToy_Vector_3( xPoint, 1.0f );
    xDirection.Normalise();

    GLToy_Matrix_3 xOrientation = GetOrientation();
    xOrientation.Orthonormalise();
    xOrientation.InvertTransformationMatrix();

    return GLToy_Ray( GetPosition(), xDirection * xOrientation );
}

bool GLToy_Camera::IsPointOnScreen( const GLToy_Vector_2& xPoint )
{
    if( ( xPoint[ 1 ] > 1.0f ) || ( xPoint[ 1 ] < -1.0f ) )
    {
        return false;
    }

    if( ( xPoint[ 0 ] > GLToy_Render::GetAspectRatio() ) || ( xPoint[ 0 ] < -GLToy_Render::GetAspectRatio() ) )
    {
        return false;
    }

    return true;
}

GLToy_ConvexHull GLToy_Camera::ViewFrustum()
{
    GLToy_ConvexHull xReturnValue( GetPosition() );
    // TODO: something correct... this is just the back plane of the view frustum
    xReturnValue.Append( GLToy_Plane( GetDirection(), -( GetDirection() * GetPosition() ) ) );

    return xReturnValue;
}

void GLToy_Camera::SetEuler( const float fX, const float fY, const float fZ )
{
    s_xDirection = GLToy_Vector_3( 0.0f, 0.0f, 1.0f );
    s_xUp = GLToy_Vector_3( 0.0f, 1.0f, 0.0f );
    
    s_xDirection = GLToy_Maths::Rotate_AxisAngle( s_xDirection, GLToy_Vector_3( 0.0f, 1.0f, 0.0f ), fY );
    s_xDirection = GLToy_Maths::Rotate_AxisAngle( s_xDirection, GLToy_Vector_3( 1.0f, 0.0f, 0.0f ), fX );
    s_xDirection = GLToy_Maths::Rotate_AxisAngle( s_xDirection, GLToy_Vector_3( 0.0f, 0.0f, 1.0f ), fZ );

    s_xUp = GLToy_Maths::Rotate_AxisAngle( s_xUp, GLToy_Vector_3( 0.0f, 1.0f, 0.0f ), fY );
    s_xUp = GLToy_Maths::Rotate_AxisAngle( s_xUp, GLToy_Vector_3( 1.0f, 0.0f, 0.0f ), fX );
    s_xUp = GLToy_Maths::Rotate_AxisAngle( s_xUp, GLToy_Vector_3( 0.0f, 0.0f, 1.0f ), fZ );
}
