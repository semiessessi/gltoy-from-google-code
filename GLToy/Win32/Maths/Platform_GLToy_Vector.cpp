/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's headers
#include <Maths/GLToy_Vector.h>
#include <Maths/Platform_GLToy_Vector.h>

// Win32
#include <windows.h>

// GL
#include <gl/gl.h>

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

void Platform_GLToy_Vector::SendToGLAsVertex( const GLToy_Vector_3& xVector )
{
    glVertex3fv( xVector.GetConstFloatPointer() );
}

void Platform_GLToy_Vector::SendToGLAsNormal( const GLToy_Vector_3& xVector )
{
    glNormal3fv( xVector.GetConstFloatPointer() );
}

void Platform_GLToy_Vector::SendToGLAsColour( const GLToy_Vector_3& xVector )
{
    glColor3fv( xVector.GetConstFloatPointer() );
}

void Platform_GLToy_Vector::SendToGLAsTextureCoordinate( const GLToy_Vector_3 &xVector, u_int uTextureUnit )
{
    // for now ignore the texture unit
    glTexCoord3fv( xVector.GetConstFloatPointer() );
}