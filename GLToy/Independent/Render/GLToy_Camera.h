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
    static const GLToy_Vector_3 GetLeft() { return s_xDirection.Cross( s_xUp ); }
    static GLToy_Matrix_3 GetOrientation() { return GLToy_Matrix_3( GetLeft(), s_xUp, s_xDirection ); }

private:

    static GLToy_Vector_3 s_xPosition;
    static GLToy_Vector_3 s_xDirection;
    static GLToy_Vector_3 s_xUp;

    static float s_fRX;
    static float s_fRY;

};

#endif