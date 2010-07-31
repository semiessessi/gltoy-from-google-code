#version 120

varying vec3 xDirection;
varying vec3 xPosition;
uniform sampler2D xTexture;

void main()
{
	// x^2 + y^2 - r^2 = 0
	// (ox + dx*t)^2 + (oy + dy*t)^2 - r^2 = 0
	// a = 2 *(dx^2 + dy^2)
	// b = 2 *(ox*dx + oy*dy)
	// c = 2 *(ox^2 + oy^2 - r^2)
	// t = ( -b + sqrt(b*b - a*c) ) / a
	
	vec3 xNormalisedDirection = normalize( xDirection );
	float fA = 2 * ( xNormalisedDirection.x * xNormalisedDirection.x + xNormalisedDirection.y * xNormalisedDirection.y );
	float fB = 2 * ( xPosition.x * xNormalisedDirection.x + xPosition.y * xNormalisedDirection.y );
	float fC = 2 * ( xPosition.x * xPosition.x + xPosition.y * xPosition.y - 4096.0f ); // r = 64
	float fT = ( sqrt( fB*fB - fA*fC ) - fB ) / fA;
	
	vec3 xSolution = xNormalisedDirection * fT + xPosition;
	
	gl_FragDepth = ( fT > 0.0 ) ? 0.99999 : 1.0; // TODO - something clever about this
	vec4 xFragColor = texture2D( xTexture, vec2( ( 4.0f / 3.141592654f ) * atan( -xSolution.y, xSolution.x ), 0.015625 * xSolution.z ) );

	// reflection
	vec3 xNormal = -normalize( vec3( xSolution.x, xSolution.y, 0.0f ) );
	vec3 xReflectionPosition = xSolution;
	vec3 xReflectionDirection = normalize( xNormalisedDirection - 2 * dot( xNormal, xNormalisedDirection ) * xNormal );
	fA = 2 * ( xReflectionDirection.x * xReflectionDirection.x + xReflectionDirection.y * xReflectionDirection.y );
	fB = 2 * ( xReflectionPosition.x * xReflectionDirection.x + xReflectionPosition.y * xReflectionDirection.y );
	fC = 2 * ( xReflectionPosition.x * xReflectionPosition.x + xReflectionPosition.y * xReflectionPosition.y - 4096.0f ); // r = 64
	fT = ( sqrt( fB*fB - fA*fC ) - fB ) / fA;
	
	xSolution = xReflectionDirection * fT + xReflectionPosition;

	gl_FragColor = xFragColor + 0.5f * texture2D( xTexture, vec2( ( 4.0f / 3.141592654f ) * atan( -xSolution.y, xSolution.x ), 0.015625 * xSolution.z ) );
}