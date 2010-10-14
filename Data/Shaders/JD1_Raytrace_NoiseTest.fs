varying vec3 xDirection;
varying vec3 xPosition;

void main()
{
	vec4 xColour = trace( 0.0, 0.0, 0.0, 0.0 );
	gl_FragColor = xColour;
}