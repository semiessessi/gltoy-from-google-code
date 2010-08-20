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
#include <Render/SuperScope/JD1_SuperScope_Dragon.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// M A C R O S
/////////////////////////////////////////////////////////////////////////////////////////////

#define cos GLToy_Maths::Cos
#define sin GLToy_Maths::Sin
#define sqrt GLToy_Maths::Sqrt
#define pow GLToy_Maths::Pow

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

static int targn;
static float asp;
static float crx;
static float srx;
static float cry;
static float sry;
static float crz;
static float srz;
static float dt;
static float lasttime;
static float rx;
static float rx2;
static float ry;
static float ry2;
static float rz;
static float drx;
static float dry;
static float drz;
static float tdrx;
static float tdry;
static float tdrz;
static float tscale;
static float scale;
static float xa;
static float ya;
static float xt;
static float tcmx;
static float tcmy;
static float cmx;
static float cmy;
static float sxa;
static float sya;
static float q;
static int fc;
static float x1;
static float y1;
static float z1;
static float x2;
static float y2;
static float z2;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void JD1_SuperScope_Dragon::Initialise()
{
    n=17;
    targn=17;
}

void JD1_SuperScope_Dragon::PerFrame()
{
    // Timer
    dt=gettime(lasttime);
    lasttime=gettime(0);

    // Rotation
    rx2=rx2+drx*dt;
    rx=2.5+.5*sin(rx2);
    ry2=ry2+dry*dt;
    ry=.2*cos(ry2);
    rz=rz+drz*dt;
    drx=drx*.95+.05*tdrx;
    dry=dry*.95+.05*tdry;
    drz=drz*.95+.05*tdrz;
    crz=cos(rz);srz=sin(rz);
    cry=cos(ry);sry=sin(ry);
    crx=cos(rx);srx=sin(rx);

    // Aspect ratio
    asp=h/w;

    // Local stuff
    tscale=sqrt(1.5/(n+1));
    scale=scale*.95+.05*tscale;
    xa=0;ya=0;
    xt=1.0f/n; // note: changed x to xt, 1 changed to 1.0f
    tcmx=sxa*xt; // note: changed x to xt
    tcmy=sya*xt; // note: changed x to xt
    cmx=cmx*.95+.05*tcmx;
    cmy=cmy*.95+.05*tcmy;
    sxa=0;
    sya=0;
    q=0;
    fc=(fc+1)%4;
    n=(fc?n:(below(n,targn)?n+1:(above(n,targn)?n-1:n))); // note if --> ? :
}

void JD1_SuperScope_Dragon::OnBeat()
{
    tdrx=(rand(11)-5)*.2;
    tdry=(rand(11)-5)*.2;
    tdrz=(rand(5)?tdrz:(rand(11)-5)*.2); // note if --> ? :
    targn=(rand(4)?targn:(equal(n,targn)?pow(2,rand(6)+2)+1:targn)); // note if --> ? :, 15 --> 4
}

void JD1_SuperScope_Dragon::PerPoint(
    const float i,
    const float v,
    float& x, float& y,
    float& red, float& green, float& blue,
    bool& skip,
    float& linesize ) const
{
    q=q+1;
    xa=xa+scale*(equal(q,2)-equal(q,4)-equal(q,6)-equal(q,8)-equal(q,10)+equal(q,12)-equal(q,14)-equal(q,16)-equal(q,18)+equal(q,20)+equal(q,22)+equal(q,24)-equal(q,26)+equal(q,28)-equal(q,30)-equal(q,32)-equal(q,34)+equal(q,36)+equal(q,38)+equal(q,40)+equal(q,42)-equal(q,44)+equal(q,46)+equal(q,48)-equal(q,50)+equal(q,52)+equal(q,54)+equal(q,56)-equal(q,58)+equal(q,60)-equal(q,62)-equal(q,64)+equal(q,66)-equal(q,68)-equal(q,70)-equal(q,72)-equal(q,74)+equal(q,76)-equal(q,78)-equal(q,80)-equal(q,82)+equal(q,84)+equal(q,86)+equal(q,88)-equal(q,90)+equal(q,92)-equal(q,94)-equal(q,96)+equal(q,98)-equal(q,100)-equal(q,102)-equal(q,104)-equal(q,106)+equal(q,108)-equal(q,110)-equal(q,112)+equal(q,114)-equal(q,116)-equal(q,118)-equal(q,120)+equal(q,122)-equal(q,124)+equal(q,126)+equal(q,128));
    ya=ya+scale*(equal(q,3)+equal(q,5)-equal(q,7)+equal(q,9)-equal(q,11)-equal(q,13)-equal(q,15)+equal(q,17)-equal(q,19)-equal(q,21)+equal(q,23)-equal(q,25)-equal(q,27)-equal(q,29)-equal(q,31)+equal(q,33)-equal(q,35)-equal(q,37)+equal(q,39)-equal(q,41)+equal(q,43)+equal(q,45)+equal(q,47)-equal(q,49)-equal(q,51)-equal(q,53)+equal(q,55)-equal(q,57)-equal(q,59)-equal(q,61)-equal(q,63)+equal(q,65)+equal(q,67)+equal(q,69)-equal(q,71)+equal(q,73)-equal(q,75)-equal(q,77)-equal(q,79)+equal(q,81)-equal(q,83)-equal(q,85)+equal(q,87)-equal(q,89)-equal(q,91)-equal(q,93)-equal(q,95)+equal(q,97)+equal(q,99)+equal(q,101)-equal(q,103)+equal(q,105)-equal(q,107)-equal(q,109)-equal(q,111)+equal(q,113)+equal(q,115)+equal(q,117)-equal(q,119)+equal(q,121)+equal(q,123)+equal(q,125)+equal(q,127)-equal(q,129));
    sxa=sxa+xa;
    sya=sya+ya;
    x1=xa-cmx;
    y1=ya-cmy;
    z1=0;

    x2=x1*crz-y1*srz;
    y2=x1*srz+y1*crz;
    x1=x2*cry+z1*sry;
    z2=-x2*sry+z1*cry;
    y1=y2*crx-z2*srx;
    z1=y2*srx+z2*crx;
    z1=1/(1+z1*0.5);
    x=x1*z1;
    y=y1*z1;
    x=x*asp;
}

#undef sin
#undef cos
#undef sqrt
#undef pow