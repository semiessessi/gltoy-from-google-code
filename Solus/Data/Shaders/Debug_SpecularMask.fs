uniform sampler2D xSampler;

varying vec2 xTexCoord;

void main()
{
	gl_FragColor = vec4( texture2D( xSampler, xTexCoord ).rrr, 1.0 );
}