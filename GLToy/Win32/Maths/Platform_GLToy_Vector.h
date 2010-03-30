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

#ifndef __PLATFORM_GLTOY_VECTOR_H_
#define __PLATFORM_GLTOY_VECTOR_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// This file's header
#include <Maths/GLToy_Vector.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class Platform_GLToy_Vector
{
    
    friend class GLToy_Vector_2;
    friend class GLToy_Vector_3;
    friend class GLToy_Vector_4;

public:
    
    static void Initialise();

private:
    
    static GLToy_Vector_3 ( *pfnAdd )( const GLToy_Vector_3&, const GLToy_Vector_3& );
    static GLToy_Vector_3 ( *pfnSub )( const GLToy_Vector_3&, const GLToy_Vector_3& );
    static GLToy_Vector_3 ( *pfnMul )( const float, const GLToy_Vector_3& );

    static GLToy_Vector_3 Add( const GLToy_Vector_3& xVector1, const GLToy_Vector_3& xVector2 );
    static GLToy_Vector_3 Sub( const GLToy_Vector_3& xVector1, const GLToy_Vector_3& xVector2 );
    static GLToy_Vector_3 Mul( const float fFloat, const GLToy_Vector_3& xVector );

    static GLToy_Vector_3 Add_Default( const GLToy_Vector_3& xVector1, const GLToy_Vector_3& xVector2 );
    static GLToy_Vector_3 Sub_Default( const GLToy_Vector_3& xVector1, const GLToy_Vector_3& xVector2 );
    static GLToy_Vector_3 Mul_Default( const float fFloat, const GLToy_Vector_3& xVector );
};

#endif