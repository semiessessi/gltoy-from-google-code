varying vec2 xTexCoord;

uniform sampler2D xLightmapSampler;
uniform sampler2D xDiffuseSampler;
uniform sampler2D xNormalSampler;

uniform vec2 xSize;
uniform vec2 xOneOverSize;

void main()
{
	vec3 xLightmap = texture2D( xLightmapSampler, xTexCoord ).xyz;
	vec3 xDiffuse = texture2D( xDiffuseSampler, gl_FragCoord.xy * xOneOverSize ).xyz;
	vec3 xWorldNormal = ReconstructNormal( texture2D( xNormalSampler, gl_FragCoord.xy * xOneOverSize ).xy );
	
	gl_FragColor = vec4( xLightmap * xDiffuse, 1.0 );
}
