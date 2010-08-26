uniform sampler2D xFrameBuffer;
uniform float fTime;
uniform float fLastFrameTime;
uniform vec2 xSize;
uniform vec2 xOneOverSize;
uniform vec2 xPoint0;
uniform vec2 xPoint1;
uniform vec2 xPoint2;
uniform vec2 xPoint3;

varying vec2 xTexCoord;

void main()
{
	float fAspectRatio = xSize.y / xSize.x;
	vec2 xCoord = vec2( 2.0 * xTexCoord.x - 1.0, ( 2.0 * xTexCoord.y - 1.0 ) * fAspectRatio ); 
	float fD, fR;

	xCoord = xCoord + xPoint0;
	fD = sqrt( dot( xCoord, xCoord ) );
	fR = atan( xCoord[ 1 ], xCoord[ 0 ] ) - 0.175 * fLastFrameTime * ( pow( 2.0, 1.0 / ( fD + 0.175 ) ) - 1.0 );
	xCoord = vec2( fD * cos( fR ), fD * sin( fR ) ) - xPoint0;

	xCoord = xCoord + xPoint1;
	fD = sqrt( dot( xCoord, xCoord ) );
	fR = atan( xCoord[ 1 ], xCoord[ 0 ] ) + 0.175 * fLastFrameTime * ( pow( 2.0, 1.0 / ( fD + 0.175 ) ) - 1.0 );
	xCoord = vec2( fD * cos( fR ), fD * sin( fR ) ) - xPoint1;

	xCoord = xCoord + xPoint2;
	fD = sqrt( dot( xCoord, xCoord ) );
	fR = atan( xCoord[ 1 ], xCoord[ 0 ] ) - 0.175 * fLastFrameTime * ( pow( 2.0, 1.0 / ( fD + 0.175 ) ) - 1.0 );
	xCoord = vec2( fD * cos( fR ), fD * sin( fR ) ) - xPoint2;

	xCoord = xCoord + xPoint3;
	fD = sqrt( dot( xCoord, xCoord ) );
	fR = atan( xCoord[ 1 ], xCoord[ 0 ] ) + 0.175 * fLastFrameTime * ( pow( 2.0, 1.0 / ( fD + 0.175 ) ) - 1.0 );
	xCoord = vec2( fD * cos( fR ), fD * sin( fR ) ) - xPoint3;

	xCoord = vec2( 0.5 + 0.5 * xCoord.x,  0.5 + 0.5 * xCoord.y / fAspectRatio );

    gl_FragColor = texture2D( xFrameBuffer, xCoord );
}