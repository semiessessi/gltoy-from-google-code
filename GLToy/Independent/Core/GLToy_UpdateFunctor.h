#ifndef __GLTOY_UPDATEFUNCTOR_H_
#define __GLTOY_UPDATEFUNCTOR_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy_Functor.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class T >
class GLToy_UpdateFunctor
: public GLToy_Functor< T >
{

public:

    virtual void operator ()( T* const pxUpdateable )
    {
        pxUpdateable->Update();
    }
};

template < class T >
class GLToy_IndirectUpdateFunctor
: public GLToy_ConstFunctor< T* >
{

public:

    virtual void operator ()( T* const* const pxUpdateable )
    {
        ( *pxUpdateable )->Update();
    }
};

#endif