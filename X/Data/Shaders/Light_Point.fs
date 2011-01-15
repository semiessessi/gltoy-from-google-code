uniform sampler2D xDiffuseSampler;
uniform sampler2D xNormalSampler;
uniform sampler2D xDepthSampler;

uniform float fLightRadius;
uniform vec3 xCameraPosition;
uniform vec3 xLightPosition;
uniform vec3 xLightColour;
// this view vector is not good enough... it should be a varying
uniform vec3 xViewVector;

uniform vec2 xSize;
uniform vec2 xOneOverSize;

void main()
{
	// TODO: fix this up
	vec3 xDiffuse = texture2D( xDiffuseSampler, gl_FragCoord.xy * xOneOverSize ).xyz;
	// HACK: crappy depth in alpha...
	vec4 xNormal = texture2D( xNormalSampler, gl_FragCoord.xy * xOneOverSize ).xyz;
	// this depth value is bollocks
	vec3 xPosition = xCameraPosition + xViewVector * xNormal.w * 1000.0;
	vec3 xLightVector = xPosition - xLightPosition;
	float fAttenuation = 1.0f - dot( xLightVector, xLightVector ) / ( fLightRadius * fLightRadius );
	float fNormalDot = 1.0; // TODO...
	vec3 xColour = xLightColour * fAttenuation * fNormalDot * xDiffuse;
	gl_FragColor = vec4( xColour, 1.0 );
}