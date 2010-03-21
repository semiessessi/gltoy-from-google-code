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

    virtual void Initialise() {}
    virtual void Shutdown() {}

    // "normal" render function - must be overloaded
    virtual void Render() const = 0;

    // 2d render function
    virtual void Render2D() const {}
    
    // for deferred shading
    virtual void RenderDepth() const {}
    virtual void RenderNormals() const {}
    virtual void RenderSpecular() const {}

    // transparent render pass
    virtual void RenderTransparent() const {}

};

#endif