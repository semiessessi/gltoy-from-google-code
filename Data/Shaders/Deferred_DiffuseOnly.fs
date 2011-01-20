varying vec2 xTexCoord;
varying vec3 xNormal;
varying vec3 xTangent;
varying vec3 xBinormal;

varying vec3 xPosition;
varying vec3 xViewPosition;

uniform sampler2D DiffuseMap;

void main()
{
	// TODO - use the spare components for:
	// fresnel specular term
	// fog alpha
	// heightmap?
	
	// diffuse + spare component (fresnel term)
	gl_FragData[ 0 ] = vec4( texture2D( DiffuseMap, xTexCoord ).xyz, 0.0 );
	gl_FragData[ 1 ] = vec4( CompressNormal( xNormal ), 0.0, 0.0 );
	// gl_FragData[ 2 ] = vec4( 0.0, 0.0, 0.0, 0.0 );
}