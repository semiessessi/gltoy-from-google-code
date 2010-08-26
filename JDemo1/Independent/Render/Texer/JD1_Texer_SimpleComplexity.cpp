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

#include <Core/JD1.h>

// This file's header
#include <Render/Texer/JD1_Texer_SimpleComplexity.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// M A C R O S
/////////////////////////////////////////////////////////////////////////////////////////////

#define acos GLToy_Maths::ACos
#define cos GLToy_Maths::Cos
#define sin GLToy_Maths::Sin
#define sqrt GLToy_Maths::Sqrt
#define pow GLToy_Maths::Pow
#define max GLToy_Maths::Max
#define abs GLToy_Maths::Abs

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

static float tpi, lasttime;
static float ddt, o1, o2, o3, o4;
static int q = 0;
static int a;
static float rx, drx, ry, dry, rz, ts, t1, crz, srz, cry, sry, crx, srx, asp, cf1, cft1, cf2, cft2;
static float fac1, nfac1, fac2, nfac2, fac3, nfac3, sz;
static float vx, u, d, sv, x1, y1, z1, x2, y2, z2, alpha;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

// note, i changed a lot of constants to be floats with f suffixed

void JD1_Texer_SimpleComplexity::Initialise()
{
    //n=512;
    n = 1024; // note: change
    tpi=2.0f*acos(-1.0f);lasttime=gettime(0.0f);
}

void JD1_Texer_SimpleComplexity::PerFrame()
{
    ddt=gettime(lasttime);lasttime=gettime(0.0f);
    o1=pow(.9f,ddt*30.0f);o2=1.0f-o1;
    o3=pow(.95f,ddt*30.0f);o4=1.0f-o3;
    q=q+1;
    rx=rx+drx*ddt;ry=ry+dry*ddt;rz=rz+ddt*getspec(0.1f,0.1f,0f);
    ts=o1*ts+o2*getspec(0.2f,0.1f,0f);
    drx=drx*o3;
    dry=dry*o3;
    t1=t1+ddt*ts;
    crz=cos(rz);srz=sin(rz);cry=cos(ry);sry=sin(ry);crx=cos(rx);srx=sin(rx);asp=h/w;
    cf1=o1*cf1+o2*cft1;cf2=o1*cf2+o2*cft2;
    fac1=o3*fac1+o4*nfac1;
    fac2=o3*fac2+o4*nfac2;
    fac3=o3*fac3+o4*nfac3;
    sz=0.002f*w;
    a=0;
}

void JD1_Texer_SimpleComplexity::OnBeat()
{
    drx=(rand(4)?drx:8.0f*getosc(0.2f,0.2f,0f)); // note: if --> ? :
    dry=4.0f*getspec(0.2f,0.2f,0f);
    cft1 = static_cast< float >( ( 31469 * q + 1 ) & 1 ); // note: completely the same, but all tidy
    cft2 = static_cast< float >( ( 31081 * q + 1 ) & 1 ); // note: completely the same, but all tidy
    nfac1=(rand(3)?nfac1:rand(2)); // note: if --> ? :
    nfac2=(rand(3)?nfac2:rand(2)); // note: if --> ? :
    nfac3=(rand(3)?nfac3:rand(2)); // note: if --> ? :
}

void JD1_Texer_SimpleComplexity::PerPoint(
    const float i,
    const float v,
    float& x, float& y, float& z,
    float& red, float& green, float& blue,
    bool& skip,
    float& sizex,
    float& sizey ) const
{
    vx=tpi*i; // note: vx --> vx
    u=160*vx+(.1f*t1)*(a%17);

    d=0.5f*((1.0f+0.2f*cos(4*u))*fac1+cos(2.0f*u)*fac2+sin(2.0f*vx)*fac3)+0.6f;

    sv=sin(vx);

    x1=d*sin(u)*sv;
    y1=d*sv*cos(u);
    z1=d*cos(vx);

    x2=x1*crz-y1*srz;
    y2=x1*srz+y1*crz;

    x1=x2*cry+z1*sry;
    z2=-x2*sry+z1*cry;

    y1=y2*crx-z2*srx;
    z1=y2*srx+z2*crx;

    alpha=max(-z1*0.5f+0.5f,0.1f); // note: removed ambiguities

    //z1=1/(2+z1*0.5);

    //x=x1*z1;
    //y=y1*z1;

    //x=x*asp;

    // note: replaces the above
    x = x1; y = y1; z = z1 + 2;

    red=max((cf1+cf2)*0.5f*alpha,0.1f); // note: removed ambiguities
    blue=max((1-cf1)*0.5f*alpha,0.1f); // note: removed ambiguities
    green=max(cf2*0.5f*alpha,0.1f); // note: removed ambiguities

    // note: addition
    red *= 2.0f;
    blue *= 2.0f;
    green *= 2.0f;

    //sizex=sz*z1;
    //sizey=sizex;

    // note: replaces the above
    sizex = 0.02f;
    sizey = 0.02f;

    //assign(gmegabuf(a),x);
    //assign(gmegabuf(a+1),y);
    //assign(gmegabuf(a+2),sizex);
    //assign(gmegabuf(a+3),red);
    //assign(gmegabuf(a+4),green);
    //assign(gmegabuf(a+5),blue);
    a=a+6;
}
