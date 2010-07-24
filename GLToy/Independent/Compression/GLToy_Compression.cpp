/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Compression/GLToy_Compression.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const GLToy_Vector_3 xBYTE_NORMALS[] =
{
    GLToy_Vector_3( -0.525731f,  0.000000f,  0.850651f ), 
    GLToy_Vector_3( -0.442863f,  0.238856f,  0.864188f ), 
    GLToy_Vector_3( -0.295242f,  0.000000f,  0.955423f ), 
    GLToy_Vector_3( -0.309017f,  0.500000f,  0.809017f ), 
    GLToy_Vector_3( -0.162460f,  0.262866f,  0.951056f ), 
    GLToy_Vector_3(  0.000000f,  0.000000f,  1.000000f ), 
    GLToy_Vector_3(  0.000000f,  0.850651f,  0.525731f ), 
    GLToy_Vector_3( -0.147621f,  0.716567f,  0.681718f ), 
    GLToy_Vector_3(  0.147621f,  0.716567f,  0.681718f ), 
    GLToy_Vector_3(  0.000000f,  0.525731f,  0.850651f ), 
    GLToy_Vector_3(  0.309017f,  0.500000f,  0.809017f ), 
    GLToy_Vector_3(  0.525731f,  0.000000f,  0.850651f ), 
    GLToy_Vector_3(  0.295242f,  0.000000f,  0.955423f ), 
    GLToy_Vector_3(  0.442863f,  0.238856f,  0.864188f ), 
    GLToy_Vector_3(  0.162460f,  0.262866f,  0.951056f ), 
    GLToy_Vector_3( -0.681718f,  0.147621f,  0.716567f ), 
    GLToy_Vector_3( -0.809017f,  0.309017f,  0.500000f ), 
    GLToy_Vector_3( -0.587785f,  0.425325f,  0.688191f ), 
    GLToy_Vector_3( -0.850651f,  0.525731f,  0.000000f ), 
    GLToy_Vector_3( -0.864188f,  0.442863f,  0.238856f ), 
    GLToy_Vector_3( -0.716567f,  0.681718f,  0.147621f ), 
    GLToy_Vector_3( -0.688191f,  0.587785f,  0.425325f ), 
    GLToy_Vector_3( -0.500000f,  0.809017f,  0.309017f ), 
    GLToy_Vector_3( -0.238856f,  0.864188f,  0.442863f ), 
    GLToy_Vector_3( -0.425325f,  0.688191f,  0.587785f ), 
    GLToy_Vector_3( -0.716567f,  0.681718f, -0.147621f ), 
    GLToy_Vector_3( -0.500000f,  0.809017f, -0.309017f ), 
    GLToy_Vector_3( -0.525731f,  0.850651f,  0.000000f ), 
    GLToy_Vector_3(  0.000000f,  0.850651f, -0.525731f ), 
    GLToy_Vector_3( -0.238856f,  0.864188f, -0.442863f ), 
    GLToy_Vector_3(  0.000000f,  0.955423f, -0.295242f ), 
    GLToy_Vector_3( -0.262866f,  0.951056f, -0.162460f ), 
    GLToy_Vector_3(  0.000000f,  1.000000f,  0.000000f ), 
    GLToy_Vector_3(  0.000000f,  0.955423f,  0.295242f ), 
    GLToy_Vector_3( -0.262866f,  0.951056f,  0.162460f ), 
    GLToy_Vector_3(  0.238856f,  0.864188f,  0.442863f ), 
    GLToy_Vector_3(  0.262866f,  0.951056f,  0.162460f ), 
    GLToy_Vector_3(  0.500000f,  0.809017f,  0.309017f ), 
    GLToy_Vector_3(  0.238856f,  0.864188f, -0.442863f ), 
    GLToy_Vector_3(  0.262866f,  0.951056f, -0.162460f ), 
    GLToy_Vector_3(  0.500000f,  0.809017f, -0.309017f ), 
    GLToy_Vector_3(  0.850651f,  0.525731f,  0.000000f ), 
    GLToy_Vector_3(  0.716567f,  0.681718f,  0.147621f ), 
    GLToy_Vector_3(  0.716567f,  0.681718f, -0.147621f ), 
    GLToy_Vector_3(  0.525731f,  0.850651f,  0.000000f ), 
    GLToy_Vector_3(  0.425325f,  0.688191f,  0.587785f ), 
    GLToy_Vector_3(  0.864188f,  0.442863f,  0.238856f ), 
    GLToy_Vector_3(  0.688191f,  0.587785f,  0.425325f ), 
    GLToy_Vector_3(  0.809017f,  0.309017f,  0.500000f ), 
    GLToy_Vector_3(  0.681718f,  0.147621f,  0.716567f ), 
    GLToy_Vector_3(  0.587785f,  0.425325f,  0.688191f ), 
    GLToy_Vector_3(  0.955423f,  0.295242f,  0.000000f ), 
    GLToy_Vector_3(  1.000000f,  0.000000f,  0.000000f ), 
    GLToy_Vector_3(  0.951056f,  0.162460f,  0.262866f ), 
    GLToy_Vector_3(  0.850651f, -0.525731f,  0.000000f ), 
    GLToy_Vector_3(  0.955423f, -0.295242f,  0.000000f ), 
    GLToy_Vector_3(  0.864188f, -0.442863f,  0.238856f ), 
    GLToy_Vector_3(  0.951056f, -0.162460f,  0.262866f ), 
    GLToy_Vector_3(  0.809017f, -0.309017f,  0.500000f ), 
    GLToy_Vector_3(  0.681718f, -0.147621f,  0.716567f ), 
    GLToy_Vector_3(  0.850651f,  0.000000f,  0.525731f ), 
    GLToy_Vector_3(  0.864188f,  0.442863f, -0.238856f ), 
    GLToy_Vector_3(  0.809017f,  0.309017f, -0.500000f ), 
    GLToy_Vector_3(  0.951056f,  0.162460f, -0.262866f ), 
    GLToy_Vector_3(  0.525731f,  0.000000f, -0.850651f ), 
    GLToy_Vector_3(  0.681718f,  0.147621f, -0.716567f ), 
    GLToy_Vector_3(  0.681718f, -0.147621f, -0.716567f ), 
    GLToy_Vector_3(  0.850651f,  0.000000f, -0.525731f ), 
    GLToy_Vector_3(  0.809017f, -0.309017f, -0.500000f ), 
    GLToy_Vector_3(  0.864188f, -0.442863f, -0.238856f ), 
    GLToy_Vector_3(  0.951056f, -0.162460f, -0.262866f ), 
    GLToy_Vector_3(  0.147621f,  0.716567f, -0.681718f ), 
    GLToy_Vector_3(  0.309017f,  0.500000f, -0.809017f ), 
    GLToy_Vector_3(  0.425325f,  0.688191f, -0.587785f ), 
    GLToy_Vector_3(  0.442863f,  0.238856f, -0.864188f ), 
    GLToy_Vector_3(  0.587785f,  0.425325f, -0.688191f ), 
    GLToy_Vector_3(  0.688191f,  0.587785f, -0.425325f ), 
    GLToy_Vector_3( -0.147621f,  0.716567f, -0.681718f ), 
    GLToy_Vector_3( -0.309017f,  0.500000f, -0.809017f ), 
    GLToy_Vector_3(  0.000000f,  0.525731f, -0.850651f ), 
    GLToy_Vector_3( -0.525731f,  0.000000f, -0.850651f ), 
    GLToy_Vector_3( -0.442863f,  0.238856f, -0.864188f ), 
    GLToy_Vector_3( -0.295242f,  0.000000f, -0.955423f ), 
    GLToy_Vector_3( -0.162460f,  0.262866f, -0.951056f ), 
    GLToy_Vector_3(  0.000000f,  0.000000f, -1.000000f ), 
    GLToy_Vector_3(  0.295242f,  0.000000f, -0.955423f ), 
    GLToy_Vector_3(  0.162460f,  0.262866f, -0.951056f ), 
    GLToy_Vector_3( -0.442863f, -0.238856f, -0.864188f ), 
    GLToy_Vector_3( -0.309017f, -0.500000f, -0.809017f ), 
    GLToy_Vector_3( -0.162460f, -0.262866f, -0.951056f ), 
    GLToy_Vector_3(  0.000000f, -0.850651f, -0.525731f ), 
    GLToy_Vector_3( -0.147621f, -0.716567f, -0.681718f ), 
    GLToy_Vector_3(  0.147621f, -0.716567f, -0.681718f ), 
    GLToy_Vector_3(  0.000000f, -0.525731f, -0.850651f ), 
    GLToy_Vector_3(  0.309017f, -0.500000f, -0.809017f ), 
    GLToy_Vector_3(  0.442863f, -0.238856f, -0.864188f ), 
    GLToy_Vector_3(  0.162460f, -0.262866f, -0.951056f ), 
    GLToy_Vector_3(  0.238856f, -0.864188f, -0.442863f ), 
    GLToy_Vector_3(  0.500000f, -0.809017f, -0.309017f ), 
    GLToy_Vector_3(  0.425325f, -0.688191f, -0.587785f ), 
    GLToy_Vector_3(  0.716567f, -0.681718f, -0.147621f ), 
    GLToy_Vector_3(  0.688191f, -0.587785f, -0.425325f ), 
    GLToy_Vector_3(  0.587785f, -0.425325f, -0.688191f ), 
    GLToy_Vector_3(  0.000000f, -0.955423f, -0.295242f ), 
    GLToy_Vector_3(  0.000000f, -1.000000f,  0.000000f ), 
    GLToy_Vector_3(  0.262866f, -0.951056f, -0.162460f ), 
    GLToy_Vector_3(  0.000000f, -0.850651f,  0.525731f ), 
    GLToy_Vector_3(  0.000000f, -0.955423f,  0.295242f ), 
    GLToy_Vector_3(  0.238856f, -0.864188f,  0.442863f ), 
    GLToy_Vector_3(  0.262866f, -0.951056f,  0.162460f ), 
    GLToy_Vector_3(  0.500000f, -0.809017f,  0.309017f ), 
    GLToy_Vector_3(  0.716567f, -0.681718f,  0.147621f ), 
    GLToy_Vector_3(  0.525731f, -0.850651f,  0.000000f ), 
    GLToy_Vector_3( -0.238856f, -0.864188f, -0.442863f ), 
    GLToy_Vector_3( -0.500000f, -0.809017f, -0.309017f ), 
    GLToy_Vector_3( -0.262866f, -0.951056f, -0.162460f ), 
    GLToy_Vector_3( -0.850651f, -0.525731f,  0.000000f ), 
    GLToy_Vector_3( -0.716567f, -0.681718f, -0.147621f ), 
    GLToy_Vector_3( -0.716567f, -0.681718f,  0.147621f ), 
    GLToy_Vector_3( -0.525731f, -0.850651f,  0.000000f ), 
    GLToy_Vector_3( -0.500000f, -0.809017f,  0.309017f ), 
    GLToy_Vector_3( -0.238856f, -0.864188f,  0.442863f ), 
    GLToy_Vector_3( -0.262866f, -0.951056f,  0.162460f ), 
    GLToy_Vector_3( -0.864188f, -0.442863f,  0.238856f ), 
    GLToy_Vector_3( -0.809017f, -0.309017f,  0.500000f ), 
    GLToy_Vector_3( -0.688191f, -0.587785f,  0.425325f ), 
    GLToy_Vector_3( -0.681718f, -0.147621f,  0.716567f ), 
    GLToy_Vector_3( -0.442863f, -0.238856f,  0.864188f ), 
    GLToy_Vector_3( -0.587785f, -0.425325f,  0.688191f ), 
    GLToy_Vector_3( -0.309017f, -0.500000f,  0.809017f ), 
    GLToy_Vector_3( -0.147621f, -0.716567f,  0.681718f ), 
    GLToy_Vector_3( -0.425325f, -0.688191f,  0.587785f ), 
    GLToy_Vector_3( -0.162460f, -0.262866f,  0.951056f ), 
    GLToy_Vector_3(  0.442863f, -0.238856f,  0.864188f ), 
    GLToy_Vector_3(  0.162460f, -0.262866f,  0.951056f ), 
    GLToy_Vector_3(  0.309017f, -0.500000f,  0.809017f ), 
    GLToy_Vector_3(  0.147621f, -0.716567f,  0.681718f ), 
    GLToy_Vector_3(  0.000000f, -0.525731f,  0.850651f ), 
    GLToy_Vector_3(  0.425325f, -0.688191f,  0.587785f ), 
    GLToy_Vector_3(  0.587785f, -0.425325f,  0.688191f ), 
    GLToy_Vector_3(  0.688191f, -0.587785f,  0.425325f ), 
    GLToy_Vector_3( -0.955423f,  0.295242f,  0.000000f ), 
    GLToy_Vector_3( -0.951056f,  0.162460f,  0.262866f ), 
    GLToy_Vector_3( -1.000000f,  0.000000f,  0.000000f ), 
    GLToy_Vector_3( -0.850651f,  0.000000f,  0.525731f ), 
    GLToy_Vector_3( -0.955423f, -0.295242f,  0.000000f ), 
    GLToy_Vector_3( -0.951056f, -0.162460f,  0.262866f ), 
    GLToy_Vector_3( -0.864188f,  0.442863f, -0.238856f ), 
    GLToy_Vector_3( -0.951056f,  0.162460f, -0.262866f ), 
    GLToy_Vector_3( -0.809017f,  0.309017f, -0.500000f ), 
    GLToy_Vector_3( -0.864188f, -0.442863f, -0.238856f ), 
    GLToy_Vector_3( -0.951056f, -0.162460f, -0.262866f ), 
    GLToy_Vector_3( -0.809017f, -0.309017f, -0.500000f ), 
    GLToy_Vector_3( -0.681718f,  0.147621f, -0.716567f ), 
    GLToy_Vector_3( -0.681718f, -0.147621f, -0.716567f ), 
    GLToy_Vector_3( -0.850651f,  0.000000f, -0.525731f ), 
    GLToy_Vector_3( -0.688191f,  0.587785f, -0.425325f ), 
    GLToy_Vector_3( -0.587785f,  0.425325f, -0.688191f ), 
    GLToy_Vector_3( -0.425325f,  0.688191f, -0.587785f ), 
    GLToy_Vector_3( -0.425325f, -0.688191f, -0.587785f ), 
    GLToy_Vector_3( -0.587785f, -0.425325f, -0.688191f ), 
    GLToy_Vector_3( -0.688191f, -0.587785f, -0.425325f )
};

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

u_int GLToy_Compress::Float_3Bytes( const float fFloat )
{
    // (sign) (6-bit exponent) (17-bit mantissa)
    // put the sign the msb
    u_int uReturnValue = ( fFloat < 0.0f ) ? ( 1 << 23 ) : 0;

    // extract the exponent and throw away two bits
    const u_int uFloat = *reinterpret_cast< const u_int* >( &fFloat );
    int iExponent = ( uFloat & 0x78000000 ) ? GLToy_Maths::Min( 32, GLToy_Maths::Max( -31, ( ( uFloat & 0x78000000 ) >> 23 ) - 127 ) ) : 0;
    uReturnValue |= ( iExponent + 31 ) << 22;

    // finally throwaway the trailing 6 bits of mantissa
    uReturnValue |= ( uFloat & 0x7FFFFF ) >> 6;

    return uReturnValue;
}

GLToy_5Bytes GLToy_Compress::Vector_5Bytes( const GLToy_Vector_3& xVector )
{
    const float fMag = xVector.Magnitude();

    GLToy_Vector_3 xDir = xVector;
    xDir.Normalise();
    const u_short usDir = UnitVector_2Bytes( xDir );

    GLToy_5Bytes xReturnValue = { Float_3Bytes( fMag ) | ( static_cast< u_int >( usDir & 0xFF00 ) << 24 ), usDir && 0xFF };
    return xReturnValue;
}

u_char GLToy_Compress::UnitVector_Byte( const GLToy_Vector_3& xVector )
{
    u_char ucMin = 0;
    float fMin = GLToy_Maths::LargeFloat;
    for( u_char u = 0; u < ( sizeof( xBYTE_NORMALS ) / sizeof( GLToy_Vector_3 ) ); ++u )
    {
        const float fDiff = ( xVector - xBYTE_NORMALS[ u ] ).MagnitudeSquared();
        if( fDiff < fMin )
        {
            fMin = fDiff;
            ucMin = u;
        }
    }

    return ucMin;
}

u_short GLToy_Compress::UnitVector_2Bytes( const GLToy_Vector_3& xVector )
{
    const u_char ucX = static_cast< u_char >( GLToy_Maths::Floor( xVector[ 0 ] * 127.5f + 127.5f ) );
    const u_char ucY = static_cast< u_char >( GLToy_Maths::Floor( xVector[ 1 ] * 63.5f + 63.5f ) );
    return ( ( xVector[ 2 ] < 0.0f ) ? 1 : 0 ) | ( static_cast< u_short >( ucY ) << 1 ) | ( static_cast< u_short >( ucX ) << 8 );
}

u_short GLToy_Compress::OrthonormalMatrix_2Bytes( const GLToy_Matrix_3& xMatrix )
{
    return static_cast< u_short >( UnitVector_Byte( xMatrix[ 0 ] ) ) | ( static_cast< u_short >( UnitVector_Byte( xMatrix[ 1 ] ) ) << 8 );
}

u_int GLToy_Compress::OrthonormalMatrix_4Bytes( const GLToy_Matrix_3& xMatrix )
{
    return static_cast< u_int >( UnitVector_2Bytes( xMatrix[ 0 ] ) ) | ( static_cast< u_int >( UnitVector_2Bytes( xMatrix[ 1 ] ) ) << 16 );
}


float GLToy_Decompress::Float_3Bytes( const u_int uInt )
{
    // get the sign bit in position
    u_int uReturnValue = ( uInt << 8 ) & 0x80000000;

    // extract the exponent and re-scale it
    int iExponent = ( ( uInt >> 17 ) & 0x3F ) - 31;
    uReturnValue |= ( iExponent == 0 ) ? 0 : ( iExponent + 127 ) << 23;

    // extract the mantissa
    uReturnValue |= ( uInt & 0x1FFFF ) << 6;

    return *reinterpret_cast< float * >( &uReturnValue );
}

GLToy_Vector_3 GLToy_Decompress::Vector_5Bytes( const GLToy_5Bytes& u5Bytes )
{
   return UnitVector_2Bytes( u5Bytes.m_uByte | ( ( u5Bytes.m_u4Bytes >> 16 ) & 0xFF00 ) ) * Float_3Bytes( u5Bytes.m_u4Bytes & 0xFFFFFF );
}

GLToy_Vector_3 GLToy_Decompress::UnitVector_Byte( const u_char ucChar )
{
    return xBYTE_NORMALS[ ucChar ];
}

GLToy_Vector_3 GLToy_Decompress::UnitVector_2Bytes( const u_short usShort )
{
    GLToy_Vector_3 xReturnValue( static_cast< float >( usShort >> 8 ), static_cast< float >( ( usShort & 0xFF ) >> 1 ), ( usShort & 1 ) ? -1.0f : 1.0f );
    
    xReturnValue[ 0 ] *= 1.0f / 127.5f;
    xReturnValue[ 1 ] *= 1.0f / 63.5f;
    xReturnValue[ 0 ] -= 1.0f;
    xReturnValue[ 1 ] -= 1.0f;
    xReturnValue[ 2 ] *= GLToy_Maths::Sqrt( GLToy_Maths::Max( 1.0f - xReturnValue[ 0 ] * xReturnValue[ 0 ] - xReturnValue[ 1 ] * xReturnValue[ 1 ], 0.0f ) );

    xReturnValue.Normalise();
    
    return xReturnValue;
}

GLToy_Matrix_3 GLToy_Decompress::OrthonormalMatrix_2Bytes( const u_short usShort )
{
    const GLToy_Vector_3 xX = UnitVector_Byte( usShort & 0xFF );
    GLToy_Vector_3 xY = UnitVector_Byte( usShort >> 8 );
    GLToy_Vector_3 xZ = xX.Cross( xY );
    xZ.Normalise();
    xY = xZ.Cross( xX );
    return GLToy_Matrix_3( xX, xY, xZ );
}

GLToy_Matrix_3 GLToy_Decompress::OrthonormalMatrix_4Bytes( const u_int uInt )
{
    const GLToy_Vector_3 xX = UnitVector_2Bytes( uInt & 0xFFFF );
    GLToy_Vector_3 xY = UnitVector_2Bytes( uInt >> 16 );
    GLToy_Vector_3 xZ = xX.Cross( xY );
    xZ.Normalise();
    xY = xZ.Cross( xX );
    return GLToy_Matrix_3( xX, xY, xZ );
}
