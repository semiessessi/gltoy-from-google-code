#ifndef __GLTOY_RENDERFUNCTOR_H_
#define __GLTOY_RENDERFUNCTOR_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy_Functor.h>
#include <Render/GLToy_Renderable.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class T >
class GLToy_RenderFunctor : public GLToy_ConstFunctor< T >
{

public:

    static GLToy_RenderFunctor& GetInstance()
    {
        static GLToy_RenderFunctor ls_xInstance = GLToy_RenderFunctor();
        return ls_xInstance;
    }

    virtual void operator ()( const T* pxRenderable )
    {
        pxRenderable->Render();
    }
};

#endif