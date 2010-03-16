#ifndef __RTTOY_H_
#define __RTTOY_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/Project_GLToy_Config.h>
#include <Core/GLToy.h>

#include <Core/Platform_RTToy.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class RTToy
{

public:

    static bool Initialise();
    static void Shutdown();

};

#endif