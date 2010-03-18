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

enum GLToy_EnvironmentType
{
    ENV_PLANE = 0,

    NUM_ENV_TYPES
};

class GLToy_Environment_System
{

public:

    static bool Initialise();
    static void Shutdown();

    static void Update();
    static void Render();

    static void LoadEnvironmentFile( const GLToy_String& szName );
    static void SaveEnvironmentFile( const GLToy_String& szName );

    static void CreateTestEnvironment();
    static GLToy_Environment* CreateEnvironmentFromType( const GLToy_EnvironmentType eType );

    static const GLToy_Environment* GetCurrentEnvironment() { return s_pxCurrentEnvironment; }
    static bool SetRender( const bool bRender ) { s_bRender = bRender; };

    static void SwitchEnvironment( GLToy_Environment* const pxEnv );

private:

    static bool s_bRender;
    static GLToy_Environment* s_pxCurrentEnvironment;

    static GLToy_HashTree< GLToy_EnvironmentFile* > s_xEnvironments;

};

#endif