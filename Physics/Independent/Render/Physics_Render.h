#ifndef __PHYSICS_RENDER_H_
#define __PHYSICS_RENDER_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class Physics_Render
{

public:

    static bool Initialise();
    static void Shutdown();

    static void Render();

private:

    static void Platform_Render();

};

#endif