#ifndef __GLTOY_RENDERFUNCTOR_H_
#define __GLTOY_RENDERFUNCTOR_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy_Functor.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class T >
class GLToy_RenderFunctor
: public GLToy_ConstFunctor< T >
{

public:

    virtual void operator ()( const T* pxRenderable )
    {
        pxRenderable->Render();
    }
};

template < class T >
class GLToy_IndirectRenderFunctor
: public GLToy_ConstFunctor< T* >
{

public:

    virtual void operator ()( T* const* pxRenderable )
    {
        ( *pxRenderable )->Render();
    }
};

#endif