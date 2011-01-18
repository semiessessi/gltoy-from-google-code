uniform sampler2D xDiffuseSampler;
uniform sampler2D xNormalSampler;

uniform vec3 xLightDirection;
uniform vec3 xLightColour;

uniform vec2 xSize;
uniform vec2 xOneOverSize;

void main()
{
	vec3 xDiffuse = texture2D( xDiffuseSampler, gl_FragCoord.xy * xOneOverSize ).xyz;
	vec3 xNormal = texture2D( xNormalSampler, gl_FragCoord.xy * xOneOverSize ).xyz;
	vec3 xColour = xLightColour * xDiffuse * clamp( dot( xNormal, xLightDirection ), 0.0, 1.0 );
	gl_FragColor = vec4( xColour, 1.0 );
}