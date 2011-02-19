varying vec3 xDirection;
varying vec3 xPosition;
uniform sampler2D xTexture1;
uniform sampler2D xTexture2;
uniform vec4 xParams;

void main()
{
    vec3 xNormalisedDirection = normalize( xDirection );

    vec4 xPlane1 = vec4( 0.0, 0.0, -1.0, xParams.x );
    vec4 xPlane2 = vec4( 0.0, 0.0, -1.0, xParams.y );

    vec3 xAdjustedPosition = xPosition + vec3( 0.0, xParams.z, 0.0 );

    float fT1 = -dot( xPlane1, vec4( xAdjustedPosition, 1.0 ) ) / ( dot( xNormalisedDirection, xPlane1.xyz ) );
    float fT2 = -dot( xPlane2, vec4( xAdjustedPosition, 1.0 ) ) / ( dot( xNormalisedDirection, xPlane2.xyz ) );
    
    vec3 xSolution1 = xNormalisedDirection * fT1 + xAdjustedPosition;
    vec3 xSolution2 = xNormalisedDirection * fT2 + xAdjustedPosition;
    
    vec4 xSample1 = texture2D( xTexture1, 0.1 * vec2( xSolution1.x, xSolution1.y ) );
    vec4 xSample2 = texture2D( xTexture2, 0.1 * vec2( xSolution2.x, xSolution2.y ) );

    gl_FragColor = 0.25 * ( xSample1 * xSample1.a + xSample2 * xSample2.a );
}