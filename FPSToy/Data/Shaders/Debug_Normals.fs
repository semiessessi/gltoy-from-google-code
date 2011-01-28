uniform sampler2D xSampler;

varying vec2 xTexCoord;

void main()
{
	gl_FragColor = vec4( 0.5 + 0.5 * InverseStereographicProjection( 4.0 * texture2D( xSampler, xTexCoord ).rg - 2.0 ), 1.0 );
}