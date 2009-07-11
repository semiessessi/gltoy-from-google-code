/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// This file's headers
#include <Maths/GLToy_Vector.h>
#include <Maths/Platform_GLToy_Vector.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// 2 D
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Vector_2::GLToy_Vector_2()
{
    fComponents[0] = fComponents[1] = 0.0f;
}

GLToy_Vector_2::GLToy_Vector_2( float fX, float fY )
{
    fComponents[0] = fX;
    fComponents[1] = fY;
}

GLToy_Vector_2::GLToy_Vector_2( const GLToy_Vector_2& xVector )
{
    fComponents[0] = xVector[0];
    fComponents[1] = xVector[1];
}

/////////////////////////////////////////////////////////////////////////////////////////////
// 3 D
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Vector_3::GLToy_Vector_3()
{
    fComponents[0]
        = fComponents[1]
        = fComponents[2]
        = 0.0f;
}

GLToy_Vector_3::GLToy_Vector_3( float fX, float fY, float fZ )
{
    fComponents[0] = fX;
    fComponents[1] = fY;
    fComponents[2] = fZ;
}

GLToy_Vector_3::GLToy_Vector_3( const GLToy_Vector_3& xVector )
{
    fComponents[0] = xVector[0];
    fComponents[1] = xVector[1];
    fComponents[2] = xVector[2];
}

GLToy_Vector_3 GLToy_Vector_3::operator +( const GLToy_Vector_3& xVector ) const
{
    return Platform_GLToy_Vector::Add( *this, xVector );
}

GLToy_Vector_3 GLToy_Vector_3::operator -( const GLToy_Vector_3& xVector ) const
{
    return Platform_GLToy_Vector::Sub( *this, xVector );
}

GLToy_Vector_3 GLToy_Vector_3::operator *( const float fFloat ) const
{
    return Platform_GLToy_Vector::Mul( fFloat, *this );
}

void GLToy_Vector_3::SendToGLAsVertex() const
{
    Platform_GLToy_Vector::SendToGLAsVertex( *this );
}

void GLToy_Vector_3::SendToGLAsNormal() const
{
    Platform_GLToy_Vector::SendToGLAsNormal( *this );
}

void GLToy_Vector_3::SendToGLAsColour() const
{
    Platform_GLToy_Vector::SendToGLAsColour( *this );
}

void GLToy_Vector_3::SendToGLAsTextureCoordinate( u_int uTextureUnit ) const
{
    Platform_GLToy_Vector::SendToGLAsTextureCoordinate( *this, uTextureUnit );
}

/////////////////////////////////////////////////////////////////////////////////////////////
// 4 D
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Vector_4::GLToy_Vector_4()
{
    fComponents[0]
        = fComponents[1]
        = fComponents[2]
        = fComponents[3]
        = 0.0f;
}

GLToy_Vector_4::GLToy_Vector_4( float fX, float fY, float fZ, float fW )
{
    fComponents[0] = fX;
    fComponents[1] = fY;
    fComponents[2] = fZ;
    fComponents[3] = fW;
}

GLToy_Vector_4::GLToy_Vector_4( const GLToy_Vector_4& xVector )
{
    fComponents[0] = xVector[0];
    fComponents[1] = xVector[1];
    fComponents[2] = xVector[2];
    fComponents[3] = xVector[3];
}