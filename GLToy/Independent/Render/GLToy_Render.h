#ifndef __GLTOY_RENDER_H_
#define __GLTOY_RENDER_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Render
{

public:

	static bool Initialise();
	static void Shutdown();

	static void Render();

private:

    static bool Project_Initialise();
    static void Project_Shutdown();

    static void Project_Render();

	static bool Platform_Initialise();
	static void Platform_Shutdown();

	static void Platform_BeginRender();
	static void Platform_EndRender();

};

#endif