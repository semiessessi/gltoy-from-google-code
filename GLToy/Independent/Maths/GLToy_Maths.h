#ifndef __GLTOY_MATHS_H_
#define __GLTOY_MATHS_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Maths/GLToy_Matrix.h>
#include <Maths/GLToy_Vector.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Maths
{

public:
    
    static const float Pi;
    static const GLToy_Vector_3 ZeroVector3;
    static const GLToy_Matrix_3 IdentityMatrix3;

    static bool Initialise();
    static void Shutdown();

    static float Clamp( float fValue, float fMin, float fMax ) { return Min( fMax, Max( fValue, fMin ) ); }
    static float Max( float fValue1, float fValue2 ) { return ( fValue1 > fValue2 ) ? fValue1 : fValue2; }
    static float Min( float fValue1, float fValue2 ) { return ( fValue1 < fValue2 ) ? fValue1 : fValue2; }

    static float Lerp( float fValue1, float fValue2, float fAmount ) { return fValue1 + ( fValue2 - fValue1 ) * fAmount; }
    //static GLToy_Vector_2 Lerp( const GLToy_Vector_2& xValue1, const GLToy_Vector_2& xValue2, float fAmount ) { return xValue1 + ( xValue2 - xValue1 ) * fAmount; }
    static GLToy_Vector_3 Lerp( const GLToy_Vector_3& xValue1, const GLToy_Vector_3& xValue2, float fAmount ) { return xValue1 + ( xValue2 - xValue1 ) * fAmount; }
    //static GLToy_Vector_4 Lerp( const GLToy_Vector_4& xValue1, const GLToy_Vector_4& xValue2, float fAmount ) { return xValue1 + ( xValue2 - xValue1 ) * fAmount; }

    static float Sqrt( const float fValue );

private:
    
    static bool Platform_Initialise();
    static void Platform_Shutdown();

};

#endif