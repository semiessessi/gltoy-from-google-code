uniform sampler2D xDiffuseSampler;
uniform sampler2D xNormalSampler;
uniform sampler2D xSpecularSampler;
uniform sampler2D xDepthSampler;

uniform vec3 xCameraPosition;
uniform vec3 xLightDirection;
uniform vec3 xLightColour;

uniform vec2 xSize;
uniform vec2 xOneOverSize;
uniform vec2 xClipPlanes;

uniform mat4x4 xViewMatrix;
uniform mat4x4 xInverseViewMatrix;

void main()
{
		vec3 xDiffuse = texture2D( xDiffuseSampler, gl_FragCoord.xy * xOneOverSize ).xyz;
		vec3 xNormal = ( vec4( normalize( InverseStereographicProjection( 4.0 * texture2D( xNormalSampler, gl_FragCoord.xy * xOneOverSize ).xy - 2.0 ) ) , 0.0 ) * xInverseViewMatrix ).xyz;
		vec4 xSpecularSample = texture2D( xSpecularSampler, gl_FragCoord.xy * xOneOverSize );
		vec3 xPosition = 
			( vec4( ViewPositionFromDepth(
				texture2D( xDepthSampler, gl_FragCoord.xy * xOneOverSize ).x,
				xClipPlanes,
				gl_FragCoord,
				xOneOverSize ), 1.0 )
			* xInverseViewMatrix ).xyz;
	
	vec3 xViewVector = normalize( xCameraPosition - xPosition );
	vec3 xReflectedLightVector = xLightDirection;
	xReflectedLightVector = xReflectedLightVector - 2.0 * dot( xNormal, xReflectedLightVector ) * xNormal;
	
	float fBrightness = dot( vec3( 0.333333, 0.333333, 0.333333 ), xLightColour.xyz );
	float fNormalDot = clamp( -dot( xNormal, xLightDirection ), 0.0, 1.0 );
	float fSpecularDot = clamp( dot( xViewVector, xReflectedLightVector ), 0.0, 1.0 );
	float fFresnelDot = clamp( -dot( xViewVector, xNormal ), 0.0, 1.0 );
	
	vec3 xDiffuseComponent = xLightColour * fNormalDot * xDiffuse;
	float fSpecularComponent = fBrightness * xSpecularSample.x * pow( fSpecularDot, 2.0 + xSpecularSample.y * 2048.0 )
				* pow( 1.0 - fFresnelDot, xSpecularSample.z * 2048.0 );
	
	// mute the specular massively... its directional so it can be really bright if not done..
	vec3 xColour = clamp( xDiffuseComponent + 0.1 * fSpecularComponent, 0.0, 1.0 );
	
	gl_FragColor = vec4( xColour, 1.0 );
}