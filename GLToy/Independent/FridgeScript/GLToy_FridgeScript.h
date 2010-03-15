#ifdef GLTOY_USE_FRIDGESCRIPT

#ifndef __GLTOY_FRIDGESCRIPT_H_
#define __GLTOY_FRIDGESCRIPT_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <FridgeScript/Platform_GLToy_FridgeScript.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_FridgeScript
{

public:

    ~GLToy_FridgeScript();

    static bool Initialise();
    static void Shutdown();

    static GLToy_FridgeScript* CreateFromFile( const char* const szFilename );
    static void RegisterAPI( char* szName, void* pfnCallback, u_int uParameterCount );

    void Execute();

protected:

    GLToy_FridgeScript();

    void CompileFromFile( const char* const szFilename );

    u_int m_uCodeID;
    static u_int s_uContextID;

};

#endif

#endif
