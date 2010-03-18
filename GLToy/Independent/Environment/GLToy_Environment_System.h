#ifndef __GLTOY_ENVIRONMENT_SYSTEM_H_
#define __GLTOY_ENVIRONMENT_SYSTEM_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class T > class GLToy_HashTree;
class GLToy_Environment;
class GLToy_EnvironmentFile;
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

    static bool SetRender( const bool bRender ) { s_bRender = bRender; };

private:

    static bool s_bRender;
    static GLToy_Environment* s_pxCurrentEnvironment;

    static GLToy_HashTree< GLToy_EnvironmentFile* > s_xEnvironments;

};

#endif