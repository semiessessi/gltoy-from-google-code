#ifndef __PHYSICS_H_
#define __PHYSICS_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/Project_GLToy_Config.h>
#include <Core/GLToy.h>

#include <Core/Platform_Physics.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class Physics
{

public:

    static bool Initialise();
    static void Shutdown();

    static void Update();

};

#endif