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

#ifndef __GLTOY_PAIR_H_
#define __GLTOY_PAIR_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

template < class T, class U = T >
class GLToy_Pair
{

public:

    GLToy_Pair( const T& xFirst = T(), const U& xSecond = U() )
    : m_xFirst( xFirst )
    , m_xSecond( xSecond )
    {
    }

    T& First() { return m_xFirst; }
    const T& First() const { return m_xFirst; }

    U& Second() { return m_xSecond; }
    const U& Second() const { return m_xSecond; }

protected:

    T m_xFirst;
    U m_xSecond;

};

#endif
