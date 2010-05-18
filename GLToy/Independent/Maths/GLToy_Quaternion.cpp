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

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Maths/GLToy_Quaternion.h>

// GLToy
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Matrix_3 GLToy_Quaternion::GetOrientationMatrix()
{
    const float& w = m_fComponents[ 0 ];
    const float& x = m_fComponents[ 1 ];
    const float& y = m_fComponents[ 2 ];
    const float& z = m_fComponents[ 3 ];

    GLToy_Matrix_3 xReturnValue(

        w*w + x*x - y*y - z*z,      2*x*y - 2*z*w,              2*x*z + 2*w*y,
        2*x*y + 2*w*z,              w*w - x*x + y*y - z*z,      2*y*z - 2*w*x,
        2*x*z - 2*w*y,              2*y*z + 2*w*x,              w*w - x*x - y*y + z*z

        );

    return xReturnValue;
}
