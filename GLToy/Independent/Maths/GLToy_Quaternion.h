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

#ifndef __GLTOY_QUATERNION_H_
#define __GLTOY_QUATERNION_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Maths/GLToy_Matrix.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Quaternion
{

public:

    GLToy_Quaternion()
    {
        m_fComponents[ 0 ] = 0.0f;
        m_fComponents[ 1 ] = 0.0f;
        m_fComponents[ 2 ] = 0.0f;
        m_fComponents[ 3 ] = 0.0f;
    }

    GLToy_Quaternion( const float fW, const float fX, const float fY, const float fZ )
    {
        m_fComponents[ 0 ] = fW;
        m_fComponents[ 1 ] = fX;
        m_fComponents[ 2 ] = fY;
        m_fComponents[ 3 ] = fZ;
    }

    virtual ~GLToy_Quaternion() {}

    GLToy_Matrix_3 GetOrientationMatrix();

protected:

    float m_fComponents[ 4 ];

};

#endif
