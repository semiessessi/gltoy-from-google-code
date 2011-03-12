varying vec3 xDirection;
varying vec3 xPosition;
uniform sampler2D xTexture1;
uniform sampler2D xTexture2;
uniform sampler2D xTexture3;
uniform sampler2D xTexture4;
uniform vec4 xParams1;
uniform vec4 xParams2;

void main()
{
    vec3 xNormalisedDirection = normalize( xDirection );

    vec4 xPlane1 = vec4( 0.0, 0.0, -1.0, xParams1.x );
    vec4 xPlane2 = vec4( 0.0, 0.0, -1.0, xParams1.y );
	//vec4 xPlane3 = vec4( 0.0, 0.0, -1.0, xParams2.x );
	//vec4 xPlane4 = vec4( 0.0, 0.0, -1.0, xParams2.y );

    vec3 xAdjustedPosition = xPosition + vec3( 0.0, xParams1.z, 0.0 );

    float fT1 = -dot( xPlane1, vec4( xAdjustedPosition, 1.0 ) ) / ( dot( xNormalisedDirection, xPlane1.xyz ) );
    float fT2 = -dot( xPlane2, vec4( xAdjustedPosition, 1.0 ) ) / ( dot( xNormalisedDirection, xPlane2.xyz ) );
	//float fT3 = -dot( xPlane3, vec4( xAdjustedPosition, 1.0 ) ) / ( dot( xNormalisedDirection, xPlane3.xyz ) );
	//float fT4 = -dot( xPlane4, vec4( xAdjustedPosition, 1.0 ) ) / ( dot( xNormalisedDirection, xPlane4.xyz ) );
    
    vec3 xSolution1 = xNormalisedDirection * fT1 + xAdjustedPosition;
    vec3 xSolution2 = xNormalisedDirection * fT2 + xAdjustedPosition;
	//vec3 xSolution3 = xNormalisedDirection * fT3 + xAdjustedPosition;
	//vec3 xSolution4 = xNormalisedDirection * fT4 + xAdjustedPosition;
    
    vec4 xSample1 = texture2D( xTexture1, 0.1 * vec2( xSolution1.x, xSolution1.y ) );
    vec4 xSample2 = texture2D( xTexture2, 0.1 * vec2( xSolution2.x, xSolution2.y ) );
	//vec4 xSample3 = texture2D( xTexture3, 0.1 * vec2( xSolution3.x, xSolution3.y ) );
	//vec4 xSample4 = texture2D( xTexture4, 0.1 * vec2( xSolution4.x, xSolution4.y ) );

    //gl_FragColor = clamp( mix( mix( mix( xSample4, xSample3, xSample3.a ),xSample2, xSample2.a ), xSample1, xSample1.a ), 0.0, 1.0 );
    gl_FragColor = clamp( mix( xSample2, xSample1, xSample1.a ), 0.0, 1.0 );
    gl_FragDepth = 1.0;
}