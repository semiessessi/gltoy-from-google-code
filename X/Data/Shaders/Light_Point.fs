uniform sampler2D xDiffuseSampler;
uniform sampler2D xNormalSampler;
uniform sampler2D xDepthSampler;

uniform float fLightRadius;
uniform vec3 xCameraPosition;
uniform vec3 xLightPosition;
uniform vec3 xLightColour;

uniform vec2 xSize;
uniform vec2 xOneOverSize;
uniform vec2 xClipPlanes;

uniform mat4x4 xViewMatrix;
uniform mat4x4 xInverseViewMatrix;

void main()
{
	// TODO: fix this up
	vec3 xDiffuse = texture2D( xDiffuseSampler, gl_FragCoord.xy * xOneOverSize ).xyz;
	vec3 xNormal = ( vec4( InverseStereographicProjection( 4.0 * texture2D( xNormalSampler, gl_FragCoord.xy * xOneOverSize ).xy - 2.0 ), 0.0 ) * xInverseViewMatrix ).xyz;
	vec3 xPosition = 
		( vec4( ViewPositionFromDepth(
			texture2D( xDepthSampler, gl_FragCoord.xy * xOneOverSize ).x,
			xClipPlanes,
			gl_FragCoord,
			xOneOverSize ), 1.0 )
		* xInverseViewMatrix ).xyz;
	vec3 xLightVector = xPosition - xLightPosition;
	float fAttenuation = clamp( 1.0f - dot( xLightVector, xLightVector ) / ( fLightRadius * fLightRadius ), 0.0, 1.0 );
	float fNormalDot = clamp( -dot( xNormal, normalize( xLightVector ) ), 0.0, 1.0 );
	vec3 xColour = xLightColour * fAttenuation * fNormalDot * xDiffuse;
	gl_FragColor = vec4( xColour, 1.0 );
}