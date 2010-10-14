varying vec3 xDirection;
varying vec3 xPosition;

void main()
{
	//vec4 xColour = texture2D( xNoiseTexture, xDirection.xy );
	float fNoise = noise2d( xDirection.xy / ( abs( xPosition.z ) + 1.0f ) ); // remember the intial texture has quite high frequency and tiles...
	vec4 xColour = vec4( fNoise, fNoise, fNoise, 1.0 );
	gl_FragColor = xColour;
}