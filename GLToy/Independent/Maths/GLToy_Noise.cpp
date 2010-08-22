/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009, 2010 Semi Essessi
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

float GLToy_Noise::Linear1D( const float fX, const float fFrequency, const float fScale )
{
    const float fT = fX * fFrequency;
    const u_int u1 = static_cast< u_int >( GLToy_Maths::Floor( fT ) );
    const u_int u2 = static_cast< u_int >( GLToy_Maths::Ceiling( fT ) );
    return fScale * GLToy_Maths::Lerp( PRNG( u1 ), PRNG( u2 ), fT - static_cast< float >( u1 ) );
}

float GLToy_Noise::Cosine1D( const float fX, const float fFrequency, const float fScale )
{
    const float fT = fX * fFrequency;
    const u_int u1 = static_cast< u_int >( GLToy_Maths::Floor( fT ) );
    const u_int u2 = static_cast< u_int >( GLToy_Maths::Ceiling( fT ) );
    return fScale * GLToy_Maths::CosineInterpolate( PRNG( u1 ), PRNG( u2 ), fT - static_cast< float >( u1 ) );
}

float GLToy_Noise::Linear2D( const float fX, const float fY, const float fFrequency, const float fScale )
{
    const float fT = fX * fFrequency;
    const u_int u1 = static_cast< u_int >( GLToy_Maths::Floor( fT ) );
    const u_int u2 = static_cast< u_int >( GLToy_Maths::Ceiling( fT ) );
    
    const float fS = fY * fFrequency;
    const u_int v1 = static_cast< u_int >( GLToy_Maths::Floor( fS ) );
    const u_int v2 = static_cast< u_int >( GLToy_Maths::Ceiling( fS ) );

    const float fX1 = GLToy_Maths::Lerp( PRNG( u1, v1 ), PRNG( u2, v1 ), fT - static_cast< float >( u1 ) );
    const float fX2 = GLToy_Maths::Lerp( PRNG( u1, v2 ), PRNG( u2, v2 ), fT - static_cast< float >( u1 ) );

    return fScale * GLToy_Maths::Lerp( fX1, fX2, fS - static_cast< float >( v1 ) );
}

float GLToy_Noise::Cosine2D( const float fX, const float fY, const float fFrequency, const float fScale )
{
    const float fT = fX * fFrequency;
    const u_int u1 = static_cast< u_int >( GLToy_Maths::Floor( fT ) );
    const u_int u2 = static_cast< u_int >( GLToy_Maths::Ceiling( fT ) );
    
    const float fS = fY * fFrequency;
    const u_int v1 = static_cast< u_int >( GLToy_Maths::Floor( fS ) );
    const u_int v2 = static_cast< u_int >( GLToy_Maths::Ceiling( fS ) );

    const float fX1 = GLToy_Maths::CosineInterpolate( PRNG( u1, v1 ), PRNG( u2, v1 ), fT - static_cast< float >( u1 ) );
    const float fX2 = GLToy_Maths::CosineInterpolate( PRNG( u1, v2 ), PRNG( u2, v2 ), fT - static_cast< float >( u1 ) );

    return fScale * GLToy_Maths::CosineInterpolate( fX1, fX2, fS - static_cast< float >( v1 ) );
}

float GLToy_Noise::Fractal2D( const float fX, const float fY, const float fFrequency, const float fScale, const float fFactor, const u_int uDepth )
{
    float fValue = 0.0f;
    const float fFrequencyChange = 1.0f / fFactor;
    float fFractalFrequency = fFrequency;
    float fU = fX;
    float fV = fY;
    float fFractalScale = 0.5f;
    
    for( u_int u = 0; u < uDepth; ++u )
    {
        fValue += fFractalScale * ( fValue + Cosine2D( fU, fV, fFractalFrequency, fScale ) );

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
