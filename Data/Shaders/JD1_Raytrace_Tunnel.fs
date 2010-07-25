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
	
	normalize( xDirection );
	float fA = 2 * ( xDirection.x * xDirection.x + xDirection.y * xDirection.y );
	float fB = 2 * ( xPosition.x * xDirection.x + xPosition.y * xDirection.y );
	float fC = 2 * ( xPosition.x * xPosition.x + xPosition.y * xPosition.y - 4096.0f ); // r = 64
	float fT = ( sqrt( fB*fB - fA*fC ) - fB ) / fA;
	
	vec3 xSolution = xDirection * fT + xPosition;
	
	gl_FragDepth = ( fT > 0.0 ) ? 0.99999 : 1.0; // TODO - something clever about this
	gl_FragColor = ( fT > 0.0 ) ? texture2D( xTexture, vec2( ( 4.0f / 3.141592654f ) * atan( -xSolution.y, xSolution.x ), 0.015625 * xSolution.z ) ) : vec4( 0.0, 0.0, 0.0, 0.0 );
}