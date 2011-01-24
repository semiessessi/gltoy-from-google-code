uniform sampler2D xSampler;

varying vec2 xTexCoord;

void main()
{
	gl_FragColor = vec4( 0.5 + 0.5 * ReconstructNormal( texture2D( xSampler, xTexCoord ).rg ), 1.0 );
}