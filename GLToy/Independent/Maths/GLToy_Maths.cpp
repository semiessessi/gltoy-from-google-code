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