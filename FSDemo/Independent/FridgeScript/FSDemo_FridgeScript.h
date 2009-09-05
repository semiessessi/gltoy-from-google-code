#ifndef __FSDEMO_FRIDGESCRIPT_H_
#define __FSDEMO_FRIDGESCRIPT_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <FridgeScript/Platform_FSDemo_FridgeScript.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class FSDemo_FridgeScript
{

public:

    ~FSDemo_FridgeScript();

    static bool Initialise();
    static void Shutdown();

    static FSDemo_FridgeScript* CreateFromFile( const char* const szFilename );
    static void RegisterAPI( char* szName, void* pfnCallback, u_int uParameterCount );

    void Execute();

protected:

    FSDemo_FridgeScript();

    void CompileFromFile( const char* const szFilename );

    u_int m_uCodeID;
    static u_int s_uContextID;

};

#endif