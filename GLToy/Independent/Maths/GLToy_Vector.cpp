/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// This file's headers
#include <Maths/GLToy_Vector.h>
#include <Maths/Platform_GLToy_Vector.h>

// GLToy
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// 2 D
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Vector_2::GLToy_Vector_2()
{
    m_fComponents[0] = m_fComponents[1] = 0.0f;
}

GLToy_Vector_2::GLToy_Vector_2( float fX, float fY )
{
    m_fComponents[0] = fX;
    m_fComponents[1] = fY;
}

GLToy_Vector_2::GLToy_Vector_2( const GLToy_Vector_2& xVector )
{
    m_fComponents[0] = xVector[0];
    m_fComponents[1] = xVector[1];
}

/////////////////////////////////////////////////////////////////////////////////////////////
// 3 D
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Vector_3::GLToy_Vector_3()
{
    // TODO - use platform zero
    m_fComponents[0]
        = m_fComponents[1]
        = m_fComponents[2]
        = 0.0f;
}

GLToy_Vector_3::GLToy_Vector_3( float fX, float fY, float fZ )
{
    m_fComponents[0] = fX;
    m_fComponents[1] = fY;
    m_fComponents[2] = fZ;
}

GLToy_Vector_3::GLToy_Vector_3( const GLToy_Vector_3& xVector )
{
    // TODO - use platform copy
    m_fComponents[0] = xVector[0];
    m_fComponents[1] = xVector[1];
    m_fComponents[2] = xVector[2];
}

GLToy_Vector_3& GLToy_Vector_3::operator =( const GLToy_Vector_3& xVector )
{
    // TODO - use platform copy
    m_fComponents[0] = xVector[0];
    m_fComponents[1] = xVector[1];
    m_fComponents[2] = xVector[2];

    return *this;
}

GLToy_Vector_3 GLToy_Vector_3::operator -() const
{
    // TODO - use platform negate
    return GLToy_Vector_3( -m_fComponents[ 0 ], -m_fComponents[ 1 ], -m_fComponents[ 2 ] );
}

GLToy_Vector_3& GLToy_Vector_3::operator +=( const GLToy_Vector_3& xVector )
{
    // TODO - use platform add
    m_fComponents[ 0 ] += xVector.m_fComponents[ 0 ];
    m_fComponents[ 1 ] += xVector.m_fComponents[ 1 ];
    m_fComponents[ 2 ] += xVector.m_fComponents[ 2 ];
    return *this;
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

bool GLToy_Vector_3::operator ==( const GLToy_Vector_3& xVector ) const
{
    // TODO - use platform compare
    return ( m_fComponents[0] == xVector[0] )
        && ( m_fComponents[1] == xVector[1] )
        && ( m_fComponents[2] == xVector[2] );
}

GLToy_Vector_3 GLToy_Vector_3::operator *( const GLToy_Matrix_3& xMatrix ) const
{
    GLToy_Vector_3 xReturnValue;
    for( u_int u = 0; u < 3; ++u )
    {
        xReturnValue[ u ] = 0.0f;
        
        // V_a = V^c M^a_c
        for( u_int w = 0; w < 3; ++w )
        {
            xReturnValue[ u ] += m_fComponents[ w ] * xMatrix.m_xComponents[ u ][ w ];
        }
    }

    return xReturnValue;
}

void GLToy_Vector_3::Normalise()
{
    // TODO - this is lazy shit
    const float fFactor = 1.0f / ( *this * *this );
    *this = *this * GLToy_Maths::Sqrt( fFactor );
}

float GLToy_Vector_3::Magnitude() const
{
    return GLToy_Maths::Sqrt( *this * *this );
}

/////////////////////////////////////////////////////////////////////////////////////////////
// 4 D
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Vector_4::GLToy_Vector_4()
{
    m_fComponents[0]
        = m_fComponents[1]
        = m_fComponents[2]
        = m_fComponents[3]
        = 0.0f;
}

GLToy_Vector_4::GLToy_Vector_4( float fX, float fY, float fZ, float fW )
{
    m_fComponents[0] = fX;
    m_fComponents[1] = fY;
    m_fComponents[2] = fZ;
    m_fComponents[3] = fW;
}

GLToy_Vector_4::GLToy_Vector_4( const GLToy_Vector_4& xVector )
{
    m_fComponents[0] = xVector[0];
    m_fComponents[1] = xVector[1];
    m_fComponents[2] = xVector[2];
    m_fComponents[3] = xVector[3];
}