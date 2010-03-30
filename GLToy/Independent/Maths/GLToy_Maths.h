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

#ifndef __GLTOY_MATHS_H_
#define __GLTOY_MATHS_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Maths/GLToy_Matrix.h>
#include <Maths/GLToy_Quaternion.h>
#include <Maths/GLToy_Vector.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Maths
{

public:
    
    static const float LargeFloat;
    static const float Pi;
    static const GLToy_Vector_3 ZeroVector3;
    static const GLToy_Matrix_3 IdentityMatrix3;
    static const GLToy_Quaternion IdentityQuaternion;

    static bool Initialise();
    static void Shutdown();

    static float Ceiling( const float fValue );
    static float Floor( const float fValue );
    static float Round( const float fValue );

    static float Clamp( const float fValue, const float fMin, const float fMax ) { return Min( fMax, Max( fValue, fMin ) ); }
    static float Max( const float fValue1, const float fValue2 ) { return ( fValue1 > fValue2 ) ? fValue1 : fValue2; }
    static float Min( const float fValue1, const float fValue2 ) { return ( fValue1 < fValue2 ) ? fValue1 : fValue2; }
    static int Max( const int iValue1, const int iValue2 ) { return ( iValue1 > iValue2 ) ? iValue1 : iValue2; }
    static int Min( const int iValue1, const int iValue2 ) { return ( iValue1 < iValue2 ) ? iValue1 : iValue2; }

    static float Lerp( const float fValue1, const float fValue2, const float fAmount ) { return fValue1 + ( fValue2 - fValue1 ) * fAmount; }
    //static GLToy_Vector_2 Lerp( const GLToy_Vector_2& xValue1, const GLToy_Vector_2& xValue2, float fAmount ) { return xValue1 + ( xValue2 - xValue1 ) * fAmount; }
    static GLToy_Vector_3 Lerp( const GLToy_Vector_3& xValue1, const GLToy_Vector_3& xValue2, const float fAmount ) { return xValue1 + ( xValue2 - xValue1 ) * fAmount; }
    //static GLToy_Vector_4 Lerp( const GLToy_Vector_4& xValue1, const GLToy_Vector_4& xValue2, float fAmount ) { return xValue1 + ( xValue2 - xValue1 ) * fAmount; }

    static float Sin( const float fValue );
    static float Cos( const float fValue );

    static float Sqrt( const float fValue );

private:
    
    static bool Platform_Initialise();
    static void Platform_Shutdown();

};

#endif