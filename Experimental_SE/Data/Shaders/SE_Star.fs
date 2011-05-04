varying vec3 xDirection;
varying vec3 xPosition;

uniform vec4 xPositionAndTime;
uniform vec4 xColourAndSize;

void main()
{
	vec3 xAdjustedPosition = xPosition - xPositionAndTime.xyz;
    	// x^2 + y^2 + z^2 - r^2 = 0
    	// t^2(dx^2 + dy^2 + dz^2) + 2t(oxdx + oydy + ozdz) + (ox^2 + oy^2 + oz^2 - r^2) = 0
	// at^2 + bt + c = 0
	// t = -b - sqrt(b^2 - 4ac) / 2a // (always take the nearest, so discard the '+' solution)

	vec3 xNormalisedDirection = normalize( xDirection );
	
	// SE - 15/10/2010 - this always seems too convenient to not have a geometric interpetation I am missing somehow
	vec3 xQ = 2.0 * vec3( dot( xNormalisedDirection, xNormalisedDirection ), dot( xAdjustedPosition, xNormalisedDirection ), dot( xAdjustedPosition, xAdjustedPosition ) - xColourAndSize.w * xColourAndSize.w );

    	vec4 xColour = vec4( 2.0 * xColourAndSize.xyz, 1.0 );

	float fDiscriminant = xQ.y * xQ.y - xQ.x * xQ.z;
	if( fDiscriminant < 0.0 )
    	{
		float fGlowDot = dot( xNormalisedDirection, normalize( xAdjustedPosition ) );
        	xColour.xyz = -fGlowDot * xColour.xyz;
		xColour.w = 0.0;
    	}

    	float fT = ( -xQ.y - sqrt( fDiscriminant ) ) / xQ.x;
    
	if( fT < 0.0 )
    	{
		float fGlowDot = dot( xNormalisedDirection, normalize( xAdjustedPosition ) );
		xColour.xyz = -fGlowDot * xColour.xyz;
		xColour.w = 0.0;
    	}

    	vec3 xSolution = xNormalisedDirection * fT + xAdjustedPosition;
    
	float fLimb = 0.5 + clamp( dot( normalize( -xSolution ), xNormalisedDirection ), 0.0, 1.0 );
    	float fNoise = 0.5 + clamp( snoise3d( xSolution * 4.0 / xColourAndSize.w * xColourAndSize.z - vec3( 0.0, 0.0, 0.005 * xPositionAndTime.w / xColourAndSize.z ) ) + 0.75f * noise3d( 3.0 * xSolution / xColourAndSize.w + vec3( 0.0, 0.0, 0.01 * xPositionAndTime.w / xColourAndSize.z ) ), 0.0, 1.0 );
    	gl_FragColor = vec4( ( xColour.a == 0.0 )
		? xColour.xyz
		: 0.5 * xColour.xyz * fNoise * fLimb
		, 1.0 );

	gl_FragDepth = 1.0; //gl_DepthRange.diff / xSolution.z;
}
