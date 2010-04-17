/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of GLToy.
//
// GLToy is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// GLToy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with GLToy.
// If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////////////////

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
class GLToy_Render2DFunctor
: public GLToy_ConstFunctor< T >
{

public:

    virtual void operator ()( const T* const pxRenderable )
    {
        pxRenderable->Render2D();
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
class GLToy_PointerRenderFunctor
: public GLToy_ConstFunctor< T >
{

public:

    virtual void operator ()( const T* const pxRenderable )
    {
        ( *pxRenderable )->Render();
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
class GLToy_IndirectRender2DFunctor
: public GLToy_ConstFunctor< T* >
{

public:

    virtual void operator ()( T* const* const pxRenderable )
    {
        ( *pxRenderable )->Render2D();
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