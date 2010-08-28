// Thomas Young 25.08.10

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/JD1.h>

// This file's header
#include <Render/Texer/JD1_Texer_P9_Birth.h>

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
#define min GLToy_Maths::Min
#define abs GLToy_Maths::Abs
#define PI GLToy_Maths::Pi

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

static float focal, depth, rad, start, todeath, theta, delta, life, death, slow, lasttime, insz, x1,y1,z1,x2,y2,z2,z3,x3,sc, mxdeath, dcount;
static float m1,m2,m3,m4,m5,rotx,roty,rotz,srotx,sroty,srotz,crotx,croty,crotz;
static int posit, num, total;

static const int iRenderOff = 100 * 1024;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

JD1_Texer_P9_Birth::JD1_Texer_P9_Birth() : GLToy_Parent( GLToy_Hash_Constant( "Sprites/P9_Sprite1.png" ) )
{
    SetOrdering( ucSPRITE_ORDERED_ALWAYS );
}

JD1_Texer_P9_Birth::~JD1_Texer_P9_Birth()
{
}

void JD1_Texer_P9_Birth::Initialise()
{
    int pos = 0;
    int pos1= 0;

    n=0;
    focal=1;
    depth=1;
    rad=0.5;
    num=4000;
    start=1;
    todeath=0;
    mxdeath=rand(1000)/300.0f;

    for(int i=0;i<num;++i)
    {
        theta=rand(1000)/1000.0f*PI;
        delta=rand(1000)/1000.0f*PI*2.0f;
        rad=rand(1000)/1000.0f;
        life=0;
        death=rand(1000)/1000.0f;

        megabuf(pos1) = theta;
        megabuf(pos1+1) = delta;
        megabuf(pos1+2) = life;
        megabuf(pos1+3) = death;
        megabuf(pos1+4) = rad;

        pos1=pos1+5;
    }
}

void JD1_Texer_P9_Birth::PerFrame()
{
    slow=gettime(0)-lasttime;
    lasttime=gettime(0);

    m1=m1+slow*0.573f*0.05f;
    m2=m2+slow*0.722f*0.05f;
    m3=m3+slow*0.2872f*0.05f;
    m4=m4+slow*0.844f*0.05f;
    m5=m5+slow*0.313f*0.05f;

    rotx=rotx+(sin(m1)+sin(m2)*0.5f+sin(m3)*0.1f)*0.001f;
    roty=roty+(sin(m2)+sin(m3)*0.5f+sin(m4)*0.1f)*0.001f;
    rotz=rotz+(sin(m3)+sin(m4)*0.5f+sin(m5)*0.1f)*0.001f;

    srotx=sin(rotx);
    sroty=sin(roty);
    srotz=sin(rotz);
    crotx=cos(rotx);
    croty=cos(roty);
    crotz=cos(rotz);

    total=0;
    int pos1=0;
    int pos2= iRenderOff;
    for(int i=0;i<num;++i)
    {

        theta = megabuf(pos1);
        delta = megabuf(pos1+1);
        life = megabuf(pos1+2);
        death=megabuf(pos1+3);
        rad= megabuf(pos1+4);

        life=life+slow*0.1f;

        if( life > death )
        {
            theta=rand(1000)/1000.0f*PI;
            delta=rand(1000)/1000.0f*PI*2.0f;
            life=0.0f;
            death=rand(100)/100.0f+mxdeath;
        }

        insz=0.3f;
        if( life > insz )
        {
            rad=rad+slow*(min(life,1.0f)-insz)*0.05f;
        }
        else
        {
            rad=0.25f;
        }

        x1= rad*sin(theta)*cos(delta);
        y1= rad*sin(theta)*sin(delta);
        z1= rad*cos(theta);

        x2 = x1*crotz-y1*srotz;
        y2 = x1*srotz+y1*crotz;
        x3 = x2*croty+z1*sroty;
        z3 = -x2*sroty+z1*croty;
        y1 = y2*crotx-z3*srotx;
        z1 = y2*srotx+z3*crotx;


        z2=focal/(z1+depth);

        if( z1 > -1.0f )
        {
            megabuf(pos2) = x3*z2;
            megabuf(pos2+1) = y1*z2;
            megabuf(pos2+2) = z1;
            megabuf(pos2+3) = life/death;
            pos2=pos2+4;
            total=total+1;
        }

        megabuf(pos1) = theta;
        megabuf(pos1+1) = delta;
        megabuf(pos1+2) = life;
        megabuf(pos1+3) = death;
        megabuf(pos1+4) = rad;
        pos1=pos1+5;
    }

    dcount=dcount+slow;
    if( dcount > 5)
    {
        dcount=0.0f;
        mxdeath=rand(1000)/300.0f;
    }

    n=total;
    posit=iRenderOff;
    sc=1;
}

void JD1_Texer_P9_Birth::OnBeat()
{
}

void JD1_Texer_P9_Birth::PerPoint(
    const float i,
    const float v,
    float& x, float& y, float& z,
    float& red, float& green, float& blue,
    bool& skip,
    float& sizex,
    float& sizey ) const
{
    x=megabuf(posit);
    y=megabuf(posit+1);
    float zed=min(megabuf(posit+2),0.3f);
    float zed1=megabuf(posit+2);
    life=megabuf(posit+3);

    float c2=1-max( min( (life-0.2f)*5.0f ,1.0f),0.0f);

    green=c2*0.5f+(1.0f-c2)*(0.5f+sin(life*PI)*0.5f);
    blue=c2*0.5f+(1.0f-c2)*(1.0f-sin(life*PI*2)*0.15f);
    red=c2*0.6f+(1.0f-c2)*0.6f;

    float mod=sqrt(sin(life*PI))*(0.5f-(zed));
    red=red*mod;
    blue=blue*mod;
    green=green*mod;

    float siz=(1.0f-(zed+0.5f)*0.5f)*sc;
    sizex=siz*0.02f;
    sizey=siz*0.02f;

    posit=posit+4; 
    z=1;
}

#undef acos
#undef cos 
#undef sin 
#undef sqrt
#undef pow 
#undef max 
#undef min 
#undef abs 
#undef PI
