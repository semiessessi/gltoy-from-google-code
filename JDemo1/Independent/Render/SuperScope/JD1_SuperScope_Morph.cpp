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
#include <Render/SuperScope/JD1_SuperScope_Morph.h>

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
#define max GLToy_Maths::Max
#define abs GLToy_Maths::Abs

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

static float root2, oos, drx, f1, f2, f3, f;
static float rx, ry, rz, dry, crz, srz, cry, sry, crx, srx, t1, t2, t3, t4;
static float react, u, halfu, vx, v2, cu, chu, su, shu, cv, sv, x1, y1, z1, x2, y2, z2, x3, y3, z3, cr;
static float alpha, r1, g1, b1, opt1, r2, g2, b2, r3, g3, b3;
static int q;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void JD1_SuperScope_Morph::Initialise()
{
    n=400;root2=sqrt(2);oos=1/7;drx=0.02;f1=rand(2);f2=rand(2);f3=rand(2);f=rand(2);
}

void JD1_SuperScope_Morph::PerFrame()
{
    // note: made frame rate independent
    rx=rx+ ( drx ) * GLToy_Timer::GetFrameTime();
    ry=ry+ ( dry+0.1*getspec(0.2,0.2,0) ) * GLToy_Timer::GetFrameTime();
    rz=rz+ ( 0.2*getspec(0.1,0.1,0) ) * GLToy_Timer::GetFrameTime();
    crz=cos(rz);srz=sin(rz);cry=cos(ry);sry=sin(ry);crx=cos(rx);srx=sin(rx);q=0;f1=0.9*f1+0.1*t1;f2=0.9*f2+0.1*t2;f3=0.9*f3+0.1*t3;f=0.9*f+0.1*t4;
}

void JD1_SuperScope_Morph::OnBeat()
{
    drx=getspec(0.2,0.2,0)*0.3;dry=(rand(50)-25)/500;t1=rand(2);t2=rand(2);t3=rand(2);t4=rand(2);
}

void JD1_SuperScope_Morph::PerPoint(
    const float i,
    const float v,
    float& x, float& y,
    float& red, float& green, float& blue,
    bool& skip,
    float& linesize ) const
{
    q=q+1;
    react=v;
    u=170*i;
    halfu=0.5*u;
    // note: v --> vx
    vx=7*i;
    v2=0.6*i-0.3;

    cu=cos(u);
    chu=cos(halfu);
    cv=cos(vx);
    su=sin(u);
    shu=sin(halfu);
    sv=sin(vx);

    x1=cu*chu*(root2+cv)+(shu*sv*cv);
    y1=su*chu*(root2+cv)+(shu*sv*cv);
    z1=-shu*(root2+cv)+chu*sv*cv;

    x1=x1*f+(1-f)*cu*sv*2;
    y1=y1*f+(1-f)*cu*cv*2;
    z1=z1*f+(1-f)*su*2;

    x2=1.5*(cu+v2*chu*cu);
    y2=1.5*(su+v2*chu*su);
    z2=1.5*v2*shu;

    x2=x2*f+(1-f)*su*cv*1.5;
    y2=y2*f+(1-f)*su*sv*1.5;
    z2=z2*f+(1-f)*su*1.5;

    x1=f1*x1+(1-f1)*x2;
    y1=f1*y1+(1-f1)*y2;
    z1=f1*z1+(1-f1)*z2;

    x3=2*cu*cu*cu*cv*cv*cv;
    y3=2*su*su*su*cv*cv*cv;
    z3=2*sv*sv*sv;

    x3=f*x3+(1-f)*2*cos(2|static_cast<int>(u))*sv; // note: cast
    y3=f*y3+(1-f)*2*cos(3|static_cast<int>(u))*cv; // note: cast
    z3=f*z3+(1-f)*1.5*su;

    x2=equal(q%16,1)+equal(q%16,2)-equal(q%16,3)-equal(q%16,4)+equal(q%16,5)+equal(q%16,6)+equal(q%16,7)+equal(q%16,8)+equal(q%16,9)-equal(q%16,10)-equal(q%16,11)-equal(q%16,12)-equal(q%16,13)-equal(q%16,14)-equal(q%16,15)+equal(q%16,0);
    y2=equal(q%16,1)-equal(q%16,2)-equal(q%16,3)+equal(q%16,4)+equal(q%16,5)+equal(q%16,6)-equal(q%16,7)-equal(q%16,8)-equal(q%16,9)-equal(q%16,10)-equal(q%16,11)-equal(q%16,12)+equal(q%16,13)+equal(q%16,14)+equal(q%16,15)+equal(q%16,0);
    z2=equal(q%16,1)+equal(q%16,2)+equal(q%16,3)+equal(q%16,4)+equal(q%16,5)-equal(q%16,6)-equal(q%16,7)+equal(q%16,8)-equal(q%16,9)-equal(q%16,10)+equal(q%16,11)-equal(q%16,12)-equal(q%16,13)+equal(q%16,14)-equal(q%16,15)-equal(q%16,0);

    x2=x2*f+(1-f)*(sv*cu+sv);
    y2=y2*f+(1-f)*(cv*cu+cv);
    z2=z2*f+(1-f)*2*chu*shu;

    x2=f2*x3+(1-f2)*x2;
    y2=f2*y3+(1-f2)*y2;
    z2=f2*z3+(1-f2)*z2;

    x1=f3*x1+(1-f3)*x2;
    y1=f3*y1+(1-f3)*y2;
    z1=f3*z1+(1-f3)*z2;

    x2=x1*crz-y1*srz;
    y2=x1*srz+y1*crz;

    x1=x2*cry+z1*sry;
    z2=-x2*sry+z1*cry;

    y1=y2*crx-z2*srx;
    z1=y2*srx+z2*crx;

    alpha=max(-0.2*z1+0.3,0);

    z1=1/(3+z1*0.5);

    x=x1*z1;
    y=y1*z1;

    x=x*h/w;

    r1=react*su*f;
    b1=abs(x2+sry*f-(1-f));
    g1=0.2+react*(1-f);

    opt1=sin(i*3+ry*5);

    r2=2*(0.5+sin(u*4+rx)*2)*f+(1-f);
    b2=(above(cr,0.5)?cr:0.5+opt1*2)*f; // note: if --> ? :
    g2=0.5*(above(cr,0.5)?cr:0.5+opt1*2); // note: if --> ? :

    r3=abs(react);
    g3=abs(x2+react);

    red=(f1*r1+(1-f1)*r2)*alpha*f3+(1-f3)*(f2*r3*alpha+(1-f2)*(f+(1-f)*alpha));
    blue=(f1*b1+(1-f1)*b2)*alpha*f3+(1-f3)*(f2*alpha+(1-f2)*2*getspec(0.2,0.1,0)*(f+(1-f)*alpha));
    green=(f1*g1+(1-f1)*g2)*alpha*f3+(1-f3)*(f2*g3*alpha+(1-f2)*2*getosc(0.2,0.1,0)*(f+(1-f)*alpha));

    red=max(red,0.1);
    blue=max(blue,0.1);
    green=max(green,0.1);
    
    // note: addition
    red *= 3.0f;
    green *= 3.0f;
    blue *= 3.0f;
}

#undef sin
#undef cos
#undef sqrt
#undef pow
#undef max
#undef abs