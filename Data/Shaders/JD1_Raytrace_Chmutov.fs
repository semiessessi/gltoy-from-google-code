#version 120

varying vec3 xDirection;
varying vec3 xPosition;
uniform sampler2D xTexture;

vec3 xSolution;
vec3 xTraceSolution;
vec3 xNormalisedDirection;
uniform vec4 xDebugVector = vec4( 1.0f, 1.0f, 1.0f, 1.0f );
float fHackEdgeSoften;

float solve( vec3 xPos, vec3 xDir )
{
	const float fSize = 0.375f;
	xNormalisedDirection = normalize( xDir );
	
	// Quartic coefficients for Chmutov surface
	vec3 xSquares = xNormalisedDirection * xNormalisedDirection;
	vec3 xCubes = xSquares * xNormalisedDirection;
	vec3 xPosSquares = xPos * xPos;
	vec3 xCHelp = 6.0f * xPosSquares - 1.0f;
	vec3 xDHelp = xPos * ( 4.0f * xPosSquares - 2.0f );

	// f = x^4 + y^4 + z^4 - x^2 - y^2 - z^2 + fSize
	float fA = dot( xSquares, xSquares ); // i.e. x^4 + y^4 + z^4
	float fB = 4.0f * dot( xCubes, xPosition ); // i.e. 4 * ( ox x^3 + oy y^3 + oz z^3 )
	float fC = dot( xCHelp, xSquares ); // i.e. x^2 * ( 6 ox^2 - 1 ) + y^2 * ( 6 oy^2 - 1 ) + z^2 * ( 6 oz^2 - 1 );
	float fD = dot( xNormalisedDirection, xDHelp ); // i.e. x * ox * ( 4 ox^2 - 2 ) + y * oy * ( 4 oy^2 - 2 ) + z * oz * ( 4 oz^2 - 2 );
	float fE = dot( xPosSquares, xPosSquares - 1.0f ) + fSize;
	
	float fK = 1.5f;
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
	fL = -0.5f;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;
	fK = fK - ( ( ( ( fA * fK + fB ) * fK + fC ) * fK + fD ) * fK + fE ) * fL;

	if( fK > 100.0f )
	{
		fK = -1.0f;
	}

	fHackEdgeSoften = min( ( ( 4.0f * fA * fK + 3.0f * fB ) * fK + 2.0f * fC ) * fK + fD, 0.0f );
	fHackEdgeSoften = ( xDebugVector.x > 0.0f ) ? clamp( -fHackEdgeSoften, 0.0f, 1.0f ) : 1.0f;

	return fK;
}

vec4 trace( vec3 xPos, vec3 xDir )
{
	float fK = solve( xPos, xDir );

	float fEdgeFade = fHackEdgeSoften;

	if( fK < 0.0f )
	{
		discard;
		return vec4( 0.0f, 0.0f, 0.0f, 0.0f );
	}

	xSolution = xNormalisedDirection * fK + xPos;

	// f = x^4 + y^4 + z^4 - x^2 - y^2 - z^2 + fSize
	// df/dx = 4x^3 - 2x
	// df/dy = 4y^3 - 2y
	// df/sz = 4z^3 - 2z
	vec3 xNormal = normalize( 4.0f * xSolution * xSolution * xSolution - 2.0f * xSolution );
	vec3 xLightPosition = xPos;
	float fLight = 0.0f;
	float fSpecularity = 0.0f;
	xTraceSolution = xSolution;
	vec3 xCurrentDirection = xNormalisedDirection;
	float fLightDistance = length( xTraceSolution - xLightPosition );
	vec3 xLightDirection = normalize( xTraceSolution - xLightPosition );

	vec4 xDiffuseTexture = texture2D( xTexture, vec2( ( 8.0f / 3.141592654f ) * atan( -xSolution.z, xSolution.x ), 2.0f * xSolution.y ) );
	float fSpecularTexture = ( dot( xDiffuseTexture, xDiffuseTexture ) > 1.5f ) ? 32.0f : 16.0f;
	float fGlossTexture = ( fSpecularTexture > 16.0f ) ? 1.0f : 0.75f;
	
	// this doesn't seem to work at all...
	//float fShadow = 1.0f - clamp( 100.0f * solve( xTraceSolution + xLightDirection * 0.01f, xLightDirection ), 0.0f, 1.0f );
	
	float fAttenuation = /* fShadow * */ 1.0f / ( 1.0f + 0.5f * dot( xLightDirection, xLightDirection ) );
	fLight = clamp( fAttenuation * dot( xNormal, -xLightDirection ), 0.0f, 1.0f );
	
	vec3 xSpecularDirection = 2 * dot( xNormal, -xCurrentDirection ) * xNormal + xCurrentDirection;
	fSpecularity = fGlossTexture * clamp( 0.25f * pow( dot( xSpecularDirection, -xLightDirection ), fSpecularTexture ), 0.0f, 1.0f );

	// return xDiffuseTexture;

	return fEdgeFade * (
			fLight * 0.8f * xDiffuseTexture * xDebugVector.y
			+ vec4( fSpecularity, fSpecularity, fSpecularity, 1.0f ) * xDebugVector.z
		);
}

void main()
{
	gl_FragColor = trace( xPosition, xDirection );

	// gl_FragDepth = ( gl_DepthRange.far - gl_DepthRange.near ) gl_DepthRange.diff
}