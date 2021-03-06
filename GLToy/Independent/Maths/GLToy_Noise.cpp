/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2009, 2010 Semi Essessi
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
#include <Maths/GLToy_Noise.h>

// GLToy
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

float GLToy_Noise::Linear1D( const float fX, const float fFrequency, const float fScale, const u_int uWrap )
{
    const float fT = fX * fFrequency;
    u_int u1 = static_cast< u_int >( GLToy_Maths::Floor( fT ) );
    u_int u2 = static_cast< u_int >( GLToy_Maths::Ceiling( fT ) );
    
    const float fU1 = static_cast< float >( u1 );
	
	if( uWrap )
    {
        u1 = GLToy_Maths::Wrap( u1, 0u, uWrap );
        u2 = GLToy_Maths::Wrap( u2, 0u, uWrap );
    }
    
    return fScale * GLToy_Maths::Lerp( PRNG( u1 ), PRNG( u2 ), fT - fU1 );
}

float GLToy_Noise::Cosine1D( const float fX, const float fFrequency, const float fScale, const u_int uWrap )
{
    const float fT = fX * fFrequency;
    u_int u1 = static_cast< u_int >( GLToy_Maths::Floor( fT ) );
    u_int u2 = static_cast< u_int >( GLToy_Maths::Ceiling( fT ) );

	const float fU1 = static_cast< float >( u1 );

    if( uWrap )
    {
        u1 = GLToy_Maths::Wrap( u1, 0u, uWrap );
        u2 = GLToy_Maths::Wrap( u2, 0u, uWrap );
    }

    return fScale * GLToy_Maths::CosineInterpolate( PRNG( u1 ), PRNG( u2 ), fT - fU1 );
}

float GLToy_Noise::Cubic1D( const float fX, const float fFrequency, const float fScale, const u_int uWrap )
{
    const float fT = fX * fFrequency;
    u_int u1 = static_cast< u_int >( GLToy_Maths::Floor( fT ) ) - 1;
    u_int u2 = u1 + 1;
	u_int u3 = u2 + 1;
    u_int u4 = u3 + 1;

	const float fU2 = static_cast< float >( u2 );

    if( uWrap )
    {
        u1 = GLToy_Maths::Wrap( u1, 0u, uWrap );
        u2 = GLToy_Maths::Wrap( u2, 0u, uWrap );
        u3 = GLToy_Maths::Wrap( u3, 0u, uWrap );
        u4 = GLToy_Maths::Wrap( u4, 0u, uWrap );
    }

    return fScale * GLToy_Maths::CubicInterpolate( PRNG( u1 ), PRNG( u2 ), PRNG( u3 ), PRNG( u4 ), fT - fU2 );
}

float GLToy_Noise::CatmullRom1D( const float fX, const float fFrequency, const float fScale, const u_int uWrap )
{
    const float fT = fX * fFrequency;
    u_int u1 = static_cast< u_int >( GLToy_Maths::Floor( fT ) ) - 1;
    u_int u2 = u1 + 1;
	u_int u3 = u2 + 1;
    u_int u4 = u3 + 1;

	const float fU2 = static_cast< float >( u2 );

    if( uWrap )
    {
        u1 = GLToy_Maths::Wrap( u1, 0u, uWrap );
        u2 = GLToy_Maths::Wrap( u2, 0u, uWrap );
        u3 = GLToy_Maths::Wrap( u3, 0u, uWrap );
        u4 = GLToy_Maths::Wrap( u4, 0u, uWrap );
    }

    return fScale * GLToy_Maths::CatmullRomInterpolate( PRNG( u1 ), PRNG( u2 ), PRNG( u3 ), PRNG( u4 ), fT - fU2 );
}

float GLToy_Noise::Quintic1D( const float fX, const float fFrequency, const float fScale, const u_int uWrap )
{
    const float fT = fX * fFrequency;
    u_int u1 = static_cast< u_int >( GLToy_Maths::Floor( fT ) );
    u_int u2 = static_cast< u_int >( GLToy_Maths::Ceiling( fT ) );
    
    const float fU1 = static_cast< float >( u1 );
	
	if( uWrap )
    {
        u1 = GLToy_Maths::Wrap( u1, 0u, uWrap );
        u2 = GLToy_Maths::Wrap( u2, 0u, uWrap );
    }
    
    return fScale * GLToy_Maths::QuinticNoiseInterpolate( PRNG( u1 ), PRNG( u2 ), fT - fU1 );
}

float GLToy_Noise::FractalCosine1D( const float fX, const float fFrequency, const float fScale, const float fFactor, const u_int uDepth, const bool bTileable, const u_int uSeed )
{
    float fValue = 0.0f;
    const float fFrequencyChange = 1.0f / fFactor;
    float fFractalFrequency = fFrequency;
    float fU = fX;
    float fFractalScale = 0.5f;
    
    for( u_int u = 0; u < uDepth; ++u )
    {
        fValue += fFractalScale * ( fValue + Cosine1D( fU, fFractalFrequency, fScale, bTileable ? static_cast< u_int >( fFractalFrequency ) : 0 ) );

        fFractalFrequency *= fFrequencyChange;
        fFractalScale *= 0.5f;
    }

    return GLToy_Maths::Clamp( fValue, -fScale, fScale );
}

float GLToy_Noise::FractalCubic1D( const float fX, const float fFrequency, const float fScale, const float fFactor, const u_int uDepth, const bool bTileable, const u_int uSeed )
{
    float fValue = 0.0f;
    const float fFrequencyChange = 1.0f / fFactor;
    float fFractalFrequency = fFrequency;
    float fU = fX;
    float fFractalScale = 0.5f;
    
    for( u_int u = 0; u < uDepth; ++u )
    {
        fValue += fFractalScale * ( fValue + Cubic1D( fU, fFractalFrequency, fScale, bTileable ? static_cast< u_int >( fFractalFrequency ) : 0 ) );

        fFractalFrequency *= fFrequencyChange;
        fFractalScale *= 0.5f;
    }

    return GLToy_Maths::Clamp( fValue, -fScale, fScale );
}

float GLToy_Noise::FractalCatmullRom1D( const float fX, const float fFrequency, const float fScale, const float fFactor, const u_int uDepth, const bool bTileable, const u_int uSeed )
{
    float fValue = 0.0f;
    const float fFrequencyChange = 1.0f / fFactor;
    float fFractalFrequency = fFrequency;
    float fU = fX;
    float fFractalScale = 0.5f;
    
    for( u_int u = 0; u < uDepth; ++u )
    {
        fValue += fFractalScale * ( fValue + CatmullRom1D( fU, fFractalFrequency, fScale, bTileable ? static_cast< u_int >( fFractalFrequency ) : 0 ) );

        fFractalFrequency *= fFrequencyChange;
        fFractalScale *= 0.5f;
    }

    return GLToy_Maths::Clamp( fValue, -fScale, fScale );
}

float GLToy_Noise::Linear2D( const float fX, const float fY, const float fFrequency, const float fScale, const u_int uWrap, const u_int uSeed )
{
    const float fT = fX * fFrequency;
    u_int u1 = static_cast< u_int >( GLToy_Maths::Floor( fT ) );
    u_int u2 = u1 + 1;
    
    const float fS = fY * fFrequency;
    u_int v1 = static_cast< u_int >( GLToy_Maths::Floor( fS ) );
    u_int v2 = v1 + 1;
	
	const float fU1 = static_cast< float >( u1 );
	const float fV1 = static_cast< float >( v1 );

    if( uWrap )
    {
        u1 = GLToy_Maths::Wrap( u1, 0u, uWrap );
        u2 = GLToy_Maths::Wrap( u2, 0u, uWrap );
        v1 = GLToy_Maths::Wrap( v1, 0u, uWrap );
        v2 = GLToy_Maths::Wrap( v2, 0u, uWrap );
    }

    const float fX1 = GLToy_Maths::Lerp( PRNG( u1 ^ uSeed, v1 ^ uSeed ), PRNG( u2 ^ uSeed, v1 ^ uSeed ), fT - fU1 );
    const float fX2 = GLToy_Maths::Lerp( PRNG( u1 ^ uSeed, v2 ^ uSeed ), PRNG( u2 ^ uSeed, v2 ^ uSeed ), fT - fU1 );

    return fScale * GLToy_Maths::Lerp( fX1, fX2, fS - fV1 );
}

float GLToy_Noise::Cosine2D( const float fX, const float fY, const float fFrequency, const float fScale, const u_int uWrap, const u_int uSeed )
{
    const float fT = fX * fFrequency;
    u_int u1 = static_cast< u_int >( GLToy_Maths::Floor( fT ) );
    u_int u2 = u1 + 1;
    
    const float fS = fY * fFrequency;
    u_int v1 = static_cast< u_int >( GLToy_Maths::Floor( fS ) );
    u_int v2 = v1 + 1;

	const float fU1 = static_cast< float >( u1 );
	const float fV1 = static_cast< float >( v1 );

    if( uWrap )
    {
        u1 = GLToy_Maths::Wrap( u1, 0u, uWrap );
        u2 = GLToy_Maths::Wrap( u2, 0u, uWrap );
        v1 = GLToy_Maths::Wrap( v1, 0u, uWrap );
        v2 = GLToy_Maths::Wrap( v2, 0u, uWrap );
    }

    const float fX1 = GLToy_Maths::CosineInterpolate( PRNG( u1 ^ uSeed, v1 ^ uSeed ), PRNG( u2 ^ uSeed, v1 ^ uSeed ), fT - fU1 );
    const float fX2 = GLToy_Maths::CosineInterpolate( PRNG( u1 ^ uSeed, v2 ^ uSeed ), PRNG( u2 ^ uSeed, v2 ^ uSeed ), fT - fU1 );

    return fScale * GLToy_Maths::CosineInterpolate( fX1, fX2, fS - fV1 );
}

float GLToy_Noise::Cubic2D( const float fX, const float fY, const float fFrequency, const float fScale, const u_int uWrap, const u_int uSeed )
{
    const float fT = fX * fFrequency;
    u_int u1 = static_cast< u_int >( GLToy_Maths::Floor( fT ) ) - 1;
    u_int u2 = u1 + 1;
	u_int u3 = u2 + 1;
    u_int u4 = u3 + 1;
    
    const float fS = fY * fFrequency;
    u_int v1 = static_cast< u_int >( GLToy_Maths::Floor( fS ) ) - 1;
    u_int v2 = v1 + 1;
	u_int v3 = v2 + 1;
    u_int v4 = v3 + 1;

	const float fU2 = static_cast< float >( u2 );
	const float fV2 = static_cast< float >( v2 );

    if( uWrap )
    {
        u1 = GLToy_Maths::Wrap( u1, 0u, uWrap );
        u2 = GLToy_Maths::Wrap( u2, 0u, uWrap );
        u3 = GLToy_Maths::Wrap( u3, 0u, uWrap );
        u4 = GLToy_Maths::Wrap( u4, 0u, uWrap );
        v1 = GLToy_Maths::Wrap( v1, 0u, uWrap );
        v2 = GLToy_Maths::Wrap( v2, 0u, uWrap );
        v3 = GLToy_Maths::Wrap( v3, 0u, uWrap );
        v4 = GLToy_Maths::Wrap( v4, 0u, uWrap );
    }

    const float fX1 = GLToy_Maths::CubicInterpolate( PRNG( u1 ^ uSeed, v1 ^ uSeed ), PRNG( u2 ^ uSeed, v1 ^ uSeed ), PRNG( u3 ^ uSeed, v1 ^ uSeed ), PRNG( u4 ^ uSeed, v1 ^ uSeed ), fT - fU2 );
    const float fX2 = GLToy_Maths::CubicInterpolate( PRNG( u1 ^ uSeed, v2 ^ uSeed ), PRNG( u2 ^ uSeed, v2 ^ uSeed ), PRNG( u3 ^ uSeed, v2 ^ uSeed ), PRNG( u4 ^ uSeed, v2 ^ uSeed ), fT - fU2 );
	const float fX3 = GLToy_Maths::CubicInterpolate( PRNG( u1 ^ uSeed, v3 ^ uSeed ), PRNG( u2 ^ uSeed, v3 ^ uSeed ), PRNG( u3 ^ uSeed, v3 ^ uSeed ), PRNG( u4 ^ uSeed, v3 ^ uSeed ), fT - fU2 );
	const float fX4 = GLToy_Maths::CubicInterpolate( PRNG( u1 ^ uSeed, v4 ^ uSeed ), PRNG( u2 ^ uSeed, v4 ^ uSeed ), PRNG( u3 ^ uSeed, v4 ^ uSeed ), PRNG( u4 ^ uSeed, v4 ^ uSeed ), fT - fU2 );

    return fScale * GLToy_Maths::CubicInterpolate( fX1, fX2, fX3, fX4, fS - fV2 );
}

float GLToy_Noise::CatmullRom2D( const float fX, const float fY, const float fFrequency, const float fScale, const u_int uWrap, const u_int uSeed )
{
    const float fT = fX * fFrequency;
    u_int u1 = static_cast< u_int >( GLToy_Maths::Floor( fT ) ) - 1;
    u_int u2 = u1 + 1;
	u_int u3 = u2 + 1;
    u_int u4 = u3 + 1;
    
    const float fS = fY * fFrequency;
    u_int v1 = static_cast< u_int >( GLToy_Maths::Floor( fS ) ) - 1;
    u_int v2 = v1 + 1;
	u_int v3 = v2 + 1;
    u_int v4 = v3 + 1;

	const float fU2 = static_cast< float >( u2 );
	const float fV2 = static_cast< float >( v2 );

    if( uWrap )
    {
        u1 = GLToy_Maths::Wrap( u1, 0u, uWrap );
        u2 = GLToy_Maths::Wrap( u2, 0u, uWrap );
        u3 = GLToy_Maths::Wrap( u3, 0u, uWrap );
        u4 = GLToy_Maths::Wrap( u4, 0u, uWrap );
        v1 = GLToy_Maths::Wrap( v1, 0u, uWrap );
        v2 = GLToy_Maths::Wrap( v2, 0u, uWrap );
        v3 = GLToy_Maths::Wrap( v3, 0u, uWrap );
        v4 = GLToy_Maths::Wrap( v4, 0u, uWrap );
    }

    const float fX1 = GLToy_Maths::CatmullRomInterpolate( PRNG( u1 ^ uSeed, v1 ^ uSeed ), PRNG( u2 ^ uSeed, v1 ^ uSeed ), PRNG( u3 ^ uSeed, v1 ^ uSeed ), PRNG( u4 ^ uSeed, v1 ^ uSeed ), fT - fU2 );
    const float fX2 = GLToy_Maths::CatmullRomInterpolate( PRNG( u1 ^ uSeed, v2 ^ uSeed ), PRNG( u2 ^ uSeed, v2 ^ uSeed ), PRNG( u3 ^ uSeed, v2 ^ uSeed ), PRNG( u4 ^ uSeed, v2 ^ uSeed ), fT - fU2 );
	const float fX3 = GLToy_Maths::CatmullRomInterpolate( PRNG( u1 ^ uSeed, v3 ^ uSeed ), PRNG( u2 ^ uSeed, v3 ^ uSeed ), PRNG( u3 ^ uSeed, v3 ^ uSeed ), PRNG( u4 ^ uSeed, v3 ^ uSeed ), fT - fU2 );
	const float fX4 = GLToy_Maths::CatmullRomInterpolate( PRNG( u1 ^ uSeed, v4 ^ uSeed ), PRNG( u2 ^ uSeed, v4 ^ uSeed ), PRNG( u3 ^ uSeed, v4 ^ uSeed ), PRNG( u4 ^ uSeed, v4 ^ uSeed ), fT - fU2 );

    return fScale * GLToy_Maths::CatmullRomInterpolate( fX1, fX2, fX3, fX4, fS - fV2 );
}

float GLToy_Noise::Quintic2D( const float fX, const float fY, const float fFrequency, const float fScale, const u_int uWrap, const u_int uSeed )
{
    const float fT = fX * fFrequency;
    u_int u1 = static_cast< u_int >( GLToy_Maths::Floor( fT ) );
    u_int u2 = u1 + 1;
    
    const float fS = fY * fFrequency;
    u_int v1 = static_cast< u_int >( GLToy_Maths::Floor( fS ) );
    u_int v2 = v1 + 1;

	const float fU1 = static_cast< float >( u1 );
	const float fV1 = static_cast< float >( v1 );

    if( uWrap )
    {
        u1 = GLToy_Maths::Wrap( u1, 0u, uWrap );
        u2 = GLToy_Maths::Wrap( u2, 0u, uWrap );
        v1 = GLToy_Maths::Wrap( v1, 0u, uWrap );
        v2 = GLToy_Maths::Wrap( v2, 0u, uWrap );
    }

    const float fX1 = GLToy_Maths::QuinticNoiseInterpolate( PRNG( u1 ^ uSeed, v1 ^ uSeed ), PRNG( u2 ^ uSeed, v1 ^ uSeed ), fT - fU1 );
    const float fX2 = GLToy_Maths::QuinticNoiseInterpolate( PRNG( u1 ^ uSeed, v2 ^ uSeed ), PRNG( u2 ^ uSeed, v2 ^ uSeed ), fT - fU1 );

    return fScale * GLToy_Maths::QuinticNoiseInterpolate( fX1, fX2, fS - fV1 );
}

float GLToy_Noise::FractalCosine2D( const float fX, const float fY, const float fFrequency, const float fScale, const float fFactor, const u_int uDepth, const bool bTileable, const u_int uSeed )
{
    float fValue = 0.0f;
    const float fFrequencyChange = 1.0f / fFactor;
    float fFractalFrequency = fFrequency;
    float fU = fX;
    float fV = fY;
    float fFractalScale = 0.5f;
    
    for( u_int u = 0; u < uDepth; ++u )
    {
        fValue += fFractalScale * ( fValue + Cosine2D( fU, fV, fFractalFrequency, fScale, bTileable ? static_cast< u_int >( fFractalFrequency ) : 0, uSeed ) );

        fFractalFrequency *= fFrequencyChange;
        fFractalScale *= 0.5f;
    }

    return GLToy_Maths::Clamp( fValue, -fScale, fScale );
}

float GLToy_Noise::FractalCubic2D( const float fX, const float fY, const float fFrequency, const float fScale, const float fFactor, const u_int uDepth, const bool bTileable, const u_int uSeed )
{
    float fValue = 0.0f;
    const float fFrequencyChange = 1.0f / fFactor;
    float fFractalFrequency = fFrequency;
    float fU = fX;
    float fV = fY;
    float fFractalScale = 0.5f;
    
    for( u_int u = 0; u < uDepth; ++u )
    {
        fValue += fFractalScale * ( fValue + Cubic2D( fU, fV, fFractalFrequency, fScale, bTileable ? static_cast< u_int >( fFractalFrequency ) : 0, uSeed ) );

        fFractalFrequency *= fFrequencyChange;
        fFractalScale *= 0.5f;
    }

    return GLToy_Maths::Clamp( fValue, -fScale, fScale );
}

float GLToy_Noise::FractalCatmullRom2D( const float fX, const float fY, const float fFrequency, const float fScale, const float fFactor, const u_int uDepth, const bool bTileable, const u_int uSeed )
{
    float fValue = 0.0f;
    const float fFrequencyChange = 1.0f / fFactor;
    float fFractalFrequency = fFrequency;
    float fU = fX;
    float fV = fY;
    float fFractalScale = 0.5f;
    
    for( u_int u = 0; u < uDepth; ++u )
    {
        fValue += fFractalScale * ( fValue + CatmullRom2D( fU, fV, fFractalFrequency, fScale, bTileable ? static_cast< u_int >( fFractalFrequency ) : 0, uSeed ) );

        fFractalFrequency *= fFrequencyChange;
        fFractalScale *= 0.5f;
    }

    return GLToy_Maths::Clamp( fValue, -fScale, fScale );
}

float GLToy_Noise::PRNG( const u_int uSeed )
{
    u_int u = uSeed;
    u ^= u << 24;
    u -= ~( u << 6 );
    return 1.0f - static_cast< float >( u * 16807 + 2147483647 ) / static_cast< float >( 0x80000000 );
}

float GLToy_Noise::PRNG( const u_int uSeed1, const u_int uSeed2 )
{
    u_int u = uSeed1;
    u ^= ~( uSeed2 << 7 );
    u -= ( u << 23 ) ^ ( uSeed2 * 1103515245 + 12345 );
    return PRNG( u );
}
