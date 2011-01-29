uniform sampler2D xDiffuseSampler;
uniform sampler2D xNormalSampler;

uniform vec3 xLightDirection;
uniform vec3 xLightColour;

uniform vec2 xSize;
uniform vec2 xOneOverSize;

uniform mat4x4 xViewMatrix;
uniform mat4x4 xInverseViewMatrix;

void main()
{
	vec3 xDiffuse = texture2D( xDiffuseSampler, gl_FragCoord.xy * xOneOverSize ).xyz;
	vec3 xWorldNormal = normalize( ( vec4( InverseStereographicProjection( 4.0 * texture2D( xNormalSampler, gl_FragCoord.xy * xOneOverSize ).xy - 2.0 ), 0.0 ) * xInverseViewMatrix ).xyz );
	vec3 xColour = clamp( xLightColour * xDiffuse * dot( xWorldNormal, xLightDirection ), 0.0, 1.0 );
	gl_FragColor = vec4( xColour, 1.0 );
}