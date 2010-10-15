varying vec3 xDirection;
varying vec3 xPosition;
uniform sampler2D xTexture;

void main()
{
    // x^2 + y^2 + z^2 - r^2 = 0
    // t^2(dx^2 + dy^2 + dz^2) + 2t(oxdx + oydy + ozdz) + (ox^2 + oy^2 + oz^2 - r^2) = 0
	// at^2 + bt + c = 0
	// t = -b - sqrt(b^2 - 4ac) / 2a // (always take the nearest, so discard the '+' solution)

	vec3 xNormalisedDirection = normalize( xDirection );
	
	// SE - 15/10/2010 - this always seems too convenient to not have a geometric interpetation I am missing somehow
	vec3 xQ = 2.0 * vec3( dot( xNormalisedDirection, xNormalisedDirection ), dot( xPosition, xNormalisedDirection ), dot( xPosition, xPosition ) - 1.0 );
    float fT = ( -xQ.y - sqrt( xQ.y * xQ.y - xQ.x * xQ.z ) ) / xQ.x;
    
    vec3 xSolution = xNormalisedDirection * fT + xPosition;

    if( fT < 0.0 )
    {
        discard;
    }
    
    gl_FragDepth = 1.0; //gl_DepthRange.diff / xSolution.z;
    float fNoise = noise3d( xSolution * 8192.0 );
	vec4 xColour = vec4( fNoise, fNoise, fNoise, 1.0 ) * -dot( normalize( xSolution ), xNormalisedDirection );
	gl_FragColor = xColour;
}
