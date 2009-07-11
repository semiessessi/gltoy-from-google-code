#ifndef __GLTOY_RENDERABLE_H_
#define __GLTOY_RENDERABLE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

class GLToy_Renderable
{
public:

    GLToy_Renderable() {}
    virtual ~GLToy_Renderable() {}

    virtual void Initialise() {};
    virtual void Render() = 0;
    virtual void Shutdown() {}
};

#endif