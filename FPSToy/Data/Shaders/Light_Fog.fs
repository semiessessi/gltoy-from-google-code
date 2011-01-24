uniform sampler2D xDiffuseSampler;

uniform vec2 xOneOverSize;
//uniform vec4 xFogColour;

void main()
{
	float fFog = texture2D( xDiffuseSampler, gl_FragCoord.xy * xOneOverSize ).a;
	//vec4 xFogColour = vec4( 0.5, 0.5, 0.5, 0.5 );
	vec4 xFogColour = vec4( 0.0, 0.0, 0.0, 0.0 );
	gl_FragColor = vec4( xFogColour.rgb, xFogColour.a * fFog );
	
}