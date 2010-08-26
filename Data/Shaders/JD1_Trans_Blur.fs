uniform sampler2D xFrameBuffer;
uniform float fTime;
uniform float fLastFrameTime;
uniform vec2 xSize;
uniform vec2 xOneOverSize;

varying vec2 xTexCoord;

void main()
{
	vec2 axOffsets[ 8 ] =
	{
		vec2( -xOneOverSize.x, -xOneOverSize.y ),
		vec2( -xOneOverSize.x, 0 ),
		vec2( -xOneOverSize.x, xOneOverSize.y ),
		vec2( 0, -xOneOverSize.y ),
		vec2( 0, xOneOverSize.y ),
		vec2( xOneOverSize.x, -xOneOverSize.y ),
		vec2( xOneOverSize.x, 0 ),
		vec2( xOneOverSize.x, xOneOverSize.y )
	};
	
	const float fMultiplier = 1.0 / 9.0;
	const float fScale = 1.5;
    
	gl_FragColor = fMultiplier * (
		texture2D( xFrameBuffer, vec2( xTexCoord.x, xTexCoord.y ) + fScale * axOffsets[ 0 ] ) +
		texture2D( xFrameBuffer, vec2( xTexCoord.x, xTexCoord.y ) + fScale * axOffsets[ 1 ] ) +
		texture2D( xFrameBuffer, vec2( xTexCoord.x, xTexCoord.y ) + fScale * axOffsets[ 2 ] ) +
		texture2D( xFrameBuffer, vec2( xTexCoord.x, xTexCoord.y ) + fScale * axOffsets[ 3 ] ) +
		texture2D( xFrameBuffer, vec2( xTexCoord.x, xTexCoord.y ) + fScale * axOffsets[ 4 ] ) +
		texture2D( xFrameBuffer, vec2( xTexCoord.x, xTexCoord.y ) + fScale * axOffsets[ 5 ] ) +
		texture2D( xFrameBuffer, vec2( xTexCoord.x, xTexCoord.y ) + fScale * axOffsets[ 6 ] ) +
		texture2D( xFrameBuffer, vec2( xTexCoord.x, xTexCoord.y ) + fScale * axOffsets[ 7 ] ) +
		texture2D( xFrameBuffer, vec2( xTexCoord.x, xTexCoord.y ) ) );
}