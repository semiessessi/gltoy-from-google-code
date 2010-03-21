#ifndef __GLTOY_STATE_EDITORFRONTEND_H_
#define __GLTOY_STATE_EDITORFRONTEND_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Core/State/GLToy_State.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_State_EditorFrontEnd
: public GLToy_State
{

public:

    virtual void Initialise();
    virtual void Shutdown();

    virtual void Render() const;
    virtual void Update();

};

#endif