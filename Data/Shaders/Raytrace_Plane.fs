varying vec3 xDirection;
varying vec3 xPosition;
uniform sampler2D xTexture;

void main()
{
    // ax + by + cz + d = 0
    // t = -( d + aox + boy + coz ) / ( adx + bdy + cdz )
    normalize( xDirection );
    vec4 xPlane = vec4( 0.0, -1.0, 0.0, 0.0 );
    vec3 xPlaneNormal = vec3( xPlane );
    float fT = -( xPlane.w + dot( xPosition, xPlaneNormal ) ) / ( dot( xDirection, xPlaneNormal ) );
    
    vec3 xSolution = xDirection * fT + xPosition;

    if( fT < 0.0 )
    {
        discard;
    }
    
    gl_FragDepth = 1.0f; //gl_DepthRange.diff / xSolution.z;
    vec4 xColour = texture2D( xTexture, 0.015625 * vec2( xSolution.x, xSolution.z ) );
    gl_FragColor = xColour;
}