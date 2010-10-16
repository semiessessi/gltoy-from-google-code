varying vec3 xDirection;
varying vec3 xPosition;
uniform sampler2D xTexture;

void main()
{
    // ax + by + cz + d = 0
    // t = -( d + aox + boy + coz ) / ( adx + bdy + cdz )
    vec3 xNormalisedDirection = normalize( xDirection );
    vec4 xPlane = vec4( 0.0, -1.0, 0.0, 0.0 );
    float fT = -dot( xPlane, vec4( xPosition, 1.0 ) ) / ( dot( xNormalisedDirection, xPlane.xyz ) );
    
    vec3 xSolution = xNormalisedDirection * fT + xPosition;

    if( fT < 0.0 )
    {
        discard;
    }
    
    gl_FragDepth = 1.0; //gl_DepthRange.diff / xSolution.z;
    vec4 xColour = texture2D( xTexture, 0.015625 * vec2( xSolution.x, xSolution.z ) );
    gl_FragColor = xColour;
}