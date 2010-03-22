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

    virtual void operator ()( const T* const pxRenderable )
    {
        pxRenderable->Render();
    }
};

template < class T >
class GLToy_RenderTransparentFunctor
: public GLToy_ConstFunctor< T >
{

public:

    virtual void operator ()( const T* const pxRenderable )
    {
        pxRenderable->RenderTransparent();
    }
};

template < class T >
class GLToy_RenderAABBFunctor
: public GLToy_ConstFunctor< T >
{

public:

    virtual void operator ()( const T* const pxRenderable )
    {
        pxRenderable->RenderAABB();
    }
};

template < class T >
class GLToy_RenderOBBFunctor
: public GLToy_ConstFunctor< T >
{

public:

    virtual void operator ()( const T* const pxRenderable )
    {
        pxRenderable->RenderOBB();
    }
};

template < class T >
class GLToy_IndirectRenderFunctor
: public GLToy_ConstFunctor< T* >
{

public:

    virtual void operator ()( T* const* const pxRenderable )
    {
        ( *pxRenderable )->Render();
    }

};

template < class T >
class GLToy_IndirectRenderTransparentFunctor
: public GLToy_ConstFunctor< T* >
{

public:

    virtual void operator ()( T* const* const pxRenderable )
    {
        ( *pxRenderable )->RenderTransparent();
    }
};

template < class T >
class GLToy_IndirectRenderAABBFunctor
: public GLToy_ConstFunctor< T* >
{

public:

    virtual void operator ()( T* const* const pxRenderable )
    {
        ( *pxRenderable )->RenderAABB();
    }
};

template < class T >
class GLToy_IndirectRenderOBBFunctor
: public GLToy_ConstFunctor< T* >
{

public:

    virtual void operator ()( T* const* const pxRenderable )
    {
        ( *pxRenderable )->RenderOBB();
    }
};

#endif