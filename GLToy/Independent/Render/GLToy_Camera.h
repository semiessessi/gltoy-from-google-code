#ifndef __GLTOY_CAMERA_H_
#define __GLTOY_CAMERA_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Vector_3;

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

private:

    static GLToy_Vector_3 s_xPosition;
    static GLToy_Vector_3 s_xDirection;
    static GLToy_Vector_3 s_xUp;

    static float s_fRX;
    static float s_fRY;

};

#endif