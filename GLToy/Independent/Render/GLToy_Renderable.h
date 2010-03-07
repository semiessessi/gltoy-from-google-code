#ifndef __GLTOY_RENDERABLE_H_
#define __GLTOY_RENDERABLE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Renderable
{
public:

    GLToy_Renderable() {}
    virtual ~GLToy_Renderable() {}

    virtual void Initialise() {};
    virtual void Render() const = 0;
    virtual void Shutdown() {}
};

#endif