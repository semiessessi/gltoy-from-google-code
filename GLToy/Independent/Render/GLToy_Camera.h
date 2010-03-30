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

#ifndef __GLTOY_CAMERA_H_
#define __GLTOY_CAMERA_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Camera
{

public:

    static bool Initialise();

    static void Update();

    static void ApplyTransforms();

    static const GLToy_Vector_3& GetPosition() { return s_xPosition; }
    static const GLToy_Vector_3& GetDirection() { return s_xDirection; }
    static const GLToy_Vector_3& GetUp() { return s_xUp; }
    static const GLToy_Vector_3 GetRight() { return s_xUp.Cross( s_xDirection ); }
    static GLToy_Matrix_3 GetOrientation() { return s_xOrientation; }
    static GLToy_Matrix_3 GetInverseOrientation() { return s_xInverseOrientation; }

    static void SetPosition( const GLToy_Vector_3& xPosition ) { s_xPosition = xPosition; }

    static void SetFlyCamEnabled( const bool bEnabled = true ) { s_bFlyCam = bEnabled; }

private:

    static GLToy_Vector_3 s_xPosition;
    static GLToy_Vector_3 s_xDirection;
    static GLToy_Vector_3 s_xUp;

    static float s_fRX;
    static float s_fRY;

    static GLToy_Matrix_3 s_xOrientation;
    static GLToy_Matrix_3 s_xInverseOrientation;

    static bool s_bFlyCam;

};

#endif