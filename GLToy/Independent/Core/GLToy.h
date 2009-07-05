#ifndef __GLTOY_H_
#define __GLTOY_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// L I B R A R I E S
/////////////////////////////////////////////////////////////////////////////////////////////

#ifdef WIN32
    #pragma comment( lib, "opengl32" )
    #pragma comment( lib, "glu32" )
#endif

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// core
#include <Core/GLToy_Assert.h>
#include <Core/GLToy_SimpleStructures.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

typedef unsigned int u_int;

class GLToy
{
	
public:

	static int EntryPoint();
	static bool Resize( const int& iWidth, const int& iHeight );
	static void UpdateBuffers();

private:

	static bool Initialise();
	static void Shutdown();

	static bool MainLoop();

	static bool Platform_Resize( const int& iWidth, const int& iHeight );
	static void Platform_UpdateBuffers();

	static bool Platform_EarlyInitialise();
	static bool Platform_LateInitialise();
	static void Platform_Shutdown();

	static bool Platform_MainLoop();

	static bool s_bFullscreen;
	static int s_iWidth;
	static int s_iHeight;

#ifdef WIN32
	friend unsigned int WndProc( unsigned int uWindowHandle, unsigned int uMessage,
									unsigned int uWParam, unsigned int uLParam );
#endif

};

#endif
