// the plan for the buffers is...
//
// D = diffuse
// N = normals
// Sm = specular mask
// Sp = specular power
// Sf = specular fresnel term
// H = heightmap
// SD = surface depth (for subsurface effects)
// AO = ambient occlusion
//
// gl_FragData[ 0 ] = Dr Dg Db Fog
// gl_FragData[ 1 ] = Nx Ny H  AO
// gl_FragData[ 2 ] = Sm Sp Sf SD

varying vec2 xTexCoord;
varying vec3 xNormal;
varying vec3 xTangent;
varying vec3 xBinormal;
varying vec3 xPosition;
varying vec3 xViewPosition;
varying float fFog;

uniform mat4x4 xViewMatrix;
uniform mat4x4 xInverseViewMatrix;

uniform sampler2D DiffuseMap;
uniform sampler2D NormalMap;

void main()
{
	vec4 xDiffuseSample = texture2D( DiffuseMap, xTexCoord );
	if( xDiffuseSample.a < 0.95 )
	{
		discard;
	}
	
	vec3 xNormalSample = 2.0 * texture2D( NormalMap, xTexCoord ).xyz - 1.0;
	vec3 xViewNormal = xTangent * xNormalSample.x + xBinormal * xNormalSample.y + xNormal * xNormalSample.z;
	gl_FragData[ 0 ] = vec4( xDiffuseSample.xyz, fFog );
	gl_FragData[ 1 ] = vec4( 0.25 * StereographicProjection( normalize( xViewNormal ) ) + 0.5, 0.0, 0.0 );
	gl_FragData[ 2 ] = vec4( 0.0, 0.0, 0.0, 0.0 );
}