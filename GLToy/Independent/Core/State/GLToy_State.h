#ifndef __GLTOY_STATE_H_
#define __GLTOY_STATE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_String;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_State
{

public:
    
    GLToy_State();
    virtual ~GLToy_State();

    virtual const GLToy_String& GetName() const = 0;

    virtual void Initialise() = 0;
    virtual void Shutdown() = 0;

};

class GLToy_State_System
{

public:

    static bool Initialise();
    static void Shutdown();

};

#endif