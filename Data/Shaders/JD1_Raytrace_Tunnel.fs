#version 120

varying vec3 xDirection;
varying vec3 xPosition;
uniform sampler2D xTexture;

vec3 xSolution;
vec3 xNormalisedDirection;
vec3 xNormal;

vec4 trace( vec3 xPos, vec3 xDir )
{
	xNormalisedDirection = normalize( xDir );
	float fA = 2 * dot( xNormalisedDirection.xy, xNormalisedDirection.xy );
	float fB = 2 * dot( xPos.xy, xNormalisedDirection.xy );
	float fC = 2 * ( dot( xPos.xy, xPos.xy ) - 4096.0f ); // r = 64
	float fT = ( sqrt( fB*fB - fA*fC ) - fB ) / fA;
	
	xSolution = xNormalisedDirection * fT + xPos;

	xNormal = -normalize( vec3( xSolution.x, xSolution.y, 8.0f * sin( 0.6f * xSolution.z ) ) );
	float fAttenuation = 32.0f / ( 1.0f + 0.005f * dot( xSolution - xPos, xSolution - xPos ) );
	float fLight = fAttenuation * dot( xNormal, -xNormalisedDirection );
	vec3 xSpecularDirection = 2 * dot( xNormal, -xNormalisedDirection ) * xNormal + xNormalisedDirection;
	float fSpecularity = clamp( fAttenuation * pow( dot( xSpecularDirection, xNormalisedDirection ), 8.0f ), 0.0f, 1.0f );
	
	return fLight * /*texture2D( xTexture, vec2( ( 4.0f / 3.141592654f ) * atan( -xSolution.y, xSolution.x ), 0.015625 * xSolution.z ) )*/
		vec4( 0.1f, 0.1f, 0.1f, 0.0f )
		+ vec4( fSpecularity, fSpecularity, fSpecularity, 1.0f );
}

void main()
{
	vec4 xFirst = trace( xPosition, xDirection );

	// reflection
    vec3 xNormal = -normalize( vec3( xSolution.x, xSolution.y, 0.0f ) );
    vec3 xReflectionPosition = xSolution;
    vec3 xReflectionDirection = normalize( xNormalisedDirection - 2 * dot( xNormal, xNormalisedDirection ) * xNormal );

	gl_FragColor = xFirst + 0.95f * trace( xReflectionPosition, xReflectionDirection );

	//gl_FragDepth = ( fT > 0.0 ) ? 0.99999 : 1.0; // TODO - something clever about this
}