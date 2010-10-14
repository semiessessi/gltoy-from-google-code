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

#ifndef __GLTOY_NOISE_H_
#define __GLTOY_NOISE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Noise
{

public:

    static float Linear1D( const float fX, const float fFrequency = 1.0f, const float fScale = 1.0f, const u_int uWrap = 0 );
    static float Cosine1D( const float fX, const float fFrequency = 1.0f, const float fScale = 1.0f, const u_int uWrap = 0 );
	static float Cubic1D( const float fX, const float fFrequency = 1.0f, const float fScale = 1.0f, const u_int uWrap = 0 );
	static float CatmullRom1D( const float fX, const float fFrequency = 1.0f, const float fScale = 1.0f, const u_int uWrap = 0 );
	static float Quintic1D( const float fX, const float fFrequency = 1.0f, const float fScale = 1.0f, const u_int uWrap = 0 );
    static float FractalCosine1D( const float fX, const float fFrequency = 1.0f, const float fScale = 1.0f, const float fFactor = 0.5f, const u_int uDepth = 4 );
	static float FractalCubic1D( const float fX, const float fFrequency = 1.0f, const float fScale = 1.0f, const float fFactor = 0.5f, const u_int uDepth = 4 );
	static float FractalCatmullRom1D( const float fX, const float fFrequency = 1.0f, const float fScale = 1.0f, const float fFactor = 0.5f, const u_int uDepth = 4 );

    static float Linear2D( const float fX, const float fY, const float fFrequency = 1.0f, const float fScale = 1.0f, const u_int uWrap = 0, const u_int uSeed = 0 );
    static float Cosine2D( const float fX, const float fY, const float fFrequency = 1.0f, const float fScale = 1.0f, const u_int uWrap = 0, const u_int uSeed = 0 );
	static float Cubic2D( const float fX, const float fY, const float fFrequency = 1.0f, const float fScale = 1.0f, const u_int uWrap = 0, const u_int uSeed = 0 );
	static float CatmullRom2D( const float fX, const float fY, const float fFrequency = 1.0f, const float fScale = 1.0f, const u_int uWrap = 0, const u_int uSeed = 0 );
	static float Quintic2D( const float fX, const float fY, const float fFrequency = 1.0f, const float fScale = 1.0f, const u_int uWrap = 0, const u_int uSeed = 0 );
    static float FractalCosine2D( const float fX, const float fY, const float fFrequency = 1.0f, const float fScale = 1.0f, const float fFactor = 0.5f, const u_int uDepth = 6 );
	static float FractalCubic2D( const float fX, const float fY, const float fFrequency = 1.0f, const float fScale = 1.0f, const float fFactor = 0.5f, const u_int uDepth = 6 );
	static float FractalCatmullRom2D( const float fX, const float fY, const float fFrequency = 1.0f, const float fScale = 1.0f, const float fFactor = 0.5f, const u_int uDepth = 6 );

private:

    static float PRNG( const u_int uSeed );
    static float PRNG( const u_int uSeed1, const u_int uSeed2 );

};

#endif
