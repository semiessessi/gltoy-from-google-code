/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's headers
#include <Maths/GLToy_Maths.h>
#include <Maths/Platform_GLToy_Maths.h>

// C++ headers (!)
#include <math.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

const float GLToy_Maths::LargeFloat = 1.0e+35f;
const float GLToy_Maths::Pi = 3.141592654f;
const GLToy_Vector_3 GLToy_Maths::ZeroVector3 = GLToy_Vector_3();
const GLToy_Matrix_3 GLToy_Maths::IdentityMatrix3 = GLToy_Matrix_3( GLToy_Vector_3( 1.0f, 0.0f, 0.0f ), GLToy_Vector_3( 0.0f, 1.0f, 0.0f ), GLToy_Vector_3( 0.0f, 0.0f, 1.0f ) );

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Maths::Initialise()
{
    return Platform_Initialise();
}

void GLToy_Maths::Shutdown()
{
    Platform_Shutdown();
}

bool GLToy_Maths::Platform_Initialise()
{
    return Platform_GLToy_Maths::Initialise();
}

void GLToy_Maths::Platform_Shutdown()
{

}

float GLToy_Maths::Sqrt( const float fValue )
{
    return sqrt( fValue );
}

float GLToy_Maths::Floor( const float fValue )
{
    return floor( fValue );
}