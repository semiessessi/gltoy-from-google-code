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