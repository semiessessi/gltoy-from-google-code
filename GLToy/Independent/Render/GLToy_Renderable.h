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

class GLToy_Renderable_Transparent
: public GLToy_Renderable
{

public:

    virtual ~GLToy_Renderable_Transparent() {}

    virtual void RenderTransparent() const = 0;

};

#endif