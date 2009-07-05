#ifndef __TESTCUBE_RENDER_H_
#define __TESTCUBE_RENDER_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class TestCube_Render
{

public:

    static bool Initialise();
    static void Shutdown();

    static void Render();

private:

    static void Platform_Render();

};

#endif