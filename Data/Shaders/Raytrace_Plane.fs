varying vec3 xDirection;
varying vec3 xPosition;
uniform sampler2D xTexture;

void main()
{
	// ax + by + cz + d = 0
	// t = -( d + aox + boy + coz ) / ( adx + bdy + cdz )
	normalize( xDirection );
	vec4 xPlane = vec4( 0, 1, 0, 0 );
	vec3 xPlaneNormal = vec3( xPlane );
	float fT = -( xPlane.w + dot( xPosition, xPlaneNormal ) ) / ( dot( xDirection, xPlaneNormal ) );
	
	vec3 xSolution = xDirection * fT + xPosition;
	
	gl_FragDepth = ( fT > 0.0f ) ? fT : 1000000.0f;
	gl_FragColor = ( fT > 0.0f ) ? texture2D( xTexture, 0.01 * vec2( xSolution.x, xSolution.z ) ) : vec4( 0, 0, 0, 0 );
}