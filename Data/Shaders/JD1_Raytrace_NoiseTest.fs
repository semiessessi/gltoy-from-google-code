varying vec3 xDirection;
varying vec3 xPosition;

void main()
{
	//vec4 xColour = texture2D( xNoiseTexture, xDirection.xy );
	float fNoise = noise2d( xDirection.xy * 0.0002 * pow( abs( xPosition.z ), 4.0 ) ); // remember the intial texture has quite high frequency and tiles...
	vec4 xColour = vec4( fNoise, fNoise, fNoise, 1.0 );
	gl_FragColor = xColour;
}