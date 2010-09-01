#version 120

varying vec3 xDirection;
varying vec3 xPosition;
uniform sampler2D xTexture;

vec3 xSolution;
vec3 xNormalisedDirection;
vec3 xNormal;

/*
x1=x;y1=y*asp;f=1/sqrt(sqr(x1)+sqr(y1)+1);
x2=x1*crz-y1*srz;y2=x1*srz+y1*crz;
y1=y2*crx-srx;z2=y2*srx+crx;
x1=x2*cry-z2*sry;z1=x2*sry+z2*cry;

·Quartic Coefficients for ax^4+bx^3+cx^2+dx+e=0;
·(Chmutov Surface);
x2=sqr(x1);y2=sqr(y1);z2=sqr(z1);
va=sqr(x2)+sqr(y2)+sqr(z2);
vb=4*(x2*x1*gx+y2*y1*gy+z2*z1*gz);
vc=sqr(x1)*(6*sqr(gx)-1)+sqr(y1)*(6*sqr(gy)-1)+sqr(z1)*(6*sqr(gz)-1);
vd=x1*gx*(4*sqr(gx)-2)+y1*gy*(4*sqr(gy)-2)+z1*gz*(4*sqr(gz)-2);
ve=(sqr(gx)*(sqr(gx)-1))+(sqr(gy)*(sqr(gy)-1))+(sqr(gz)*(sqr(gz)-1))+.375;

k=f*.6;wh=-.15;
£ Whittaker Iteration;
k=k-((((va*k+vb)*k+vc)*k+vd)*k+ve)*wh;
k=k-((((va*k+vb)*k+vc)*k+vd)*k+ve)*wh;
k=k-((((va*k+vb)*k+vc)*k+vd)*k+ve)*wh;
wh=-.5;
k=k-((((va*k+vb)*k+vc)*k+vd)*k+ve)*wh;
k=k-((((va*k+vb)*k+vc)*k+vd)*k+ve)*wh;
k=k-((((va*k+vb)*k+vc)*k+vd)*k+ve)*wh;
k=k-((((va*k+vb)*k+vc)*k+vd)*k+ve)*wh;
wh=-.5;
k=k-((((va*k+vb)*k+vc)*k+vd)*k+ve)*wh;
k=k-((((va*k+vb)*k+vc)*k+vd)*k+ve)*wh;
k=k-((((va*k+vb)*k+vc)*k+vd)*k+ve)*wh;
k=k-((((va*k+vb)*k+vc)*k+vd)*k+ve)*wh;
wh=-.5;
k=k-((((va*k+vb)*k+vc)*k+vd)*k+ve)*wh;
k=k-((((va*k+vb)*k+vc)*k+vd)*k+ve)*wh;
k=k-((((va*k+vb)*k+vc)*k+vd)*k+ve)*wh;
k=k-((((va*k+vb)*k+vc)*k+vd)*k+ve)*wh;
wh=-2;
k=k-((((va*k+vb)*k+vc)*k+vd)*k+ve)*wh;
k=k-((((va*k+vb)*k+vc)*k+vd)*k+ve)*wh;
k=k-((((va*k+vb)*k+vc)*k+vd)*k+ve)*wh;
k=k-((((va*k+vb)*k+vc)*k+vd)*k+ve)*wh;

x=asin(sin(atan2(y1*k+gy,z1*k+gz)));y=x1*k+gx;

fk0=min(0,((4*va*k+3*vb)*k+2*vc)*k+vd);
alpha=sqr(fk0)*.5+exp(-k)*.5;
*/

vec4 trace( vec3 xPos, vec3 xDir )
{
	const float fSize = 0.375f;
	xNormalisedDirection = normalize( xDir );
	
	// Quartic coefficients for Chmutov surface
	vec3 xSquares = xNormalisedDirection * xNormalisedDirection;
	vec3 xCubes = xSquares * xNormalisedDirection;
	vec3 xPosSquares = xPos * xPos;
	vec3 xCHelp = 6.0f * xPosSquares - 1.0f;
	vec3 xDHelp = xPos * ( 4.0f * xPosSquares - 2.0f );

	float fA = dot( xSquares, xSquares ); // i.e. x^4 + y^4 + z^4
	float fB = 4.0f * dot( xCubes, xPosition ); // i.e. 4 * ( ox x^3 + oy y^3 + oz z^3 )
	float fC = dot( xCHelp, xSquares ); // i.e. x^2 * ( 6 ox^2 - 1 ) + y^2 * ( 6 oy^2 - 1 ) + z^2 * ( 6 oz^2 - 1 );
	float fD = dot( xNormalisedDirection, xDHelp ); // i.e. x * ox * ( 4 ox^2 - 2 ) + y * oy * ( 4 oy^2 - 2 ) + z * oz * ( 4 oz^2 - 2 );
	float fE = dot( xPosSquares, xPosSquares - 1.0f ) + fSize;
	
	float fK = 1.0f / sqrt( dot( xNormalisedDirection.xy, xNormalisedDirection.xy ) + 1.0f );
	float fL = -0.15f;

	// Whittaker iteration
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fL = -0.5f;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fL = -2.0f;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;

	if( fK < 0.0f )
	{
		 return vec4( 0.0, 0.0, 0.0, 0.0 );
	}

	if( fK > 100.0f )
	{
		 return vec4( 0.0, 0.0, 0.0, 0.0 );
	}

	xSolution = xNormalisedDirection * fK + xPos;

	//xNormal = -normalize( vec3( xSolution.x, xSolution.y, 8.0f * sin( 0.6f * xSolution.z ) ) );
	//float fAttenuation = 32.0f / ( 1.0f + 0.005f * dot( xSolution - xPos, xSolution - xPos ) );
	//float fLight = fAttenuation * dot( xNormal, -xNormalisedDirection );
	//vec3 xSpecularDirection = 2 * dot( xNormal, -xNormalisedDirection ) * xNormal + xNormalisedDirection;
	//float fSpecularity = clamp( fAttenuation * pow( dot( xSpecularDirection, xNormalisedDirection ), 8.0f ), 0.0f, 1.0f );
	

	return
		/* fLight * */
		texture2D( xTexture, vec2( ( 4.0f / 3.141592654f ) * atan( -xSolution.y, xSolution.x ), 2.0f * xSolution.z ) )
		/* + vec4( fSpecularity, fSpecularity, fSpecularity, 1.0f ) */;
}

void main()
{
	gl_FragColor = trace( xPosition, xDirection );

	//gl_FragDepth = ( fT > 0.0 ) ? 0.99999 : 1.0; // TODO - something clever about this
}