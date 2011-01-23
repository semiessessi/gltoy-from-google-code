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
uniform vec2 xClipPlanes;

uniform mat4x4 xViewMatrix;
uniform mat4x4 xInverseViewMatrix;

void main()
{
	// TODO: fix this up
	vec3 xDiffuse = texture2D( xDiffuseSampler, gl_FragCoord.xy * xOneOverSize ).xyz;
	vec3 xNormal = ReconstructNormal( texture2D( xNormalSampler, gl_FragCoord.xy * xOneOverSize ).xy );
	// depth still not working :/
	vec3 xPosition = xCameraPosition + xViewVector * ViewZFromDepth( texture2D( xDepthSampler, gl_FragCoord.xy * xOneOverSize ).a, xClipPlanes );
	vec3 xLightVector = xPosition - xLightPosition;
	float fAttenuation = 1.0f - dot( xLightVector, xLightVector ) / ( fLightRadius * fLightRadius );
	float fNormalDot = 1.0; // TODO...
	vec3 xColour = xLightColour * fAttenuation * fNormalDot * xDiffuse;
	gl_FragColor = vec4( xColour, 1.0 );
}