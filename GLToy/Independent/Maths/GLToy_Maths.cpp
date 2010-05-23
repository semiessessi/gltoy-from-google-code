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
#include <Maths/GLToy_Maths.h>
#include <Maths/Platform_GLToy_Maths.h>

// GLToy
#include <Core/GLToy_Timer.h>

// C++ headers (!)
#include <math.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

const float GLToy_Maths::LargeFloat = 1.0e+35f;
const GLToy_Vector_2 GLToy_Maths::LargeVector2 = GLToy_Vector_2( 1.0e+35f, 1.0e+35f );
const GLToy_Vector_3 GLToy_Maths::LargeVector3 = GLToy_Vector_3( 1.0e+35f, 1.0e+35f, 1.0e+35f );
const float GLToy_Maths::Pi = 3.141592654f;
const GLToy_Vector_2 GLToy_Maths::ZeroVector2 = GLToy_Vector_2();
const GLToy_Vector_3 GLToy_Maths::ZeroVector3 = GLToy_Vector_3();
const GLToy_Vector_4 GLToy_Maths::ZeroVector4 = GLToy_Vector_4();
const GLToy_Matrix_3 GLToy_Maths::IdentityMatrix3 = GLToy_Matrix_3( GLToy_Vector_3( 1.0f, 0.0f, 0.0f ), GLToy_Vector_3( 0.0f, 1.0f, 0.0f ), GLToy_Vector_3( 0.0f, 0.0f, 1.0f ) );
const GLToy_Quaternion GLToy_Maths::IdentityQuaternion = GLToy_Quaternion( 1.0f, 0.0f, 0.0f, 0.0f );

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Maths::Initialise()
{
    return Platform_Initialise();
}

void GLToy_Maths::Shutdown()
{
    Platform_Shutdown();
}

void GLToy_Maths::Update()
{
    Platform_Update();
}

bool GLToy_Maths::Platform_Initialise()
{
    return Platform_GLToy_Maths::Initialise();
}

void GLToy_Maths::Platform_Shutdown()
{
}

void GLToy_Maths::Platform_Update()
{
    Platform_GLToy_Maths::Update();
}

float GLToy_Maths::Sin( const float fValue )
{
    return sin( fValue );
}

float GLToy_Maths::Cos( const float fValue )
{
    return cos( fValue );
}

float GLToy_Maths::Sqrt( const float fValue )
{
    return Platform_GLToy_Maths::Sqrt( fValue );
}

GLToy_Vector_3 GLToy_Maths::Rotate_AxisAngle( const GLToy_Vector_3& xVector, const GLToy_Vector_3& xAxis, const float fAngle )
{
    if( fAngle == 0.0f )
    {
        return xVector;
    }

    // TODO: this can be optimised a load...
    const float fXSquared = xAxis[ 0 ] * xAxis[ 0 ];
    const float fYSquared = xAxis[ 1 ] * xAxis[ 1 ];
    const float fZSquared = xAxis[ 2 ] * xAxis[ 2 ];
    const float fXY = xAxis[ 0 ] * xAxis[ 1 ];
    const float fYZ = xAxis[ 1 ] * xAxis[ 2 ];
    const float fZX = xAxis[ 2 ] * xAxis[ 0 ];
    const float fC = Cos( fAngle );
    const float fS = Sin( fAngle );

    return xVector * GLToy_Matrix_3( 

            fXSquared + ( 1.0f - fXSquared ) * fC,     fXY * ( 1.0f - fC ) - xAxis[ 2 ] * fS,       fZX * ( 1.0f - fC ) + xAxis[ 1 ] * fS,

            fXY * ( 1.0f - fC ) + xAxis[ 2 ] * fS,     fYSquared + ( 1.0f - fYSquared ) * fC,       fYZ * ( 1.0f - fC ) - xAxis[ 0 ] * fS,

            fZX * ( 1.0f - fC ) - xAxis[ 1 ] * fS,     fYZ * ( 1.0f - fC ) + xAxis[ 0 ] * fS,       fZSquared + ( 1.0f - fZSquared ) * fC

        );
}

float GLToy_Maths::Ceiling( const float fValue )
{
    return ceil( fValue );
}

float GLToy_Maths::Floor( const float fValue )
{
    return floor( fValue );
}

float GLToy_Maths::Round( const float fValue )
{
    return Floor( fValue + 0.5f );
}

float GLToy_Maths::Random( const float fLower, const float fHigher )
{
    static u_int uWorkingValue = 0xA183E191;

    uWorkingValue += 37 * *reinterpret_cast< const u_int* >( &GLToy_Timer::GetTime() );

    return ( static_cast< float >( uWorkingValue ) / 4294967295.0f ) * ( fHigher - fLower ) + fLower;
}

float GLToy_Maths::Deg2Rad( const float fValue )
{
    static const float ls_fConversionFactor = Pi / 180.0f;
    return ls_fConversionFactor * fValue;
}

float GLToy_Maths::Rad2Deg( const float fValue )
{
    static const float ls_fConversionFactor = 180.0f / Pi;
    return ls_fConversionFactor * fValue;
}
