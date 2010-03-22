#ifndef __GLTOY_STATE_EDITOR_H_
#define __GLTOY_STATE_EDITOR_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Core/State/GLToy_State.h>
#include <String/GLToy_String.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_State_Editor
: public GLToy_State
{

public:

    virtual const GLToy_String& GetName() const { static GLToy_String ls_szName = "Editor"; return ls_szName; }

};

#endif