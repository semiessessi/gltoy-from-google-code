varying vec3 xDirection;
varying vec3 xPosition;
uniform sampler2D xTexture;

vec3 xSolution;
vec3 xTraceSolution;
vec3 xNormalisedDirection;
vec3 xSpecularDirection;
vec3 xLightPosition;
float fGlossTexture;
float fHackEdgeSoften;
float fInitialK = -1.0;
float fK;

float solve( vec3 xPos, vec3 xDir )
{
	const float fSize = 0.375;
	xNormalisedDirection = normalize( xDir );
	
	// Quartic coefficients for Chmutov surface
	vec3 xSquares = xNormalisedDirection * xNormalisedDirection;
	vec3 xCubes = xSquares * xNormalisedDirection;
	vec3 xPosSquares = xPos * xPos;
	vec3 xCHelp = 6.0 * xPosSquares - 1.0;
	vec3 xDHelp = xPos * ( 4.0 * xPosSquares - 2.0 );

	// f = x^4 + y^4 + z^4 - x^2 - y^2 - z^2 + fSize
	float fA = dot( xSquares, xSquares ); // i.e. x^4 + y^4 + z^4
	float fB = 4.0 * dot( xCubes, xPosition ); // i.e. 4 * ( ox x^3 + oy y^3 + oz z^3 )
	float fC = dot( xCHelp, xSquares ); // i.e. x^2 * ( 6 ox^2 - 1 ) + y^2 * ( 6 oy^2 - 1 ) + z^2 * ( 6 oz^2 - 1 );
	float fD = dot( xNormalisedDirection, xDHelp ); // i.e. x * ox * ( 4 ox^2 - 2 ) + y * oy * ( 4 oy^2 - 2 ) + z * oz * ( 4 oz^2 - 2 );
	float fE = dot( xPosSquares, xPosSquares - 1.0 ) + fSize;
	
	fK = max( 0.0, fInitialK + length( xPos ) );
	float fL = -0.15;

	// Whittaker iteration
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fL = -0.5;
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
	fL = -2.0;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fL = -0.5;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	
	if( fK > 100.0 )
	{
		fK = -1.0;
	}

	fHackEdgeSoften = min( ( ( 4.0 * fA * fK + 3.0 * fB ) * fK + 2.0 * fC ) * fK + fD, 0.0 );
	fHackEdgeSoften = clamp( -fHackEdgeSoften, 0.0, 1.0 );

	return fK;
}

vec4 trace( vec3 xPos, vec3 xDir, bool bDiscard )
{
	float fK = solve( xPos, xDir );

	float fEdgeFade = fHackEdgeSoften;

	if( fK < 0.0 )
	{
		if( bDiscard )
		{
			discard;
		}

		return vec4( 0.0, 0.0, 0.0, 0.0 );
	}

	xSolution = xNormalisedDirection * fK + xPos;

	// f = x^4 + y^4 + z^4 - x^2 - y^2 - z^2 + fSize
	// df/dx = 4x^3 - 2x
	// df/dy = 4y^3 - 2y
	// df/sz = 4z^3 - 2z
	vec3 xNormal = normalize( 4.0 * xSolution * xSolution * xSolution - 2.0 * xSolution );
	// ok, lets mess with the normal, why not
	xNormal += 0.1 * normalize( sin( 170.0 * xSolution ) );
	xNormal = normalize( xNormal );

	float fLight = 0.0;
	float fSpecularity = 0.0;
	xTraceSolution = xSolution;
	vec3 xCurrentDirection = xNormalisedDirection;
	float fLightDistance = length( xTraceSolution - xLightPosition );
	vec3 xLightDirection = xTraceSolution - xLightPosition;
	vec3 xNormalisedLightDirection = normalize( xLightDirection );

	vec4 xDiffuseTexture = vec4( ( ( cos( 12.0 * xSolution.x ) * cos( 12.0 * xSolution.y ) * cos( 12.0 * xSolution.z ) ) > 0.0 ) ? vec3( 0.7, 0.6, 1.0 ) : vec3( 0.45, 0.3, 0.85 ), 1.0 );
	/*texture2D( xTexture,
		vec2(
			( 10.0 / 3.141592654 ) * atan( -xSolution.z, xSolution.x ),
			2.5 * xSolution.y
			) );*/
	float fSpecularTexture = ( xDiffuseTexture.x > 0.5 ) ? 32.0 : 8.0;
	fGlossTexture = ( fSpecularTexture > 16.0 ) ? 1.0 : 0.75;
	
	float fAttenuation = 3.0 / ( 1.0 + 0.5 * dot( xLightDirection, xLightDirection ) );
	fLight = clamp( fAttenuation * dot( xNormal, -xNormalisedLightDirection ), 0.0, 1.0 );
	
	xSpecularDirection = 2.0 * dot( xNormal, -xCurrentDirection ) * xNormal + xCurrentDirection;
	fSpecularity = fGlossTexture * clamp( 0.25 * pow( dot( xSpecularDirection, -xNormalisedLightDirection ), fSpecularTexture ), 0.00001, 1.0 );

	return fEdgeFade * ( fLight * 0.8 * xDiffuseTexture + vec4( fSpecularity, fSpecularity, fSpecularity, 1.0 ) );
}

void main()
{
	xLightPosition = xPosition - normalize( xDirection );
	vec4 xColour = trace( xPosition, xDirection, true );
	float fFade = fHackEdgeSoften;
	float fBounceScale = 1.0;
	float fGloss = fBounceScale * fGlossTexture;
	// reflect
	// 1 bounce
	xColour += fFade * fGloss * trace( xTraceSolution + xSpecularDirection * 0.05, xSpecularDirection, false );
	// fGloss *= fBounceScale * fGlossTexture;
	// 2 bounces
	// xColour += fFade * fGloss * trace( xTraceSolution + xSpecularDirection * 0.05, xSpecularDirection, false );
	// fGloss *= fBounceScale * fGlossTexture;
	// 3 bounces
	// xColour += fFade * fGloss * trace( xTraceSolution + xSpecularDirection * 0.05, xSpecularDirection, false );
	// fGloss *= fBounceScale * fGlossTexture;
	// 4 bounces
	// xColour += fFade * fGloss * trace( xTraceSolution + xSpecularDirection * 0.05, xSpecularDirection, false );

	gl_FragColor = xColour;

	// gl_FragDepth = ( gl_DepthRange.far - gl_DepthRange.near ) gl_DepthRange.diff
}