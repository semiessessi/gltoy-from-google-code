/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009, 2010 Semi Essessi
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

#ifndef __GLTOY_CAMERA_H_
#define __GLTOY_CAMERA_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Maths/GLToy_Maths.h>
#include <Maths/GLToy_Ray.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Camera
{

public:

    static bool Initialise();

    static void Update();

    static void ApplyTransforms();
    static void LookAt( const GLToy_Vector_3& xPosition );
    static void Reset();

    // a bit of a nasty hack for overcam
    static const GLToy_Vector_3 GetPosition() { return s_bOverCam ? s_xPosition - s_xDirection * s_fOverCamHeight : s_xPosition; }
    static const GLToy_Vector_3& GetDirection() { return s_xDirection; }
    static const GLToy_Vector_3& GetUp() { return s_xUp; }
    static const GLToy_Vector_3 GetRight() { return s_xUp.Cross( s_xDirection ); }
    static GLToy_Matrix_3 GetOrientation() { return s_xOrientation; }
    static GLToy_Matrix_3 GetInverseOrientation() { return s_xInverseOrientation; }

    static void Set( const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xDirection, const GLToy_Vector_3& xUp ) { s_xPosition = xPosition; s_xDirection = xDirection; s_xUp = xUp; }
    static void SetEuler( const float fX, const float fY, const float fZ );
    static void SetPosition( const GLToy_Vector_3& xPosition ) { s_xPosition = xPosition; }
	static void SetOrientation( const GLToy_Matrix_3& xOrientation ) { s_xDirection = xOrientation[ 2 ]; s_xUp = xOrientation[ 1 ]; }

    static void SetLocked( const bool bLocked = true );
    static void SetFlyCamEnabled( const bool bEnabled = true );
    static void SetControllerCamEnabled( const bool bEnabled = true );
    static void SetOverCamEnabled( const bool bEnabled = true );

    static bool IsFlyCamEnabled() { return s_bFlyCam; }
    static bool IsControllerCamEnabled() { return s_bControllerCam; }
    static bool IsOverCamEnabled() { return s_bOverCam; }

    static GLToy_Vector_2 WorldSpaceToScreenSpace( const GLToy_Vector_3& xWorldPosition );
    static GLToy_Ray ScreenSpaceToRay( const GLToy_Vector_2& xPoint );
    static bool IsPointOnScreen( const GLToy_Vector_2& xPoint );

private:

    static GLToy_Vector_3 s_xPosition;
    static GLToy_Vector_3 s_xDirection;
    static GLToy_Vector_3 s_xUp;

    static float s_fRX;
    static float s_fRY;

    static GLToy_Matrix_3 s_xOrientation;
    static GLToy_Matrix_3 s_xInverseOrientation;

    static bool s_bLockedCam;
    static bool s_bFlyCam;
    static bool s_bControllerCam;
    static bool s_bOverCam;

    static float s_fOverCamHeight;
	static bool s_bOverCamMouseIsDown;
	static GLToy_Vector_2 s_xOverCamMouseDown;
	static GLToy_Vector_2 s_xOverCamOldRot;
	static float s_fOverCamOldHeight;
};

#endif
