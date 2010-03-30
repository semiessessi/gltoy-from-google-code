/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009, 2010 Semi Essessi
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

// This file's headers
#include <Maths/GLToy_Vector.h>
#include <Maths/Platform_GLToy_Vector.h>

// Win32
#include <windows.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Vector_3 ( *Platform_GLToy_Vector::pfnAdd )( const GLToy_Vector_3&, const GLToy_Vector_3& )
    = Platform_GLToy_Vector::Add_Default;
GLToy_Vector_3 ( *Platform_GLToy_Vector::pfnSub )( const GLToy_Vector_3&, const GLToy_Vector_3& )
    = Platform_GLToy_Vector::Sub_Default;
GLToy_Vector_3 ( *Platform_GLToy_Vector::pfnMul )( const float, const GLToy_Vector_3& )
    = Platform_GLToy_Vector::Mul_Default;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Vector_3 Platform_GLToy_Vector::Add_Default(const GLToy_Vector_3& xVector1, const GLToy_Vector_3& xVector2)
{
    return GLToy_Vector_3( xVector1[0] + xVector2[0], xVector1[1] + xVector2[1], xVector1[2] + xVector2[2] );
}

GLToy_Vector_3 Platform_GLToy_Vector::Sub_Default(const GLToy_Vector_3& xVector1, const GLToy_Vector_3& xVector2)
{
    return GLToy_Vector_3( xVector1[0] - xVector2[0], xVector1[1] - xVector2[1], xVector1[2] - xVector2[2] );
}

GLToy_Vector_3 Platform_GLToy_Vector::Mul_Default( const float fFloat, const GLToy_Vector_3& xVector )
{
    return GLToy_Vector_3( fFloat * xVector[0], fFloat * xVector[1], fFloat * xVector[2] );
}

GLToy_Vector_3 Platform_GLToy_Vector::Add(const GLToy_Vector_3& xVector1, const GLToy_Vector_3& xVector2)
{
    return pfnAdd( xVector1, xVector2 );
}

GLToy_Vector_3 Platform_GLToy_Vector::Sub(const GLToy_Vector_3& xVector1, const GLToy_Vector_3& xVector2)
{
    return pfnSub( xVector1, xVector2 );
}

GLToy_Vector_3 Platform_GLToy_Vector::Mul(const float fFloat, const GLToy_Vector_3& xVector)
{
    return pfnMul( fFloat, xVector );
}