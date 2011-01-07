/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010, 2011 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GLTOY_RENDERFUNCTOR_H_
#define __GLTOY_RENDERFUNCTOR_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Core/GLToy_Functor.h>
#include <Render/Shader/GLToy_Shader_System.h>

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
class GLToy_RenderDeferredFunctor
: public GLToy_ConstFunctor< T >
{

public:

    virtual void operator ()( const T* const pxRenderable )
    {
        const GLToy_Hash uShaderHash = pxRenderable->GetShaderHash();
        if( uShaderHash )
        {
            GLToy_Shader_System::BindShaderProgram( uShaderHash );
        }

        pxRenderable->RenderDeferred();
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
class GLToy_IndirectRenderDeferredFunctor
: public GLToy_ConstFunctor< T* >
{

public:

    virtual void operator ()( T* const* const pxRenderable )
    {
        const GLToy_Hash uShaderHash = ( *pxRenderable )->GetShaderHash();
        if( uShaderHash )
        {
            GLToy_Shader_System::BindShaderProgram( uShaderHash );
        }

        ( *pxRenderable )->RenderDeferred();
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
