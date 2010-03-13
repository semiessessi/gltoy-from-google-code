#ifndef __GLTOY_ENVIRONMENT_SYSTEM_H_
#define __GLTOY_ENVIRONMENT_SYSTEM_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Environment;
class GLToy_String;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Environment_System
{

public:

    static bool Initialise();
    static void Shutdown();

    static void Update();
    static void Render();

    static void LoadTestEnvironment();

private:

    static GLToy_Environment* CreateFromOBJFile( const GLToy_String& szFilename );

    static GLToy_Environment* s_pxCurrentEnvironment;

};

#endif