uniform sampler2D xSampler;

varying vec2 xTexCoord;

void main()
{
	vec3 xDepthSample = texture2D( xSampler, xTexCoord ).xyz;
	float fDepth = xDepthSample.x * 255.0 / 256.0 + xDepthSample.y * 255.0 / 65536.0 + xDepthSample.z * 255.0 / 16777216.0;
	gl_FragColor = vec4( fDepth, fDepth, fDepth, 1.0 );
}
