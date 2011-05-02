uniform sampler2D xSampler;

varying vec2 xTexCoord;

void main()
{
	float fDepth = pow( texture2D( xSampler, xTexCoord ).x, 800 );
	gl_FragColor = vec4( fDepth, fDepth, fDepth, 1.0 );
}
