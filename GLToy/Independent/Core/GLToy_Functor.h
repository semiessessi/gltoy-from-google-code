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

#ifndef __GLTOY_FUNCTOR_H_
#define __GLTOY_FUNCTOR_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class T >
class GLToy_ConstFunctor
{

public:

    virtual void operator ()( const T* const pxData ) = 0;
};

template < class T >
class GLToy_Functor
{

public:

    virtual void operator ()( T* const pxData ) = 0;
};

template < class T >
class GLToy_DeleteFunctor
: public GLToy_Functor< T >
{

public:

    GLToy_DeleteFunctor()
    {
    }

    virtual void operator ()( T* const pxData )
    {
        delete *pxData;
    }

};

#define GLToy_QuickFunctor( xName, T, xParameter, xCode ) class xName : public GLToy_Functor< T > { public: virtual void operator()( T* const xParameter ) { xCode } };

#endif