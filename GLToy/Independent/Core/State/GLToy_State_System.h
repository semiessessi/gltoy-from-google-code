#ifndef __GLTOY_STATE_SYSTEM_H_
#define __GLTOY_STATE_SYSTEM_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Core/GLToy_Hash.h>
#include <String/GLToy_String.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class T > class GLToy_Array;
template < class T > class GLToy_HashTree;
class GLToy_State;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_State_System
{

public:

    static bool Initialise();
    static void Shutdown();

    static void Render();
    static void Render2D();
    static void Update();

    static void RegisterState( GLToy_State* const pxState, const GLToy_Hash uStateHash );
    static bool ChangeState( const GLToy_Hash uStateHash );
    static void ChangeState_Console( const GLToy_String& szName );

private:

    static GLToy_HashTree< GLToy_State* > s_xStates;
    static GLToy_State* s_pxCurrentState;

};

#endif