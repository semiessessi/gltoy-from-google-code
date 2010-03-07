#ifndef __GLTOY_FUNCTOR_H_
#define __GLTOY_FUNCTOR_H_


template < class T >
class GLToy_ConstFunctor
{

public:

    virtual void operator ()( const T* pxData ) = 0;
};

template < class T >
class GLToy_Functor
{

public:

    virtual void operator ()( T* pxData ) = 0;
};

#endif