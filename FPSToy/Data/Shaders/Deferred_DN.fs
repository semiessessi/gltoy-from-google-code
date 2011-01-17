varying vec2 xTexCoord;
varying vec3 xNormal;
varying vec3 xTangent;
varying vec3 xBinormal;

varying vec3 xPosition;
varying vec3 xViewPosition;

uniform sampler2D DiffuseMap;
uniform sampler2D NormalsMap;

vec3 InverseStereographicProjection( vec2 xProjected )
{
	float fSumOfSquares = dot( xProjected, xProjected );
	return vec3( 2.0 * xProjected.x, 2.0 * xProjected.y, fSumOfSquares - 1.0 ) / ( 1.0 + fSumOfSquares );
}

vec2 StereographicProjection( vec3 xNormalised )
{
	return xNormalised.xy / ( 1.0 - xNormalised.z );
}

void main()
{
	// TODO - use the spare components for:
	// fresnel specular term
	// fog alpha
	// heightmap?
	
	// diffuse + spare component (fresnel term)
	gl_FragData[ 0 ] = vec4( texture2D( DiffuseMap, xTexCoord ).xyz, 0.0 );
	
	// TODO: transform a normal map with the basis vectors and encode the result
	// normal and two spare components (fog alpha + heightmap?)
	// NOTE: this only encodes less than 3/4 of the sphere (but more than 1/2)...
	// TODO: work out how to get this into view space...
	//vec3 xViewNormal = ...;
	//gl_FragData[ 1 ] = vec4( 0.25 * StereographicProjection( xNormal ) + 0.5, 0.0, 0.0 );
	// actually, put out world space normals for now and get the fancy compression working later...
	// HACK: crappy depth in alpha...
	gl_FragData[ 1 ] = vec4( 0.5 * xNormal + 0.5, xViewPosition.z / 1000.0 );
	
	// TODO: specular
	// gl_FragData[ 2 ] = texture2D( xSpecularSampler, xTexCoord );
}